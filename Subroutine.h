#include <iostream>
#include "Variable.h"
using namespace std;

class Subroutine
{
private:
    word kind;
    string type;
    string name;
    Variable* varList;
    int varCount,loc,arg;
public:
    Subroutine();
    ~Subroutine();
    void setData(word,string,string);
    void setData(word,string,string,int);
    void addNewVar(word,string,string);

    void showList();

    friend class JackClass;
    friend class CodeGenerator;
};
