#include <iostream>
#include "Word.h"
using namespace std;

class Variable
{
private:
    word kind;
    string type;
    string name;
    int num;
public:
    Variable();
    void setData(word,string,string,int);

    friend class Subroutine;
    friend class JackClass;
    friend class CodeGenerator;
};
