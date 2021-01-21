#include "Word.h"

word strToWord(string s)
{
    /// symbol
    if(s=="{") return LLP;
    if(s=="}") return RLP;
    if(s=="[") return LMP;
    if(s=="]") return RMP;
    if(s=="(") return LSP;
    if(s==")") return RSP;
    if(s==".") return PER;
    if(s==",") return COMMA;
    if(s==";") return SEMI;
    if(s=="+") return ADD;
    if(s=="-") return SUB;
    if(s=="*") return MUL;
    if(s=="/") return DIV;
    if(s=="&") return AND;
    if(s=="|") return OR;
    if(s=="<") return LT;
    if(s==">") return GT;
    if(s=="=") return EQ;
    if(s=="~") return NEG;

    /// keyword
    if(s=="class") return CLASS;
    if(s=="constructor") return CONSTR;
    if(s=="function") return FUNC;
    if(s=="method") return METH;
    if(s=="field") return FIELD;
    if(s=="static") return STATIC;
    if(s=="var") return VAR;
    if(s=="int") return INT;
    if(s=="char") return CHAR;
    if(s=="boolean") return BOOL;
    if(s=="void") return VOID;
    if(s=="true") return TRUE;
    if(s=="false") return FALSE;
    if(s=="null") return NUL;
    if(s=="this") return THIS;
    if(s=="let") return LET;
    if(s=="do") return DO;
    if(s=="if") return IF;
    if(s=="else") return ELSE;
    if(s=="while") return WHILE;
    if(s=="return") return RETURN;

    int len=s.length();
    /// StrConst
    if(s[0]=='"' && s[len-1]=='"') return StrConst;
    /// intConst
    if(s[0]>='0' && s[0]<='9')
    {
        bool f=true;
        for(int i=0;i<len;i++)
            if(s[i]<'0' || s[i]>'9') {f=false; break;}
        if(f) return intConst;
    }
    if(s[0]<'0' || s[0]>'9') return ID;
    return fail;

}
/// wordToString is added on Dec. 31 for the use in CodeGenerator
string wordToStr(word w)
{
    if(w==LLP) return "{";
    if(w==RLP) return "}";
    if(w==LMP) return "[";
    if(w==RMP) return "]";
    if(w==LSP) return "(";
    if(w==RSP) return ")";
    if(w==PER) return ".";
    if(w==COMMA) return ",";
    if(w==SEMI) return ";";
    if(w==ADD) return "+";
    if(w==SUB) return "-";
    if(w==MUL) return "*";
    if(w==DIV) return "/";
    if(w==AND) return "&";
    if(w==OR) return "|";
    if(w==LT) return "<";
    if(w==GT) return ">";
    if(w==EQ) return "=";
    if(w==NEG) return "~";
    if(w==CLASS) return "class";
    if(w==CONSTR) return "constructor";
    if(w==FUNC) return "function";
    if(w==METH) return "method";
    if(w==FIELD) return "this";
    if(w==STATIC) return "static";
    if(w==VAR) return "var";
    if(w==LCL) return "local";
    if(w==ARG) return "argument";
    if(w==INT) return "int";
    if(w==CHAR) return "char";
    if(w==BOOL) return "boolean";
    if(w==VOID) return "void";
    if(w==TRUE) return "true";
    if(w==FALSE) return "false";
    if(w==NUL) return "null";
    if(w==THIS) return "this";
    if(w==LET) return "let";
    if(w==DO) return "do";
    if(w==IF) return "if";
    if(w==ELSE) return "else";
    if(w==WHILE) return "while";
    if(w==RETURN) return "return";
    return "";
}
