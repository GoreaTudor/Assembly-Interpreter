#include <iostream>
#include <cstring>
#include <conio.h>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////// CLASSES



//Simple linked list Node used for Dictionary
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

//Maps a string to a value, this class will be used for Jumpers
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



/////////////////////////////////////////////////////////////////////////////////////////////////// VARIABLES



Dictionary* dict; //The dictionary object pointer
FILE* SourceCodeFile; //The file containing the MyASM code

char ***code; //Code (Word Matrix)
size_t numberOfLines;
size_t wordsPerLine = 2;
size_t CurrentIndex; //The index of the MyASM code representing the line

char *reg; //Registers
size_t numberOfRegisters = 64;

bool debugMode; //True - Debug Mode, False - Normal Mode

char A; //Accumulator

bool CarryFlag; //The carry flag
size_t CarryLine; //The number of the line that set the carry flag



/////////////////////////////////////////////////////////////////////////////////////////////////// METHOD PROTOTYPES



// Converts string to number
int Str_Nr(char* s);

// Checks if the line is NOT an empty line or a comment line
bool lineIsValid(char* lineToValidate);

// Reads the MyASM code from the file and saves it into the word matrix
bool Read(char* filename);

// Displays the values of the registers
void Display();

//Finds the digits from the string and forms a number with the digits in order
//Ex 1: "1234" --> 1234
//Ex 2: "1f2g3h6" --> 1236
//Ex 3: "%256" --> 256
int StringToNumber(char* s);

//Checks if the operand is valid (the second word)
bool Valid(char* s);

// Runs the code in Normal Mode / Debug Mode
bool Run();

// The ending of the program
void Epilogue();



/////////////////////////////////////////////////////////////////////////////////////////////////// MAIN METHOD



int main(int argc, char* argv[]){
    char option;
    char* filename;

    cout << "Name of the file: ";
    cin >> filename;
    
    if(Read(filename)){
        //cout << "Read.\n";

        reg = new char[numberOfRegisters]; //32 bytes allocated for the registers
        for(int i=0; i<numberOfRegisters; i++) //Initializing the register values
            reg[i] = 0;

        //Verify if the code is properly saved
        // for(int i=0; i < numberOfLines; i++){
        //     for(int j=0; j<wordsPerLine; j++)
        //         cout << code[i][j] << "    ";
        //     cout << endl;
        // }

        cout << "\nDo you want to Run Debug Mode ? (y/n)";
        cout << "\nOption: ";
        option = getche();

        cout << "\n";
        if(option == 'y')
            debugMode = true;
        else if(option == 'n')
            debugMode = false;
        else{
            cout << "Invalid key.";
            return -1;
        }

        if(Run())
            cout << "\nCode ran with no errors, Hoorah!";
        else
            cout << "\nError at line: " << CurrentIndex;

        Epilogue();
    }
    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////// METHODS



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


bool Valid(char* s){
    if(s == NULL) //An empty operand is not valid
        return false;


    // Syntax
    if(strcmp(s, "A") == 0) //A is the accumulator
        return true;

    if(s[0] != '%' and s[0] != '&' and (s[0] < '0' or s[0] > '9')) //the first caracter should be '%' or '&' or a digit
        return false;
    
    for(int i=1; i<strlen(s); i++){ //all the other characters should be digits
        if(s[i] < '0' or s[i] > '9')
            return false;
    }


    // Logic
    int operand = StringToNumber(s);
    
    if(s[0] == '&' and (operand >= numberOfRegisters or reg[operand] >= numberOfRegisters)) // pointer that exits the allocated memory
        return false;

    if(s[0] == '%' and operand >= numberOfRegisters) // variable that exits the allocated memory
        return false;


    // The operand is valid
    return true;
}


bool lineIsValid(char* lineToValidate){
    if(lineToValidate[0] == '\n') //Empty line
        return false;

    if(lineToValidate[0] == '#') //Comment line
        return false;

    return true;
}


bool Read(char* filename){
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
    // cout << "\n\n";
    // for(line = 0; line < numberOfLines; line ++){
    //     cout << line << ": " << code[line][0] << "   " << code[line][1] << endl;
    // }


    fclose(SourceCodeFile);
    return true;
}


void Display(){
    cout << "\n";
    for(int i=0; i<numberOfRegisters; i++){
        printf("%.2x ", reg[i]);

        if(i % 5 == 4)
            cout << "\n";
    }
    cout << "\nAccumulator Value: " << (int)A << "\n";
}


bool Run(){
    cout << "Running...";
    CurrentIndex = 0;
    int operand;

    while(true){
        if(debugMode)
            cout << "\n\nCurrent command: " << code[CurrentIndex][0] << " " << code[CurrentIndex][1];

        if(!Valid(code[CurrentIndex][1])){ //The operand is not valid
            cout << "\nSyntax Error: " << code[CurrentIndex][1] << " is not a valid operand";
            return false;
        }

        operand = StringToNumber(code[CurrentIndex][1]); // making the operand easier to use
        //cout << "\nOperand: " << operand;

/////////////////////////////////////////////////////////////////////////////// ARITHMETIC OPERATORS
        if( strcmp(code[CurrentIndex][0], "ADD") == 0 ){           // ADD
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A += reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A += reg[ operand ];

            } else { //numeric value
                A += operand;
            }

            CurrentIndex ++;

        } else if( strcmp(code[CurrentIndex][0], "SUB") == 0 ){    // SUB
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A -= reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A -= reg[ operand ];

            } else { //numeric value
                A -= operand;
            };

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "INC") == 0 ){    // INC
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A = ++ reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A = ++ reg[ operand ];

            } else if( strcmp(code[CurrentIndex][1], "A") == 0 ){ //Accumulator
                A ++;
            }

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "DEC") == 0 ){    // DEC
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A = -- reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A = -- reg[ operand ];

            } else if( strcmp(code[CurrentIndex][1], "A") == 0 ){ //Accumulator
                A --;
            }

            CurrentIndex ++;
        }
            
