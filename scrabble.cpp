
#include "LinkedList.h"

#include <iostream>
#include "GameEngine.h"
#include <string>
#include <fstream>
//#include <limits>
//#include <typeinfo>

using std::cin;
using  std::cout;
using  std::endl;
using std::string;
using std::fstream;
string userInput();
using std::cin;
using std::cout;
using std::endl;


#define EXIT_SUCCESS    0

void MainMenu();
string userInput();
int main(void) {
   LinkedList* list = new LinkedList();
   delete list;
   GameEngine* gameInstance = new GameEngine();
   // Need this so it doesn't have a sook at compile time about gameInstance not being used
   std::cout << typeid(gameInstance).name() << std::endl;
   std::cout << "TODO: Implement Scrabble!" << std::endl;
   MainMenu();
   return EXIT_SUCCESS;
}

void MainMenu() {
   cout << "Welcome to Scrabble!"<<endl;
    cout << "-------------------"<<endl;
    cout << ""<<endl;
    cout << "Menu" << endl;
    cout<< "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl;
    string choice = userInput();
    if (choice == "1") {
        cout << "Starting a New Game" << endl;
        cout << " "<<endl;
        cout << "Enter a name for player 1 (uppercase characters only)" << endl;
        string name;
        cout << "> ", cin>> name;
        cout << "Enter a name for player 2 (uppercase characters only)" << endl;
        string name2;
        cout << "> ", cin>> name2;
        cout << "Let's Play!" << endl;

    }
    if (choice == "2") {
        cout << "Enter the filename from which load a game" << endl;
        string dir = userInput();
        fstream myFile;
        myFile.open(dir);
        if (myFile) {
            /*
            string line;
            if (typeid(GotoLine(myFile, 1)).name() == "Ss" && typeid(GotoLine(myFile, 2)).name() == "i"
            && typeid(GotoLine(myFile, 3)).name() == "Ss" && typeid(GotoLine(myFile, 1)).name() == "Ss") {
            }
            */
            cout << "" << endl;
            cout << "Scrabble game successfully loaded" << endl;
            
        }
        else {
            cout << "" << endl;
            cout << "The file does not exist!" << endl;

        }

    }
    if (choice == "3") {
        cout << "" << endl;
        cout << "----------------------------------" << endl;
        cout << "Name: Arian Eskandarinejad" << endl;
        cout << "Strudent ID: s3884975" << endl;
        cout << "Email: s3884975@student.rmit.edu.au" << endl;
        cout << "" << endl;
        cout << "Name: Jeremy Ng Kwik Tung" << endl;
        cout << "Strudent ID: s3842858" << endl;
        cout << "Email: s3842858@student.rmit.edu.au" << endl;
        cout << "" << endl;
        cout << "Name: Guy Seccull" << endl;
        cout << "Strudent ID: s3785085" << endl;
        cout << "Email: s3785085@student.rmit.edu.au" << endl;
        cout << "" << endl;
        cout << "Name: Christopher Truong" << endl;
        cout << "Strudent ID: s3848927" << endl;
        cout << "Email: s3848927@student.rmit.edu.au" << endl;
        cout << "----------------------------------" << endl;
        
    }
    if (choice == "4") { 
        cout << "Goodbye" << endl;
        exit(4);
    }

}

string userInput() {
    string input;
    cout << "> ";
    cin >> input;
    if (cin.eof() || input == "^D") {
        cout << "\nGoodbye" << endl;
    } else {

    }
    return input;

}
