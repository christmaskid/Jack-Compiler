#include <iostream>
#include <fstream>
#include <cstdlib>
#include "CodeGenerator.h"
#define DEBUG 0
using namespace std;

CodeGenerator::CodeGenerator(){
    classList=new JackClass[30];
    classList[0].regist("Math");
    classList[1].regist("String");
    classList[2].regist("Array");
    classList[3].regist("Memory");
    classList[4].regist("Output");
    classList[5].regist("Screen");
    classList[6].regist("Keyboard");
    classList[7].regist("Sys");
    classCnt=8;

    JackClass* jp=&classList[0];
    jp[0].registerSub(FUNC,"void","init",0);
    jp[0].registerSub(FUNC,"int","abs",1);
    jp[0].registerSub(FUNC,"int","multiply",2);
    jp[0].registerSub(FUNC,"int","divide",2);
    jp[0].registerSub(FUNC,"int","min",2);
    jp[0].registerSub(FUNC,"int","max",2);
    jp[0].registerSub(FUNC,"int","sqrt",2);

    jp[1].registerSub(CONSTR,"String","new",2);
    jp[1].registerSub(METH,"void","dispose",1);
    jp[1].registerSub(METH,"int","length",1);
    jp[1].registerSub(METH,"char","charAt",2);
    jp[1].registerSub(METH,"void","setCharAt",3);
    jp[1].registerSub(METH,"String","appendChar",2);
    jp[1].registerSub(METH,"void","eraseLastChar",0);
    jp[1].registerSub(METH,"int","intValue",0);
    jp[1].registerSub(METH,"void","setInt",0);
    jp[1].registerSub(FUNC,"char","backspace",0);
    jp[1].registerSub(FUNC,"char","doubleQuote",0);
    jp[1].registerSub(FUNC,"char","newLine",0);

    jp[2].registerSub(FUNC,"Array","new",1);
    jp[2].registerSub(METH,"void","dispose",1);

    jp[3].registerSub(FUNC,"int","peek",1);
    jp[3].registerSub(FUNC,"void","poke",2);
    jp[3].registerSub(FUNC,"Array","alloc",1);
    jp[3].registerSub(FUNC,"void","deAlloc",1);

    jp[4].registerSub(FUNC,"void","moveCursor",2);
    jp[4].registerSub(FUNC,"void","printChar",1);
    jp[4].registerSub(FUNC,"void","printString",1);
    jp[4].registerSub(FUNC,"void","printInt",1);
    jp[4].registerSub(FUNC,"void","println",0);
    jp[4].registerSub(FUNC,"void","backspace",0);

    jp[5].registerSub(FUNC,"void","clearScreen",0);
    jp[5].registerSub(FUNC,"void","setColor",1);
    jp[5].registerSub(FUNC,"void","drawPixel",2);
    jp[5].registerSub(FUNC,"void","drawLine",4);
    jp[5].registerSub(FUNC,"void","drawRectangle",4);
    jp[5].registerSub(FUNC,"void","drawCircle",3);

    jp[6].registerSub(FUNC,"char","keyPressed",0);
    jp[6].registerSub(FUNC,"char","readChar",0);
    jp[6].registerSub(FUNC,"String","readLine",1);
    jp[6].registerSub(FUNC,"int","readInt",1);

    jp[7].registerSub(FUNC,"void","halt",0);
    jp[7].registerSub(FUNC,"void","error",1);
    jp[7].registerSub(FUNC,"void","wait",1);

}
void CodeGenerator::reset()
{
    ptr=0;
    nowClass=nowSub=0;
}
CodeGenerator::~CodeGenerator(){
    delete [] classList;
}
    /* file processing */
void CodeGenerator::openfile(string name){
    tk.openfile(name);
    fout.open(name+".vm",ios::out);
}
void CodeGenerator::closefile(){
    tk.closefile();
    fout.close();
}
word CodeGenerator::next(){
    return strToWord(tk.next());
}
string CodeGenerator::nextID(){
    return tk.next();
}
void CodeGenerator::eat(word w){
    return tk.eat();
}
    /* code generating*/
