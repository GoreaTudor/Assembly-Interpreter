#include <iostream>
#include <cstring>
using namespace std;


int StringToNumber(char* s){
    int i, nr = 0;
    char ch;

    for(i = 0; i < strlen(s); i++){
        ch = s[i];

        if(ch < '0' or ch > '9')
            continue;
        
        nr = nr * 10 + (ch - '0');
    }

    return nr;
}


int main(int argc, char* argv[]){
    cout << StringToNumber("%12314");
    
    return 0;
}