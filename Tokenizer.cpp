/// Dec. 27~29, 2020, by Yu-Tong Cheng
#include "Tokenizer.h"
#include <iostream>
#include <sstream>
#include <cstring>
#include <queue>
Tokenizer::Tokenizer()
{
    reset();
}
void Tokenizer::reset()
{
    comment=false;
    line=0;
    while(!stk.empty()) stk.pop();
}
void Tokenizer::openfile(string name)
{
    fin.open(name+".jack",ios::in);
}
void Tokenizer::closefile()
{
    fin.close();
}
bool Tokenizer::END()
{
    if(fin.eof()) return true;
    return false;
}
void Tokenizer::tokenize()
{
    string in;
    getline(fin,in);
    stringstream ss;
    ss.str(in);
    string tmp,t,_t;
    int st,ed;
    //cout<<"Read>>"<<in<<endl;

    while(ss>>tmp){
        //cout<<"ss>>"<<tmp<<endl;
        int len=tmp.length();
        /// remove in-line comments, cross-line comments, and empty lines
        if(len==0) continue;
        if(len>=2 && (tmp[0]=='/' && tmp[1]=='/')) break; /// line comment
        if(tmp[0]=='/' && tmp[1]=='*') /// start of cross-line comment
        {
            while(1)
            {
                if(ss>>tmp) {
                    if(tmp[tmp.length()-2]=='*' && tmp[tmp.length()-1]=='/')
                        {ss>>tmp; break;}
                }
                else {ss.clear(); getline(fin,in); ss.str(in); line++; }
            }
        }
        if(tmp[tmp.length()-2]=='*' && tmp[tmp.length()-1]=='/') {comment=false; break;} /// end  of cross-line comment
        while(quotaCount(tmp)%2==1) { /// addressing white spaces in strings
            tmp=tmp.substr(0,tmp.find('"'));
            t=ss.str();
            st=t.find('"');
            _t=t.substr(st,len-1);
            ed=t.length();
            tmp+=t.substr(st,ed);
            ss.clear(); getline(fin,in); ss.str(in); line++;
        }
        process(tmp);
    }
    ss.clear();
    line++;
}

void Tokenizer::process(string s)
{
    s+='\\';
    //cout<<"token<<"<<s<<endl;
    string out="";
    int len=s.length(),pre=0;
    bool str=false;
    for(int i=0;i<len;i++)
    {
        out="";
        if(s[i]=='"') str=!str;
        if(isSymbol(s[i]) && !str)
        {
            for(int j=pre;j<i;j++) out+=s[j];
            if(out.length()>0) stk.push(out);
            while(isSymbol(s[i]) && s[i]!='\\'){
                out=""; out+=s[i++]; stk.push(out);
            }
            pre=i;
            if(s[i]=='"') str=!str;
        }
    }
}

string Tokenizer::next()
{
    while(stk.empty()) tokenize();
    return stk.front();
}
void Tokenizer::eat()
{
    while(stk.empty()) tokenize();
    stk.pop();
}
bool Tokenizer::isEmpty()
{
    return stk.empty();
}

bool isSymbol(char c)
{
    char charList[]={"{}[]().,;+-*/&|<>=~\\ "};
    int len=strlen(charList);
    for(int i=0;i<len;i++)
    {
        if(c==charList[i]) return true;
    }
    return false;
}
int quotaCount(string s)
{
    int len=s.length(),ans=0;
    for(int i=0;i<len;i++)
    {
        if(s[i]=='"') ans++;
    }
    return ans;
}
