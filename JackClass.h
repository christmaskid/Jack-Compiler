#include "Subroutine.h"

class JackClass
{
private:
    string name;
    Variable* varList;
    Subroutine* subList;
    int varCount,st,fi;
    int subCount;
    bool isArray;
public:
    JackClass();
    ~JackClass();
    void regist(string);
    void addNewVar(word,string,string);
    void registerSub(word,string,string);
    void registerSub(word,string,string,int);

    void showList();

    friend class CodeGenerator;
};
