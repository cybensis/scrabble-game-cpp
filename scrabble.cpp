
#include "LinkedList.h"

#include <iostream>
#include "GameEngine.h"
#include <string>
#include <fstream>
//#include <limits>
//#include <typeinfo>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::fstream;
string userInput();
using std::cin;
using std::cout;
using std::endl;


#define EXIT_SUCCESS    0

void welcomeMessage();
void mainMenu();
void newGame();
void loadGame();
void credits();
std::fstream& GotoLine(std::fstream& file, unsigned int num);
string userInput();

int main(void) {
//   LinkedList* list = new LinkedList();
//   delete list;
    welcomeMessage();
    mainMenu();
    return EXIT_SUCCESS;
}

void welcomeMessage() {
    cout << "Welcome to Scrabble!" << endl;
    cout << "-------------------"<< endl;
}

void mainMenu() {
    cout << endl << "Menu" << endl;
    cout << "----" << endl;
    cout << "1. New Game" << endl;
    cout << "2. Load Game" << endl;
    cout << "3. Credits (Show student information)" << endl;
    cout << "4. Quit" << endl << endl;

    bool toRePrompt;
    string choice;
    do {
        toRePrompt = false;
        choice = userInput();
        if (choice == "1") {
            newGame();
        } else if (choice == "2") {
            loadGame();
        } else if (choice == "3") {
            credits();
        } else if (choice == "4") {
            cout << "\nGoodbye" << endl;
            exit(EXIT_SUCCESS);
        } else {
            cout << "Invalid Input" << endl;
            toRePrompt = true;
        }
    } while (toRePrompt);
}

void newGame() {
    cout << endl << endl << "Starting a New Game" << endl;
    GameEngine* gameInstance = new GameEngine();
    gameInstance->gameController();
    std::cout << std::endl << "Goodbye" << std::endl;
    delete gameInstance;
    return;
}

void loadGame() {
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

void credits() {
    std::ifstream f("authors.txt");
    cout << endl << "----------------------------------" << endl;
    if (f.is_open())
        cout << f.rdbuf();
    cout << endl << "----------------------------------" << endl;
    mainMenu();
}

//ref = https://stackoverflow.com/questions/5207550/in-c-is-there-a-way-to-go-to-a-specific-line-in-a-text-file
std::fstream& GotoLine(std::fstream& file, unsigned int num){
    file.seekg(std::ios::beg);
    for(unsigned int i=0; i < num - 1; ++i){
        file.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    return file;
}


string userInput() {
    string input;
    cout << "> ";
    getline(cin, input);
    if (cin.eof() || input == "^D") {
        cout << ((input == "^D") ? "\nGoodbye" : "\n\nGoodbye") << endl;
        exit(EXIT_SUCCESS);
    }
    return input;
}