void CodeGenerator::generateCode(){Class();}
void CodeGenerator::Class(){
    eat(CLASS); string className=nextID();
    eat(ID); nowClass=classCnt; registerClass(className);
    eat(LLP);
    while(next()==STATIC || next()==FIELD) classVarDec();
    while(next()==CONSTR || next()==METH || next()==FUNC) subroutineDec();
    eat(RLP);
    nowClass++;
}
void CodeGenerator::registerClass(string className){
    classList[classCnt].regist(className);
    classCnt++;
}
void CodeGenerator::classVarDec(){
    word kind=next(); eat(KIND); // static|field
    string type=nextID(); eat(TYPE); // int|char|boolean
    string name=nextID(); eat(ID); // varName
    classList[nowClass].addNewVar(kind,type,name);
    while(next()==COMMA) {
        eat(COMMA);
        name=nextID(); eat(ID);
        classList[nowClass].addNewVar(kind,type,name);
    }
    eat(SEMI);
}
void CodeGenerator::subroutineDec(){
    //cout<<"class "<<nowClass<<": "<<classList[nowClass].name<<endl;
    word kind=next(); eat(KIND);        // constructor|method|function
    string type=nextID(); eat(TYPE);    // void|int|char|boolean|(className)
    string name=nextID(); eat(ID);      //subroutine name
    int arg=parameterList(kind); // arguments
    classList[nowClass].registerSub(kind,type,name,arg);

    eat(LLP);
    subVarDec(); // local variables
    int varNum=classList[nowClass].subList[nowSub].loc;
    fout<<"function "<<classList[nowClass].name<<"."<<name<<" "<<varNum<<endl;


    if(kind==METH) { /// methods
        fout<<"push argument 0"<<endl;
        fout<<"pop pointer 0"<<endl;
    }
    else if(kind==CONSTR){/// constructors
        fout<<"push constant "<<classList[nowClass].fi<<endl;
        fout<<"call Memory.alloc 1"<<endl;
        fout<<"pop pointer 0"<<endl;
    }

    subroutineBody();
    eat(RLP);
    nowSub++;
}
int CodeGenerator::parameterList(word kind){
    int n=0;
    eat(LSP);
    Subroutine* sp=&classList[nowClass].subList[nowSub];
    if(kind==METH) { /// methods
        sp->addNewVar(ARG,classList[nowClass].name,"this");
        n++;
    }
    while(next()!=RSP)
    {
        if(next()==COMMA) eat(COMMA);
        string type=nextID(); eat(TYPE);
        string name=nextID(); eat(ID);
        sp->addNewVar(ARG,type,name);
        n++;
    }
    eat(RSP);
    return n;
}
void CodeGenerator::subroutineBody(){
    whileL=ifL=0;
    statements();
}
void CodeGenerator::subVarDec(){
    Subroutine* sp=&classList[nowClass].subList[nowSub];
    while(next()==VAR)
    {
        eat(VAR);
        string type=nextID(); eat(TYPE);
        string name=nextID(); eat(ID);
        sp->addNewVar(LCL,type,name);
        while(next()==COMMA)
        {
            eat(COMMA);
            name=nextID(); eat(ID);
            sp->addNewVar(LCL,type,name);
        }
        eat(SEMI);
    }
}
void CodeGenerator::statements(){
    while(next()!=RLP) statement();
}
void CodeGenerator::statement(){
    switch(next())
    {
        case LET: letStatement(); break;
        case DO: doStatement(); break;
        case WHILE: whileStatement(); break;
        case IF: ifStatement(); break;
        case RETURN: returnStatement(); break;
        default: break;
    }
}
void CodeGenerator::letStatement(){
    eat(LET);
    string var=nextID();
    eat(ID);
    if(next()==LMP)  /// array: at the left side of '='
    {
        /// a[i]=k
        eat(LMP);
        expression(); /// index(i)
        eat(RMP);
        fout<<"push "<<lookup(var)<<endl;
        fout<<"add"<<endl; /// var+index(a+i)
        eat(EQ);
        expression();
        fout<<"pop temp 0"<<endl; /// result of expression(k)
        fout<<"pop pointer 1"<<endl; /// result of var+index(a+i)
        fout<<"push temp 0"<<endl; /// k
        fout<<"pop that 0"<<endl; /// *(a+i)
    }
    else{
        eat(EQ);
        expression();
        fout<<"pop "<<lookup(var)<<endl;
    }
    eat(SEMI);
}
void CodeGenerator::expression(){
    term();
    while(next()!=SEMI && next()!=RMP && next()!=RSP && next()!=COMMA) {
        if(isOp(next())){
            string oper=op();
            term();
            /// if 'term();' is written here: left-priority
            /// if 'expression();' is written here: right-priority
            fout<<oper<<endl;
        }
        else if(next()==LMP) /// array: at the right side of '='
        {
            eat(LMP);
            expression();
            eat(RMP);
            fout<<"add"<<endl;
            fout<<"pop pointer 1"<<endl;
            fout<<"push that 0"<<endl;
        }
    }
}
void CodeGenerator::term(){
    string id;
    switch(next())
    {
        case intConst: fout<<"push constant "<<nextID()<<endl; eat(intConst); break;
        case StrConst: str(nextID()); eat(StrConst); break;
        case ID: /// variable | subroutineCall | array
            id=nextID(); eat(ID);
            if(next()==PER || next()==LSP) /// subroutineCall
                subroutineCall(id);
            else /// simple variable
                fout<<"push "<<lookup(id)<<endl;
            break;
        case LSP: eat(LSP); expression(); eat(RSP); break;
        case NEG: /// ~
            eat(NEG);
            term(); fout<<"not"<<endl;
            break;
        case SUB: /// - as unaryOp
            eat(NEG); term(); fout<<"neg"<<endl; break;
        case THIS: /// this.XXX
            eat(THIS); fout<<"push pointer 0"<<endl;
            if(next()==PER) {eat(PER); fout<<lookup(nextID())<<endl; eat(ID);}
            break;
        case TRUE: fout<<"push constant 0"<<endl<<"not"<<endl; eat(TRUE); break;
        case FALSE: fout<<"push constant 0"<<endl; eat(FALSE); break;
        case NUL: fout<<"push constant 0"<<endl; eat(NUL); break;
        default: break;
    }
}
void CodeGenerator::str(string s){
    int n=s.length();
    fout<<"push constant "<<n-2<<endl;
    fout<<"call String.new 1"<<endl;
    for(int i=1;i<n-1;i++)
    {
        fout<<"push constant "<<(int)(s[i])<<endl;
        fout<<"call String.appendChar 2"<<endl;
    }
}
string CodeGenerator::op(){
    switch(next())
    {
        case ADD: eat(ADD); return "add";
        case SUB: eat(SUB); return "sub";
        case MUL: eat(MUL); return "call Math.multiply 2";
        case DIV: eat(DIV); return "call Math.divide 2";
        case AND: eat(AND); return "and";
        case OR:  eat(OR);  return "or";
        case LT:  eat(LT);  return "lt";
        case GT:  eat(GT);  return "gt";
        case EQ:  eat(EQ);  return "eq";
        default: break;
    }
    return 0;
}
Variable CodeGenerator::lookupTable(string target){
    Variable ans;
    JackClass* jp=&classList[nowClass];
    Subroutine* sp=&(jp->subList[nowSub]);
    int jvc=jp->varCount,svc=sp->varCount;

    /* method scope */
    for(int i=0;i<svc;i++)
    {
        if(sp->varList[i].name==target)
            return sp->varList[i];
    }
    /* class scope */
    for(int i=0;i<jvc;i++)
    {
        if(jp->varList[i].name==target) {
            return jp->varList[i];
        }
    }
    return ans;
}
string CodeGenerator::lookup(string target)
{
    Variable ans=lookupTable(target);
    if(ans.num==-1) return "NULL";
    return wordToStr(ans.kind)+" "+numToString(ans.num);
}
string CodeGenerator::lookupType(string target){
    return lookupTable(target).type;
}
void CodeGenerator::doStatement(){
    eat(DO);
    string id=nextID(); eat(ID);
    isVoid=true;
    subroutineCall(id);
    isVoid=false;
    eat(SEMI);
}
void CodeGenerator::subroutineCall(string id){
    int argNum;
    bool isObject=(lookup(id)!="NULL");

    if(next()==LSP) /// method; within the same class
    {
        fout<<"push pointer 0"<<endl;
        argNum=expressionList()+1;
        fout<<"call "<<classList[nowClass].name<<"."<<id<<" "<<argNum<<endl;
    }
    else if(next()==PER && isObject) /// object.method
    {
        string classType=lookupType(id);
        eat(PER);
        string subName=nextID(); eat(ID);
        fout<<"push "<<lookup(id)<<endl;
        argNum=expressionList()+1;
        fout<<"call "<<classType<<"."<<subName<<" "<<argNum<<endl;
    }
    else if(next()==PER) /// class.function/constructor();
    {
        eat(PER);
        string subName=nextID(); eat(ID);
        argNum=expressionList();
        fout<<"call "<<id<<"."<<subName<<" "<<argNum<<endl;
    }
    if(isVoid) fout<<"pop temp 0"<<endl; /// void
}
int CodeGenerator::expressionList(){
    eat(LSP);
    int n=0;
    if(next()!=RSP)
    {
        n++;
        expression();
        while(next()==COMMA)
        {
            eat(COMMA);
            expression();
            n++;
        }
    }
    eat(RSP);
    return n;
}
void CodeGenerator::whileStatement(){
    eat(WHILE); eat(LSP);
    int lab=whileL;
    whileL++;
    fout<<"label WHILE_EXP"<<lab<<endl;
    expression(); // condition
    eat(RSP);
    eat(LLP);
    fout<<"not"<<endl;
    fout<<"if-goto WHILE_END"<<lab<<endl;
    statements();
    fout<<"goto WHILE_EXP"<<lab<<endl;
    fout<<"label WHILE_END"<<lab<<endl;
    eat(RLP);
}
void CodeGenerator::ifStatement(){
    eat(IF); eat(LSP);
    expression();
    eat(RSP);
    int lab=ifL;
    ifL++;
    fout<<"if-goto IF_TRUE"<<lab<<endl;
    fout<<"goto IF_FALSE"<<lab<<endl;

    fout<<"label IF_TRUE"<<lab<<endl;
    eat(LLP);
    statements();
    eat(RLP);

    if(next()==ELSE)
    {
        fout<<"goto IF_END"<<lab<<endl;
        fout<<"label IF_FALSE"<<lab<<endl;
        eat(ELSE); eat(LLP);
        statements();
        eat(RLP);
        fout<<"label IF_END"<<lab<<endl;
    }
    else
        fout<<"label IF_FALSE"<<lab<<endl;
}
void CodeGenerator::returnStatement(){
    eat(RETURN);
    if(next()==SEMI) /// void
    {
        fout<<"push constant 0"<<endl;
        eat(SEMI);
    }
    else {expression(); eat(SEMI);}
    fout<<"return"<<endl;
}

/* tools */
string numToString(int n){
    if(n==0) return "0";
    string ans="";
    bool neg=false;
    if(n<0) {neg=true; n*=-1;}
    while(n)
    {
        ans=(char)(n%10+'0')+ans;
        n/=10;
    }
    if(neg) ans="-"+ans;
    return ans;
}
bool isOp(word w){return (ADD<=w && w<=NEG);}
bool isStatement(word w){return (LET<=w && w<=RETURN);}
