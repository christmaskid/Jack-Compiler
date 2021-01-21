#include <iostream>
using namespace std;

enum word{
    /// keywords: 0~20
    CLASS,CONSTR,FUNC,METH,FIELD,STATIC,VAR,INT,CHAR,BOOL,VOID,TRUE,FALSE,NUL,THIS,LET,DO,IF,ELSE,WHILE,RETURN,
    /// symbols: 21~39
    LLP,RLP,LMP,RMP,LSP,RSP,PER,COMMA,SEMI,ADD,SUB,MUL,DIV,AND,OR,LT,GT,EQ,NEG,
    /// constants: 40~41
    intConst,StrConst,
    /// identifiers: 42
    ID,
    /// errors
    fail,
    /// for compiler use
    KIND,TYPE,LCL,ARG
};
typedef word word;
word strToWord(string);
string wordToStr(word);
