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
        this->PK = PK;
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

int main(){
    ///// TESTING NODE CLASS /////

    // Node* n1 = new Node("Adina", 19);
    // n1->display();

    // Node* n2 = new Node("Tudor", 21);
    // n2->display();

    // n1->next = n2;

    // n1->display();
    // n1->next->display();



    ///// TESTING DICTIONARY CLASS /////

    Dictionary* dict = new Dictionary();
    dict->display();

    dict->addToDictionary("Adina", 19);
    dict->addToDictionary("Tudor", 21);
    dict->addToDictionary("Ionut", 17);
    dict->addToDictionary("Adina", 32); //doesn't get added because the key "Adina" already exists
    dict->addToDictionary("Noje", 26);
    dict->addToDictionary("Haba", 20);
    dict->addToDictionary("Ionut", 45);//doesn't get added because the key "Ionut" already exists

    dict->display();
    cout << endl;

    cout << dict->getNodeByPK("Adina")->value << endl;
    cout << dict->getNodeByPK("Ionut")->value << endl;
    cout << dict->getNodeByPK("Tudor")->value << endl;
    //cout << dict->getNodeByPK("abcd")->value << endl; //Segmentation fault: the key "abcd" doesn't exist in the Dictionary, NULL is returned

    return 0;
}