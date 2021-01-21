#include "Subroutine.h"

Subroutine::Subroutine(){
    varList=new Variable[10];
    varCount=loc=arg=0;
}
Subroutine::~Subroutine(){
    delete [] varList;
}
void Subroutine::addNewVar(word kind,string type,string name){
    int tmp=-1;
    if(kind==LCL) {tmp=loc; loc++;}
    else if(kind==ARG) {tmp=arg; arg++;}
    varList[varCount].setData(kind,type,name,tmp);
    varCount++;
}
void Subroutine::setData(word k,string t,string na){
    kind=k; type=t; name=na;
}
void Subroutine::setData(word k,string t,string na,int ar){
    kind=k; type=t; name=na; arg=ar;
}
#if DEBUG
void Subroutine::showList(){
    cout<<"Show subroutine "<<name<<". varCount:"<<varCount<<", arg="<<arg<<endl;
    for(int i=0;i<varCount;i++) cout<<wordToStr(varList[i].kind)<<"\t"<<varList[i].type<<"\t"<<varList[i].name<<"\t"<<varList[i].num<<"\t"<<endl;
}
#endif
