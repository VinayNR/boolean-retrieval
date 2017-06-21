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
map<string, float> prob;
map<string,vector<pair<int,int> > > voc;
int docID=1, pos;
float posclass=-1;

class LinkedList{

struct Node {
    string word;
    float prob;
    Node *next;
};

  Node *head; //head pointer

public:
// constructor
LinkedList(){
    head = NULL;  //head to NULL
}

void addWord(string ele, float p){
    Node *prob_list = new Node();
    prob_list->word = ele;
    prob_list->prob = p;
    prob_list->next = head;

    head = prob_list;
}

string popWord(){
    Node *n = head;
    string ret = n->word;

    head = head->next;
    delete n;
    return ret;
  }

void disp()
{
  Node *i = head;
  while(i!=NULL)
  {
    cout<<i->word<<" "<<i->prob<<endl;
    i++;
  }
}
}listclass;


void index(string s)
{
    istringstream iss(s);
    do
    {
        string sub;
        iss>>sub;
        if(find(stop.begin(),stop.end(),sub)==stop.end())
          {
              voc[sub].push_back(make_pair(docID,pos++));
              posclass++;
          }
          else
          {
            posclass++;
          }
    }while(iss);
}

void display()
{
    cout<<"\nIndex-:\n\n";
    map<string,vector<pair<int,int> > >:: iterator it;
    voc.erase(voc.begin());
    for(it=voc.begin();it!=voc.end();it++)
    {
        cout<<it->first;
        vector<pair<int,int> >:: iterator q;
        for(q=it->second.begin();q!=it->second.end();q++)
            cout<<"->"<<q->first<<","<<q->second;
        cout<<endl;
    }
}

void displayprob()
{
  cout<<"\nProbabilities:-\n";
  map<string, float> :: iterator q;
  for(q=prob.begin();q!=prob.end();q++)
  {
    cout<<q->first<<" "<<q->second<<endl;
  }
}

void prob_calc()
{
  map<string,vector<pair<int,int> > >:: iterator it;
  voc.erase(voc.begin());
  for(it=voc.begin();it!=voc.end();it++)
  {
      vector<pair<int, int> >:: iterator q;
      float size = 0;
      for(q=it->second.begin();q!=it->second.end();q++)
      {
        size++;
      }
      //listclass.addWord(it->first, float((size)/(posclass)));
      prob[it->first] = float((size)/(posclass));
  }
}


void extract_file(ifstream &fin)
{
    pos=1;
    string line;
    if (fin.is_open())
    {
        while(getline(fin,line))
        {
            for(unsigned int i=0;i<line.length();i++)
            {
                if(ispunct(line[i]))
                    line.erase(line.begin()+i);
            }
            transform(line.begin(),line.end(),line.begin(),::tolower);
            index(line);
            pos--;
        }
        fin.close();
    }
    else
        cout <<"\nUnable to open this file";
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
    ans=voc[s1];
    while(iss)
    {
        iss>>s1;
        temp=voc[s1];
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

int main()
{
    //read stop words
    string word;
    ifstream fin("stop.txt");
    if (fin.is_open())
    {
        while(!fin.eof())
        {
            fin>>word;
            stop.push_back(word);
        }
    }
    fin.close();
    const string str(".txt");
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir ("/Users/apple/Desktop/boolean-retrieval/data/dataex2/")) != NULL)
    {
        while ((ent = readdir (dir)) != NULL)
        {
            string s(ent->d_name);
            if(s.find(str)!=string::npos && (strcmp(ent->d_name,"stop.txt")!=0))
            {
                char S[100] = "/Users/apple/Desktop/boolean-retrieval/data/dataex2/";
                ifstream fin(strcat(S, s.data()));
                extract_file(fin);
                //cout<<ent->d_name<<endl;
                docID++;
            }
        }
        display();
        cout<<"The total number of words in this class is = "<<posclass<<endl;
        prob_calc();
        //listclass.disp();
        displayprob();
        closedir(dir);
    }
    else
        cout<<"\nCould not access the collection!";
    //cout<<"\nEnter a Boolean Query:";
    //string q;
    //getline(cin,q);
    //search(q);
    return 0;
}
