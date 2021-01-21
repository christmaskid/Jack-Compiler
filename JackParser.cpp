/// Dec. 22~28, 2020
#include <iostream>
#include <sstream>
#include <string>
#include "JackParser.h"
using namespace std;

Parser::Parser()
{
}
void Parser::reset()
{
    ptr=0;
    depth=0;
    tk.reset();
}
void Parser::openfile(string name)
{
    tk.openfile(name);
    fout.open(name+".xml",ios::out);
}
void Parser::closefile(){
    tk.closefile();
    fout.close();
}
word Parser::next(){
    return strToWord(tk.next());
}
void Parser::eat(word w){
    s=tk.next();
    tk.eat();
}

void Parser::indent()
{
    for(int i=0;i<depth;i++) fout<<" ";
}
void Parser::parse()
{
    fout<<"<token>"<<endl; depth++;
    Class();
    fout<<"</token>"<<endl; depth--;
}
void Parser::printSymbol(){indent(); fout<<"<symbol> "<<s<<" </symbol>"<<endl;}
void Parser::printKeyword(){indent(); fout<<"<keyword> "<<s<<" </keyword>"<<endl;}
void Parser::printID(){indent(); fout<<"<identifier> "<<s<<" </identifier>"<<endl;}
void Parser::Class()
{
    /* 'class' className '{' classVarDec* subroutineDec* '}' */
    eat(CLASS); printKeyword(); indent(); fout<<"<class>"<<endl; depth++;
    className();
    eat(LLP); printSymbol();
    classVarDec();
    subroutineDec();
    eat(RLP); printSymbol();
    depth--; indent(); fout<<"</class>"<<endl;
}
void Parser::classVarDec() /// variable declaration
{
    if(next()==STATIC || next()==FIELD){
        indent(); fout<<"<classVarDec>"<<endl; depth++;
        while(next()==STATIC || next()==FIELD)
        {
            /* ('static'|'field')? type varName (',' varName)* ';' */
            switch(next())
            {
                case STATIC:
                    eat(STATIC); printKeyword(); break;
                case FIELD:
                    eat(FIELD);  printKeyword(); break;
                default: break;
            }
            type();
            varName();
            while(next()==COMMA)
            {
                eat(COMMA); printSymbol();
                varName();
            }
            eat(SEMI);
        }
        depth--; indent(); fout<<"</classVarDec>"<<endl;
    }
}
void Parser::varDec()
{
    if(next()==VAR){
    indent(); fout<<"<varDec>"<<endl; depth++;
    while(next()==VAR)
    {
        /* var type varName (',' varName)* ';' */
        eat(VAR); printKeyword();
        type();
        varName();
        while(next()==COMMA)
        {
            eat(COMMA); printSymbol();
            varName();
        }
        eat(SEMI);
    }
    depth--; indent(); fout<<"</varDec>"<<endl;
    }
}
void Parser::type()
{
    /* 'int'|'char'|'boolean'|className */
    switch(next())
    {
        case INT: eat(INT); printKeyword(); break;
        case CHAR: eat(CHAR); printKeyword(); break;
        case BOOL: eat(BOOL); printKeyword(); break;
        case ID: className(); break;
        default: break;
    }
}
void Parser::subroutineDec()
{
    indent(); fout<<"<subroutineDec>"<<endl; depth++;
    /* ('constructor'|'function'|'method') ('void'|type) subroutineName '(' parameterList ')' subroutineBody */
    word kind=fail;
    while(next()==CONSTR || next()==FUNC || next()==METH){
        switch(next())
        {
            case CONSTR: eat(CONSTR); indent(); fout<<"<constructor> "<<endl; kind=CONSTR; break;
            case FUNC: eat(FUNC); indent(); fout<<"<function>"<<endl; kind=FUNC; break;
            case METH: eat(METH); indent(); fout<<"<method>"<<endl; kind=METH; break;
            default: cout<<"fail"<<endl; break;
        }
        depth++;
        printKeyword();
        switch(next())
        {
            case VOID: eat(VOID); printKeyword(); break;
            default: type();
        }
        subroutineName();
        eat(LSP); printSymbol();
        parameterList();
        eat(RSP); printSymbol();
        subroutineBody();
        depth--;
        switch(kind)
        {
            case CONSTR: indent(); fout<<"</constructor>"<<endl; break;
            case FUNC: indent(); fout<<"</function>"<<endl; break;
            case METH: indent(); fout<<"</method>"<<endl; break;
            default: break;
        }
    }
    depth--; indent(); fout<<"</subroutineDec>"<<endl;
}
void Parser::parameterList()
{
    /* ((type varName)(',' type varName))? */
    if(next()!=RSP)  // ( ')'
    {
        indent(); fout<<"<parameterList>"<<endl; depth++;
        type();
        varName();
        while(next()==COMMA)
        {
            eat(COMMA); printSymbol();
            type(); varName();
        }
        depth--; indent(); fout<<"</parameterList>"<<endl;
    }
}
void Parser::subroutineBody()
{
    indent(); fout<<"<subroutineBody>"<<endl; depth++;
    /* '{' varDec* statements '}' */
    eat(LLP); printSymbol();
    while(next()==VAR) varDec();
    statements();
    eat(RLP); printSymbol();
    depth--; indent(); fout<<"</subroutineBody>"<<endl;
}
void Parser::className(){eat(ID); printID();}
void Parser::subroutineName(){eat(ID); printID();}
void Parser::varName(){eat(ID); printID();}
void Parser::statements()
{
    indent(); fout<<"<statements>"<<endl; depth++;
    while(next()!=RLP) statement();
    depth--; indent(); fout<<"</statements>"<<endl;
}
void Parser::statement()
{
    depth++;
    switch(next())
    {
        case LET: letStatement(); break;
        case IF: ifStatement(); break;
        case WHILE: whileStatement(); break;
        case DO: doStatement(); break;
        case RETURN: returnStatement(); break;
        default: break;
    }
    depth--;
}
void Parser::letStatement()
{
    indent(); fout<<"<letStatement>"<<endl; depth++;
    eat(LET); printKeyword();
    varName();
    if(next()==LMP)
    {
        eat(LMP); printSymbol();
        expression();
        eat(RMP); printSymbol();
    }
    eat(EQ); printSymbol();
    expression();
    eat(SEMI); printSymbol();
    depth--; indent(); fout<<"</letStatement>"<<endl;
}
void Parser::ifStatement()
{
    indent(); fout<<"<ifStatement>"<<endl; depth++;
    eat(IF); printKeyword();
    eat(LSP); printSymbol();
    expression();
    eat(RSP); printSymbol();
    eat(LLP); printSymbol();
    statements();
    eat(RLP); printSymbol();
    if(next()==ELSE)
    {
        eat(ELSE); printKeyword();
        eat(LLP); printSymbol();
        depth++; statements(); depth--;
        eat(RLP); printSymbol();
    }
    depth--; indent(); fout<<"</ifStatement>"<<endl;
}
void Parser::whileStatement()
{
    indent(); fout<<"<whileStatement>"<<endl; depth++;
    eat(WHILE); printKeyword();
    eat(LSP); printSymbol();
    expression();
    eat(RSP); printSymbol();
    eat(LLP); printSymbol();
    depth++; statements(); depth--;
    eat(RLP); printSymbol();
    depth--; indent(); fout<<"</whileStatement>"<<endl;
}
void Parser::doStatement()
{
    indent(); fout<<"<doStatement>"<<endl; depth++;
    eat(DO); printKeyword();
    eat(ID); printID(); subroutineCall();
    eat(SEMI); printSymbol();
    depth--; indent(); fout<<"</doStatement>"<<endl;
}
void Parser::returnStatement()
{
    indent(); fout<<"<returnStatement>"<<endl; depth++;
    eat(RETURN); printKeyword();
    if(next()!=SEMI) expression();
    eat(SEMI); printSymbol();
    depth--; indent(); fout<<"</returnStatement>"<<endl;
}
void Parser::expression()
{
    indent(); fout<<"<expression>"<<endl; depth++;
    term();
    while(next()!=SEMI && next()!=RSP && next()!=RMP) {op(); term();}
    depth--; indent(); fout<<"</expression>"<<endl;
}
void Parser::term()
{
    switch(next())
    {
        case intConst: eat(intConst); indent(); fout<<"<int.Const.> "<<s<<" </int.Const.>"<<endl; break;
        case StrConst: eat(StrConst); indent(); fout<<"<str.Const.> "<<s<<" </strConst.>"<<endl; break;
        case ID:
            varName();
            if(next()==LMP)
            {
                eat(LMP); printSymbol();
                expression();
                eat(RMP); printSymbol();
            }
            else if(next()==PER || next()==LSP)
                subroutineCall();
            break;
        case LSP:
            eat(LSP); printSymbol();
            expression();
            eat(RSP); printSymbol();
            break;
        case NEG:
        case SUB:
            unaryOp();
            term();
            break;
        case TRUE:
        case FALSE:
        case NUL:
        case THIS:
            keywordConstant(); break;
        default: break;
    }
}
void Parser::subroutineCall()
{
    //eat(ID); printID(); // subroutineName() | className() | varName()
    if(next()==LSP) // subroutineName()
    {
        eat(LSP); printSymbol();
        expressionList();
        eat(RSP); printSymbol();
    }
    else // className() | varName()
    {
        eat(PER); printSymbol();
        subroutineName();
        eat(LSP); printSymbol();
        expressionList();
        eat(RSP); printSymbol();
    }
}
void Parser::expressionList()
{
    indent(); fout<<"<expressionList>"<<endl; depth++;
    if(next()!=RSP)
    {
        expression();
        while(next()==COMMA)
        {
            eat(COMMA); printSymbol();
            expression();
        }
    }
    depth--; indent(); fout<<"</expressionList>"<<endl;
}
void Parser::op()
{
    eat(ADD); // | SUB | MUL | DIV | AND | OR | LT | GT | EQ
    printSymbol();
}
void Parser::unaryOp()
{
    eat(NEG); // | SUB
    printSymbol();
}
void Parser::keywordConstant()
{
    eat(TRUE); // | FALSE | NUL | THIS
    printKeyword();
}
