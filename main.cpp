// Author: Cheng, Yu-Tong
// Date: Dec. 22~28, 2020
#include <iostream>
#include <fstream>
#include <string>
#include "CodeGenerator.h"
#include <io.h> /// for directory management
using namespace std;
int main()
{
    string path;
    while(getline(cin,path)){
        Tokenizer tokenizer;
        Parser parser;
        CodeGenerator cg;

        if(path=="quit") break;
        string fileName;
        fstream fin;
        fin.open(path,ios::in);

        /// directory
        if(!fin){
            long hFile=0;
            struct _finddata_t fileInfo;
            string pathName,exdName;
            hFile=_findfirst(pathName.assign(path).append("\\*.jack").c_str(),&fileInfo);
            do
            {
                tokenizer.reset();
                parser.reset();
                cg.reset();

                fileName=fileInfo.name;
                fileName=fileName.substr(0,fileName.find("."));
                cout<<fileName<<endl;
                fileName=path+"\\"+fileName;
/*
                tokenizer.openfile(fileName);
                tokenizer.tokenize();
                tokenizer.closefile();
                cout<<"Complete tokenizing "<<fileName<<" successfully."<<endl;
*/
                parser.openfile(fileName);
                parser.parse();
                parser.closefile();
                cout<<"Complete parsing "<<fileName<<" successfully."<<endl;

                cg.openfile(fileName);
                cg.generateCode();
                cg.closefile();
                cout<<"Complete compiling "<<fileName<<" successfully."<<endl;
            }while(_findnext(hFile,&fileInfo)==0);
            _findclose(hFile);
        }


        /// single file
        else{
            parser.reset();
            cg.reset();
            fileName=path.substr(0,path.length()-5);
/*
            tokenizer.openfile(fileName);
            tokenizer.tokenize();
            tokenizer.closefile();
            cout<<"Complete tokenizing "<<fileName<<" successfully."<<endl;
*/
            parser.openfile(fileName);
            parser.parse();
            parser.closefile();
            cout<<"Complete parsing "<<fileName<<" successfully."<<endl;

            cg.openfile(fileName);
            cg.generateCode();
            cg.closefile();
            cout<<"Complete compiling "<<fileName<<" successfully."<<endl;

        }
        fin.close();
    }
    cout<<"End of the program."<<endl;
    return 0;
}
