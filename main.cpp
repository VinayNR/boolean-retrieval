#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <math.h>
//#include "porter_stemmer.h"
#include <dirent.h>

using namespace std;
//using namespace Porter2Stemmer::internal;

vector<string> stop;
vector<int> length(10,0);
vector<float> prior(10);
map<string,vector<pair<int,int> > > voc[10];
map<string,float> prob[10];
unsigned int docID=1,pos,len=0;
int classID=-2;

vector<pair<int,int> > intersect(vector<pair<int,int> > p1, vector<pair<int,int> > p2)
{
    vector<pair<int,int> > res;
    unsigned int i=0,j=0;
    while(i<p1.size() && j<p2.size())
    {
        if(p1[i].first==p2[j].first)
        {
            res.push_back(make_pair(p1[i].first,0));
            i++;
            j++;
        }
        else if(p1[i].first<p2[j].first)
            i++;
        else
            j++;
    }
    return res;
}

void search(string q)
{
    cout<<"\nSearch Results (All the returned Documents):-\n";
    vector<pair<int,int> > ans,temp;
    int flag=0;
    vector<pair<int,int> >:: iterator r;
    string s1;
    istringstream iss(q);
    iss>>s1;
    ans=voc[0][s1];
    while(iss)
    {
        iss>>s1;
        temp=voc[0][s1];
        ans=intersect(ans,temp);
    }
    for(r=ans.begin();r!=ans.end();r++)
    {
        if(r==ans.end()-1)
        {
            cout<<r->first<<endl;
            flag = 1;
            break;
        }
        if(r->first!=(r+1)->first)
        cout<<r->first<<endl;
        flag = 1;
     }
     if(flag==0)
        cout<<"No matching documents found!\n";
}

void display()
{
    cout<<"\nIndex-:\n\n";
    map<string,vector<pair<int,int> > >:: iterator it;
    voc[classID].erase(voc[classID].begin());
    for(it=voc[classID].begin();it!=voc[classID].end();it++)
    {
        cout<<it->first;
        vector<pair<int,int> >:: iterator q;
        for(q=it->second.begin();q!=it->second.end();q++)
            cout<<"->"<<q->first<<","<<q->second;
        cout<<endl;
    }
}

void extract_file(ifstream &fin)
{
    pos=1;
    if (fin.is_open())
    {
        string word;
        while(fin>>word)
        {
            string res;
            remove_copy_if(word.begin(),word.end(),back_inserter(res),ptr_fun<int,int>(&ispunct));
            for(unsigned int i=0;i<res.length();i++)
                if(isupper(res[i]))
                    res[i]=tolower(res[i]);
            if(find(stop.begin(),stop.end(),res)==stop.end())
            {
                //Porter2Stemmer::stem(res);
                voc[classID][res].push_back(make_pair(docID,pos));
            }
            pos++;
        }
        fin.close();
        length[classID]+=pos;
    }
    else
        cout <<"\nUnable to open this file:";
}

void calcprob(string word, vector<float> & test)
{
    for(int i=0;i<classID;i++)
    {
        if(prob[i].find(word)==prob[i].end())
            prob[i][word]=((float)voc[i][word].size()+1)/(length[i]+len);
        test[i]+=log(prob[i][word]);
    }
}

void classify(ifstream & fin)
{
    int i;
    string word;
    vector<float> test(9,0.0);
    for(i=0;i<classID;i++)
        test[i]+=log(prior[i]);
    if(fin.is_open())
    {
        while(fin>>word)
        {
            string res;
            remove_copy_if(word.begin(),word.end(),back_inserter(res),ptr_fun<int,int>(&ispunct));
            for(unsigned int i=0;i<res.length();i++)
                if(isupper(res[i]))
                    res[i]=tolower(res[i]);
            calcprob(res,test);
        }
        priority_queue<pair<float, int> > q;
        for (unsigned int i=0;i<test.size();i++)
            q.push(make_pair(test[i],i+1));
        for (i=0;i<3;i++)
        {
            cout<<q.top().second<<" ";
            q.pop();
        }
    }
    else
        cout<<"\nUnable to open file!";
}

void trainNB()
{
    cout << "\nTraining starts";
    DIR *dir;
    struct dirent *ent;
    if (((dir = opendir("Train/"))) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            char S[100]="Train/";
            strcat(S, ent->d_name);
            DIR *folder;
            struct dirent *file;
            if ((((folder = opendir(S))) != NULL) && classID>=0)
            {
                while((file = readdir(folder))!=NULL)
                {
                    char T[200];
                    strcpy(T,S);
                    strcat(T,"/");
                    ifstream fin(strcat(T,file->d_name));
                    if(fin.is_open())
                    {
                        extract_file(fin);
                        docID++;
                    }
                }
                closedir(folder);
            }
            else
            {
              cout << "\nDirectory of class " << classID << " could not be opened";
            }
            classID++;
        }
        closedir(dir);
    }
    else
    {
      cout << "\n Main directory could not be opened!";
    }
    for(int i=0;i<classID;i++)
    {
        prior[i]=1.0/classID;
        len+=voc[i].size();
    }
    cout << "\nTraining ends";
}

void testNB()
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("Test/")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            char S[100]="Test/";
            ifstream fin(strcat(S,ent->d_name));
            if(fin.is_open())
            {
                cout<<"\n\n"<<ent->d_name<<" belongs to ";
                classify(fin);
            }
            fin.close();
        }
        closedir(dir);
    }
}

int main()
{
    //----------- READ STOP WORDS ---------------------------------
    ifstream fin("stop.txt");
    if (fin.is_open())
    {
        string word;
        while(fin>>word)
            stop.push_back(word);
    }
    fin.close();
    //--------------- STOP WORDS ----------------------------------

    trainNB();

    testNB();

    return 0;
}


    //---------------- BOOLEAN QUERY ------------------------------------
    cout<<"\nEnter a Boolean Query:";
    string q;
    getline(cin,q);
    search(q);
    //---------------- BOOLEAN QUERY ------------------------------------
