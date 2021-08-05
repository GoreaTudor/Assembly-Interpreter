#include <iostream>
#include <cstring>
using namespace std;


class Node{
public:
    char* PK;
    size_t value;
    Node* next;

    //Constructor
    Node(char* PK, size_t value){
        this->PK = new char[strlen(PK)];
        strcpy( this->PK, PK );
        if(this->PK[ strlen(this->PK) - 1 ] == '\n')
            this->PK[ strlen(this->PK) - 1 ] = '\0';

        this->value = value;
        this->next = NULL;
    }

    //Default constructor
    Node(){
        this->PK = NULL;
        this->value = 0;
        this->next = NULL;
    }

    //Displays the node info
    void display(){
        cout << "\n" << this << ": " << this->PK << " " << this->value << " " << this->next;
    }
};

class Dictionary{
private:
    Node* head;
    Node* last;

public:
    Dictionary(){
        this->head = NULL;
        this->last = NULL;
    }

    bool addToDictionary(char* PK, size_t value){
        if(this->head == NULL){ //The Dictionary is empty
            this->head = new Node(PK, value);
            this->last = this->head;
            return true;

        } else { //There are some elements in the Dictionary
            Node* p;
            for(p = this->head; p != NULL; p = p->next){ //if the key already exists in the Dictionary, the element will not be added
                if(strcmp(PK, p->PK) == 0)
                    return false;
            }

            //if the Primary Key was not found, then add the new node
            Node* q = new Node(PK, value);
            this->last->next = q;
            this->last = q;

            return true;
        }
    }

    Node* getNodeByPK(char* PK){
        if(this->head == NULL) //The Dictionary is empty
            return NULL;

        for(Node* p = this->head; p != NULL; p = p->next) //Return the node that has the Primary Key equivalent to PK
            if(strcmp(PK, p->PK) == 0)
                return p;

        return NULL; //if PK was not found in the list, return NULL
    }

    void display(){
        if(this->head == NULL){
            cout << "\nThe Dictionary is empty.";
            return;
        }

        for(Node* p = this->head; p != NULL; p = p->next){
            p->display();
        }
        
    }
};


FILE* SourceCodeFile = NULL;
Dictionary* dict;

char ***code;
size_t numberOfLines;
size_t wordsPerLine = 2;
size_t CurrentIndex;

char *reg;
size_t numberOfRegisters = 64;

bool debugMode;

char A;

bool CarryFlag;
size_t CarryLine;



void Epilogue(){
    delete[] reg;

    for(int line=0; line<numberOfLines; line ++){
        delete[] code[line][0];
        delete[] code[line][1];

        delete[] code[line];
    }
    delete[] code;
}


bool lineIsValid(char* lineToValidate){
    if(lineToValidate[0] == '\n') //Empty line
        return false;

    if(lineToValidate[0] == '#') //Comment line
        return false;

    return true;
}


bool Read_v2(char* filename){
    cout << "Reading...\n";

    SourceCodeFile = fopen(filename, "r");
    if(!SourceCodeFile){
        cout << "\nThere was an error on opening the file.";
        return false;
    }


    ////////// The FILE is OPENED //////////
    

    char readLine[100];
    size_t line, word;
    char* currentWord;


    ///// Get NUMBER of LINES /////
    while( fgets(readLine, sizeof(readLine), SourceCodeFile) ){
        if(readLine[0] == ':'){
            dict->addToDictionary(readLine + 1, numberOfLines);

        } else if(lineIsValid(readLine))
            numberOfLines ++;
    }
    //cout << "Nr of lines: " << numberOfLines;


    rewind(SourceCodeFile);


    ///// Allocate Memory /////
    code = new char**[numberOfLines];
    for(line = 0; line < numberOfLines; line ++){
        code[line] = new char*[2];

        code[line][0] = new char[7]; //Operation
        code[line][1] = new char[7]; //Operand
    }


    ///// SAVE the CODE /////
    line = 0;
    while( fgets(readLine, sizeof(readLine), SourceCodeFile) ){
        if(lineIsValid(readLine) and readLine[0] != ':'){
            currentWord = strtok(readLine, " "); //Operation

            if(strlen(currentWord) >= 6)
                return false;
            strcpy( code[line][0], currentWord );
            

            currentWord = strtok(NULL, " "); //Operand
            if(currentWord[strlen(currentWord) - 1] == '\n')
                currentWord[strlen(currentWord) - 1] = '\0';

            if(currentWord == NULL or strlen(currentWord) >= 6)
                return false;
            strcpy( code[line][1], currentWord );

            line ++;
        }
    }


    ///// VERIFY the CODE /////
    cout << "\n\n";
    for(line = 0; line < numberOfLines; line ++){
        cout << line << ": " << code[line][0] << "   " << code[line][1] << endl;
    }
    dict->display();


    fclose(SourceCodeFile);
    return true;
}


int main(){
    dict = new Dictionary();

    if(!Read_v2("AsmTest.txt"))
        cout << "\nError";

    ;
    
    Epilogue();
    return 0;
}

