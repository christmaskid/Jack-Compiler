#include <iostream>
#include <fstream>
#include "JackParser.h"
using namespace std;

class CodeGenerator
{
private:
    fstream fout;
    string s;
    int ptr;
    JackClass* classList;
    int classCnt;
    int whileL,ifL;
    int nowClass,nowSub;
    bool isVoid;
    Tokenizer tk;

public:
    CodeGenerator();
    ~CodeGenerator();
    void reset();

    void eat(word);
    void openfile(string name);
    void closefile();
    void peekline();
    word next();
    string nextID();
    bool END();

    void generateCode();

    void Class();
    void registerClass(string);
    void classVarDec();
    void subroutineDec();
    int parameterList(word);
    void subroutineBody();
    void subVarDec();
    void statements();
    void statement();
    void letStatement();
    void expression();
    void term();
    string op();
    Variable lookupTable(string);
    string lookup(string);
    string lookupType(string);
    void str(string);
    void doStatement();
    void subroutineCall(string);
    int expressionList();
    void whileStatement();
    void ifStatement();
    void returnStatement();
};
bool isStatement(word);
bool isOp(word);
string numToString(int);

