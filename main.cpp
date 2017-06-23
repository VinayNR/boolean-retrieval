#include <iostream>
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <string.h>
#include <algorithm>
#include <dirent.h>

using namespace std;

vector<string> stop;
vector<int> length(2,0);
vector<float> prior(2);
map<string,vector<pair<int,int> > > voc[2];
map<string,float> prob[2];
unsigned int docID=1,pos,classID=0;

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
            for(unsigned int i=0;i<word.length();i++)
            {
                if(ispunct(word[i]))
                    word.erase(word.begin()+i);
                else if(isupper(word[i]))
                    word[i]=tolower(word[i]);
            }
            if(find(stop.begin(),stop.end(),word)==stop.end())
                voc[classID][word].push_back(make_pair(docID,pos));
            pos++;
        }
        fin.close();
        length[classID]+=pos;
    }
    else
        cout <<"\nUnable to open this file:";
}

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

void calcprob(string word, vector<float> & test)
{
    for(unsigned int i=0;i<classID;i++)
    {
        if(prob[i].find(word)==prob[i].end())
            prob[i][word]=((float)voc[i][word].size()+1)/(length[i]+voc[0].size()+voc[1].size());
        test[i]+=log(prob[i][word]);
    }
}

void classify(ifstream & fin)
{
    string word;
    vector<float> test(2,0.0);
    for(unsigned int i=0;i<classID;i++)
        test[i]+=log(prior[i]);
    if(fin.is_open())
    {
        while(fin>>word)
        {
            for(unsigned int i=0;i<word.length();i++)
            {
                if(ispunct(word[i]))
                    word.erase(word.begin()+i);
                else if(isupper(word[i]))
                    word[i]=tolower(word[i]);
            }
            calcprob(word,test);
        }
        cout<<"\nProbabilities-:\n";
        cout<<test[0]<<" "<<test[1];
        cout<<"\nThe file belongs to class "<<max_element(test.begin(), test.end()) - test.begin()+1<<endl;
    }
    else
        cout<<"\nUnable to open file!";
}

int main()
{
    unsigned int x;
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

    //--------------- CLASS 1 DOCUMENTS ---------------------------
    const string str(".txt");
    DIR *dir;
    struct dirent *ent;
    if (((dir = opendir("C:\\Users\\Dell\\Desktop\\ir-temp\\Texts\\train1\\"))) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            char S[100]="C:\\Users\\Dell\\Desktop\\ir-temp\\Texts\\train1\\";
            string s(ent->d_name);
            //if(s.find(str)!=string::npos)
            {
                ifstream fin(strcat(S,s.data()));
                if(fin.is_open())
                    extract_file(fin);
                //else
                //    cout<<"\nFile not opened";
                fin.close();
                docID++;
            }
        }
        //display();
        closedir(dir);
        classID++;
        x=docID-1;
    }
    //------------------- CLASS 1 DOCUMENTS TAKEN -----------------------

    //------------------- CLASS 2 DOCUMENTS -----------------------------
    if ((dir = opendir ("C:\\Users\\Dell\\Desktop\\ir-temp\\Texts\\train2")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string s(ent->d_name);
            char S[100]="C:\\Users\\Dell\\Desktop\\ir-temp\\Texts\\train2\\";
            //if(s.find(str)!=string::npos)
            {
                ifstream fin(strcat(S,s.data()));
                if(fin.is_open())
                    extract_file(fin);
                //else
                //    cout<<"\nFile not opened";
                fin.close();
                docID++;
            }
        }
        //display();
        closedir(dir);
        classID++;
        prior[0]=(float)x/(docID-1);
        prior[1]=1-prior[0];
    }
    //----------------- CLASS 2 DOCUMENTS TAKEN ------------------------
    else
        cout<<"\nCould not access the collection!";

    //----------------- TEST DOCUMENT ----------------------------------
    if ((dir = opendir ("C:\\Users\\Dell\\Desktop\\ir-temp\\Texts\\test")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string s(ent->d_name);
            char S[100]="C:\\Users\\Dell\\Desktop\\ir-temp\\Texts\\test\\";
            //if(s.find(str)!=string::npos)
            {
                ifstream fin(strcat(S,s.data()));
                if(fin.is_open())
                    classify(fin);
                    //cout<<ent->d_name<<endl;
                //else
                //    cout<<"\nFile not opened";
                fin.close();
            }
        }
    }
    //---------------- TEXT DOCUMENT CLASSIFIED -------------------------

    //---------------- BOOLEAN QUERY ------------------------------------
    /*cout<<"\nEnter a Boolean Query:";
    string q;
    getline(cin,q);
    search(q);*/
    //---------------- BOOLEAN QUERY ------------------------------------
    return 0;
}
