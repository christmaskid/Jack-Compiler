#include "JackClass.h"

JackClass::JackClass(){
    varList=new Variable[20];
    subList=new Subroutine[20];
    varCount=subCount=0;
    st=fi=0;
}
JackClass::~JackClass(){
    delete [] varList;
    delete [] subList;
}
#if DEBUG
void JackClass::showList(){
    cout<<"Show class "<<name<<". varCount:"<<varCount<<endl;
    for(int i=0;i<varCount;i++) cout<<wordToStr(varList[i].kind)<<"\t"<<varList[i].type<<"\t"<<varList[i].name<<"\t"<<varList[i].num<<endl;
    cout<<"subCount:"<<subCount<<endl;
    for(int i=0;i<subCount;i++) subList[i].showList();
}
#endif
void JackClass::regist(string str) {name=str;}
void JackClass::addNewVar(word kind,string type,string varName){
    int tmp;
    if(kind==STATIC) {tmp=st; st++;}
    else if(kind==FIELD) {tmp=fi; fi++;}
    varList[varCount].setData(kind,type,varName,tmp);
    varCount++;
}
void JackClass::registerSub(word kind,string type,string name){
    subList[subCount].setData(kind,type,name);
    subCount++;
}
void JackClass::registerSub(word kind,string type,string name,int arg){
    subList[subCount].setData(kind,type,name,arg);
    subCount++;
}
