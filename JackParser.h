#include <iostream>
#include <fstream>
#include "Tokenizer.h"
using namespace std;

class Parser
{
private:

    fstream fin,fout;
    string s;
    int depth;
    int ptr;
    Tokenizer tk;

public:
    Parser();
    void reset();

    void eat(word);
    void openfile(string name);
    void closefile();
    void peekline();
    word next();
    bool END();

    void indent();
    void printSymbol();
    void printKeyword();
    void printID();

    void parse();
    void Class();
    void classVarDec();
    void varDec();
    void type();
    void subroutineDec();
    void parameterList();
    void subroutineBody();
    void className();
    void subroutineName();
    void varName();
    void statements();
    void statement();
    void letStatement();
    void ifStatement();
    void whileStatement();
    void doStatement();
    void returnStatement();
    void expression();
    void term();
    void subroutineCall();
    void expressionList();
    void op();
    void unaryOp();
    void keywordConstant();
};