/////////////////////////////////////////////////////////////////////////////// LOGIC OPERATORS
        else if( strcmp(code[CurrentIndex][0], "AND") == 0 ){      // AND
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A &= reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A &= reg[ operand ];

            } else { //numeric value
                A &= operand;
            };

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "OR") == 0 ){     // OR
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A |= reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A |= reg[ operand ];

            } else { //numeric value
                A |= operand;
            };

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "NOT") == 0 ){    // NOT
            /*NOT A is the equivalent of -A -1, 
            because -A = NOT A + 1  
            (2's complement rule)*/

            if( strcmp(code[CurrentIndex][1], "A") == 0 ){ //Accumulator
                A = -A -1;

            }else if(code[CurrentIndex][1][0] == '&'){ //pointer
                A = -reg[ reg[ operand ] ] -1;

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A = -reg[ operand ] -1;

            } else { //numeric value
                A = -operand -1;
            }

            CurrentIndex ++;
        }

/////////////////////////////////////////////////////////////////////////////// BIT OPERATORS
        else if( strcmp(code[CurrentIndex][0], "SHL") == 0 ){      // SHL
            if( strcmp(code[CurrentIndex][1], "A") == 0 ){ //Accumulator
                A = A << 1;

            }else if(code[CurrentIndex][1][0] == '&'){ //pointer
                A = (reg[ reg[ operand ] ]) << 1;

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A = (reg[ operand ]) << 1;

            };

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "SHR") == 0 ){    // SHR
            if( strcmp(code[CurrentIndex][1], "A") == 0 ){ //Accumulator
                A = A >> 1;

            }else if(code[CurrentIndex][1][0] == '&'){ //pointer
                A = (reg[ reg[ operand ] ]) >> 1;

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A = (reg[ operand ]) >> 1;

            };

            CurrentIndex ++;
        }
            
/////////////////////////////////////////////////////////////////////////////// JUMPERS
        else if( strcmp(code[CurrentIndex][0], "JMP") == 0 ){      // JMP
            ;

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "JZ") == 0 ){     // JZ
            ;

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "JNZ") == 0 ){    // JNZ
            ;

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "JLZ") == 0 ){    // JLZ
            ;

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "JMZ") == 0 ){    // JMZ
            ;

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "JC") == 0 ){     // JC
            ;

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "JNC") == 0 ){    // JNC
            ;

            CurrentIndex ++;
        }

/////////////////////////////////////////////////////////////////////////////// MEMORY OPERATORS    
        else if( strcmp(code[CurrentIndex][0], "STORE") == 0 ){    // STORE
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                reg[ reg[ operand ] ] = A;

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                reg[ operand ] = A;

            }

            CurrentIndex ++;
            
        } else if( strcmp(code[CurrentIndex][0], "LOAD") == 0 ){   // LOAD
            if(code[CurrentIndex][1][0] == '&'){ //pointer
                A = reg[ reg[ operand ] ];

            } else if(code[CurrentIndex][1][0] == '%'){ //variable
                A = reg[ operand ];

            } else { //numeric value
                A = operand;
            };

            CurrentIndex ++;
        }

/////////////////////////////////////////////////////////////////////////////// HALTING & ERRORS
        else if( strcmp(code[CurrentIndex][0], "HLT") == 0 ){      // HLT
            Display();
            return true;
        }

        else{ //Not in the language Error
            cout << "\nSyntax Error: " << code[CurrentIndex][0] << " is not in the language";
            return false;
        }

///////////////////////////////////////////////////////////////////////////////////////////////////

        if(debugMode){ //Display the value of registers at every step in Debug Mode
            Display();
            getch();
        }
        
        
        if(CurrentIndex >= numberOfLines){ //if it reached the end
            Display(); //Display the values of registers at the end of execution
            return true;
        }
    }
}


void Epilogue(){
    delete[] reg;

    for(int line=0; line<numberOfLines; line ++){
        delete[] code[line][0];
        delete[] code[line][1];

        delete[] code[line];
    }
    delete[] code;
}

///// THE END /////