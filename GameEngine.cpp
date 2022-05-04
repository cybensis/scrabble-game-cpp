#include "GameEngine.h"

GameEngine::GameEngine()
{
    this->instanceData = new Session();
    // Need this so it doesn't have a sook at compile time about instanceData not being used
    std::cout << typeid(instanceData).name() << std::endl;
    std::cout << "Let's Play!" << std::endl << std::endl;
    gameController();

    // NOTES: I'm pretty sure when entering "place __ at __", these commands get queued, rather
    // than executed straight away, then once "place Done" is ran, they all get executed.
    // That seems a shit way to do it tho, so maybe place tiles and add score as you go,
    // then once they type "place Done", add all the tiles back to their hand.
}

GameEngine::~GameEngine()
{
    delete this->instanceData;
}

void GameEngine::gameController() {
    // Create while loop here probably, while (GAME_NOT_FINISHED) {...}

    // The whole currentPlayer, otherPlayer can definitely be refactored, but I'm just focusing
    // on getting the game going for now.
    Player *currentPlayer = this->instanceData->getCurrentPlayer();
    Player *otherPlayer = this->instanceData->getOtherPlayer();
    std::cout << currentPlayer->getName() << ", it's your turn" << std::endl;
    std::cout << "Score for " << currentPlayer->getName() << ": " << currentPlayer->getScore() << std::endl;
    std::cout << "Score for " << otherPlayer->getName() << ": " << otherPlayer->getScore() << std::endl;
    printBoard();
    std::cout << "Your hand is" << std::endl;
    this->instanceData->printPlayersHand(currentPlayer);
    handlePlayerTurn();

}



void GameEngine::handlePlayerTurn() {
    for (int i = 0; i < MOVES_PER_TURN; i++) {
        std::cout << "> ";
        std::string userInput;
        // getline is needed because just using std::cin >> userInput will ignore everything after whitespaces
        std::getline(std::cin, userInput);
        // NOTE: Probably change userInput so it passes an address not a copy
        validateInput(userInput);
    }
}


// Idea taken from https://java2blog.com/split-string-space-cpp/
void GameEngine::validateInput(std::string input) {
    std::stringstream inputStream(input); 
    std::vector<std::string> splitInput; 
    std::string tempString;
    // This while loop will take the input thats been read into a string stream, then it will read through the stream
    // until it finds a space, then all the characters up until that space are placed into tempString, which is then
    // pushed onto the splitInput vector.
    while (std::getline(inputStream, tempString, SPACE_DELIMITER)) { 
        // We need to allow for any amount of spaces to be entered, so this checks that once the delimiter picks
        // up a space, that it actually has a string and its not just an empty space.
        if (tempString.length() > 0) {
            splitInput.push_back(tempString); 
        }
    }
    if (splitInput.size() > 4) {
        // do nothing or throw error or whatever, no input combo should have more than 4 "chunks" after being split
    }
    else if (splitInput.at(0) == "place") {
        // Verify the rest here
        std::cout << "You're trying to place something" << std::endl;
    }
    else if (splitInput.at(0) == "replace") {
        // Verify replace input here
        std::cout << "You're trying to replace something" << std::endl;
    }
    else if (splitInput.at(0) == "pass") {
        // just pass straight to the next player
        std::cout << "You're trying to pass" << std::endl;
    }
    else {
        //this should be an error im pretty sure
        std::cout << "Try again buddy" << std::endl;
    }

}



void GameEngine::printBoard()
{
    BoardVector *board = this->instanceData->getBoard();
    // Start by printing out the Column coordinates, starting with 4 spaces for the row coordinates,
    // then start printing out " 1  ", " 2  ", etc. Note that the second space after each number is
    // needed because of the "|" in between each coordinate, but if the number is double digits,
    // it has the 2nd space stripped off
    std::cout << "    ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::string colHeader = " " + std::to_string(i) + "  ";
        std::cout << colHeader.substr(0, COL_HEADER_LENGTH);
    }
    // Now printing out the ---- under the column numbers, this time we only need 3 spaces because the
    // dashes start when the "|" start, whereas column numbers start after "|"
    std::cout << std::endl << "   ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        // Exactly 4 dashes are needed per column
        std::cout << "----";
    }
    std::cout << std::endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        std::cout << " " << char(i + INT_ASCII_OFFSET) << " |";
        for (int a = 0; a < BOARD_SIZE; a++) {
            std::cout << " " << board->at(i).at(a) << " |";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return;
}