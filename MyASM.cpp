#include <iostream>
#include <cstring>
using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////// VARIABLES


char *reg; //Registers
char ***code; //Code (Word Martix)
size_t numberOfLines;

FILE* SourceCodeFile; //The file containing the MyASM code
//FILE* CustomInputs;

char A; //Accumulator
size_t CurrentIndex; //The index of the assembly code representing the line

bool CarryFlag; //The carry flag
size_t CarryLine; //The number of the line that set the carry flag


/////////////////////////////////////////////////////////////////////////////////////////////////// METHOD NAMES


// Converts string to number
int Str_Nr(char* s);

// Reads the MyASM code from the file and saves it into the word matrix
bool Read(char* filename);

// Verifies the syntax and returns True if it is correct, otherwise returns False
bool Compiler();

// Displays the values of the registers
void Display();

// Runs the program in normal mode, Displays the values of registers at the end
void NormalMode();

// Runs the program in Debug mode, Displays the values of registers at every step
void DebugMode();

// The ending of the program
void Epilogue();


/////////////////////////////////////////////////////////////////////////////////////////////////// MAIN METHOD



int main(int argc, char* argv[]){
    reg = new char[32]; //32 bytes allocated for the registers

    Read(argv[1]);

    Epilogue();
    return 0;
}



/////////////////////////////////////////////////////////////////////////////////////////////////// METHODS


bool Read(char* filename){
    cout << "Reading...\n";
    
    //SourceCodeFile = fopen(filename, "r");
    SourceCodeFile = fopen("AsmTest.txt", "r"); //for test purpose only

    if(SourceCodeFile){ //The source file was opened
        char line[255]; //maximum characters per line
        int i, j; //general purpose iterator

        //Calculating the number of lines
        while(fgets(line, sizeof(line), SourceCodeFile)){
            if(line[0] != '\n' and line[0] != '#'){ //eliminating empty lines and comment lines
                cout << numberOfLines << ": " << line;
                numberOfLines ++;
            }
        }

        //Rewind the file to read it again
        rewind(SourceCodeFile);

        //Allocate memory for the lines
        code = new char**[numberOfLines];


        //Read and save the contents of the file
        i = 0;
        while(fgets(line, sizeof(line), SourceCodeFile)){
            if(line[0] != '\n' and line[0] != '#'){ //eliminating empty lines and comment lines
                char* word;
                char numberOfWords;

                //Allocate memory for words
                code[i] = new char*[3]; //3 words per command

                //Allocate memory for each word
                for(j=0; j<3; j++)
                    code[i][j] = new char[5]; //5 characters per word


                //Save the words in the word matrix
                numberOfWords = 0;
                word = strtok(line, " ,");

                while(numberOfWords < 3 and word and word[0] != '#'){
                    if(word[strlen(word) - 1] == '\n')
                        word[strlen(word) - 1] = '\0';

                    //cout << word << " " << strlen(word) << endl; // for checking the length of each word
                    if(strlen(word) > 5) // a word that has more than 5 characters
                        return false;
                    
                    //Allocate memory for each word
                    // code[i][numberOfWords] = new char[strlen(word)];

                    strcpy(code[i][numberOfWords], word);
                    //cout << "code[" << i << "][" << (int)numberOfWords << "] = " << code[i][numberOfWords] << endl;

                    numberOfWords ++;

                    //go to the next word
                    word = strtok(NULL, " ,");
                }

                i++; //go to the next line
            }
        }

        //Verify
        cout << "\n\n";
        for(i=0; i < numberOfLines; i++){
            for(j=0; j<2; j++)
                cout << code[i][j] << "    ";
            cout << endl;
        }


        fclose(SourceCodeFile); //closing the file 
        return true;

    } else { //The source file wasn't opened
        cout << "\nThere was an error on opening the file.";
        return false;
    }
}



void Epilogue(){
    delete[] reg;

    // for(int i=0; i<numberOfLines; i++){
    //     delete code[i];
    // }
    // delete[] code;
}