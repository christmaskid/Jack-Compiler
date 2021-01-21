#include <iostream>
#include <fstream>
#include <queue>
#include "JackClass.h"
using namespace std;

bool isSymbol(char);
int quotaCount(string);

class Tokenizer
{
private:
    fstream fin;
    string in;
    bool comment;
    queue<string> stk;
    int line;

public:
    Tokenizer();
    void reset();
    void openfile(string name);
    void closefile();
    bool END();
    void tokenize();
    void process(string);
    string next();
    void eat();
    bool isEmpty();

    friend class CodeGenerator;
    friend class JackParser;
};
