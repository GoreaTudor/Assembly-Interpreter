#include <iostream>
#include <cstring>
using namespace std;


static char *reg;
static char **code;

FILE* SourceCodeFile;
//FILE* CustomInputs;

char A;
unsigned CurrentIndex;

bool CarryFlag;
unsigned CarryLine;


int main(int argc, char* argv[]){
    reg = new char[32];
    code = new char*[50];
    char* s = new char[100];
    size_t len = 0;

    SourceCodeFile = fopen("AsmTest.txt", "r");

    if(SourceCodeFile){
        while(fgets(s, 100, SourceCodeFile)){
            if(s[0] != '\n'){
                cout << len << ": " << s;
                len++;
            }
        }
        cout << endl << "len = " << len;

        fclose(SourceCodeFile);
    }
    else {
        cout << "Error!";
    }
    
}