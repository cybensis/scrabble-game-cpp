
#include "Session.h"

Session::Session() {
    playerOnesTurn = true;
    // Since you can't declare a 2D vectors size in the header file, it needs to be done here. This loops pushes 15 times,
    // another vector that has 15 char slots in it, all initialised with an empty space.
    for (int i = 0; i < BOARD_SIZE; i++) {
        this->board.push_back(std::vector<char>(BOARD_SIZE, ' '));
    }
    return;
}

Session::Session(std::fstream* loadFile) {
    if (*loadFile) {
        std::string playerName;
        int playerScore; 
        std::string playerHand;
        std::string tempString;
        std::getline(*loadFile, playerName);
        std::getline(*loadFile, tempString);
        playerScore = std::stoi(tempString);
        std::getline(*loadFile, playerHand);
        this->playerOne = new Player(playerName, playerHand, playerScore);

        std::getline(*loadFile, playerName);
        std::getline(*loadFile, tempString);
        playerScore = std::stoi(tempString);
        std::getline(*loadFile, playerHand);
        this->playerTwo = new Player(playerName, playerHand, playerScore);

        std::vector<std::string> board;
        // The board is going to be very difficult to do as you have to do a lot of splitting input, on the "|   |" - Maybe change the save
        // file so it only prints out the chars, so one row might look like "-------READ----" where "-" are spaces, then you don't have to split
        // anything
        // there are 17 lines for the entire board. 
        for (int i = 0; i < 17; i++) {
            std::getline(*loadFile, tempString);
            board.push_back(tempString);
        }
        // It will probably crash until this is finished so just exit here
        exit(1);
    }
}

Session::~Session() {
    delete this->tileBag;
    delete this->playerOne;
    delete this->playerTwo;
}



bool Session::generateTileBag() {
// Need to create the tile bag from ScrabbleTiles.txt first before adding tiles to players hands.
    this->tileBag = new LinkedList();
    bool error = false;
    // Create an ifstream and open the ScrabbleTiles.txt file
    // Check that the file is good, aka that it exists, its readable, etc, then continue
    std::ifstream tilesFile; 
    tilesFile.open("./ScrabbleTiles.txt");
    if (tilesFile.good()) {
        std::string line;
        // Read each line of the file into the line variable.
        while (std::getline(tilesFile, line)) {
            // These two lines get the character via its index in the string, and its score, starting from
            // the start of the score index, to the end of the string (since a score can be two characters long)
            char tileCharacter = line[CHAR_INDEX];
            // Yoinked from https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int, this
            // checks if the substring of line that contains the score is actually an integer. This is preferred
            // over something like isalpha because it works on strings as well not just chars.
            if (line.substr(SCORE_INDEX,line.length()).find_first_not_of( "0123456789" ) != std::string::npos) {
                std::cout << "Please make sure there is a valid ScrabbleTiles.txt file in your working directory" << std::endl;
                error = true;
            }
            else {
                int tileScore = std::stoi(line.substr(SCORE_INDEX,line.length()));
                Tile tempTile(tileCharacter, tileScore);
                this->tileBag->addBack(&tempTile);
            }
        }
    }
    else {
        std::cout << "Please make sure there is a valid ScrabbleTiles.txt file in your working directory" << std::endl;
        error = true;        
    }
    return error;
}



void Session::generatePlayers() {
    bool inputtingUsernames = true;
    bool invalidInput = false;
    int currentUser = 1;
    while (inputtingUsernames) {
        std::string userInput;
        if (!invalidInput) {
            std::cout << "Enter a name for player " + std::to_string(currentUser) +" (uppercase characters only)" << std::endl << "> "; 
        } else {
            std::cout << "> ";
        }
        invalidInput = false;
        std::getline(std::cin, userInput);
        // Code yoinked from https://stackoverflow.com/questions/48082092/c-check-if-whole-string-is-uppercase
        // This will check if all characters in a string are uppercase characters AND if they are only valid chars (A-Z)
        if (std::cin.eof() || userInput == "^D") {
            std::cout << std::endl; 
            inputtingUsernames = false;
        } else if (std::all_of(userInput.begin(), userInput.end(), [](unsigned char c){ return std::isupper(c); }) && userInput.length() > 0) { 
            // If the currentUser is 1 then create the first player object and add to currentUser, otherwise
            // create playerTwo and end the loop
            if (currentUser == 1) {
                this->playerOne = new Player(userInput, this->tileBag);
                currentUser += 1;
            }
            else if (this->playerOne->getName() != userInput) {
                this->playerTwo = new Player(userInput, this->tileBag);
                inputtingUsernames = false;
            }
            // Adding an empty line after each input
            std::cout << std::endl;
        } else {
            std::cout << "Invalid Input" << std::endl;
            invalidInput = true;
        }
        
        
    }
    return;
}



Player* Session::getCurrentPlayer() {
    Player* currentPlayer = (playerOnesTurn) ? this->playerOne : this->playerTwo;
    return currentPlayer;
}

void Session::swapCurrentPlayer() {
    this->getCurrentPlayer()->fillHand();
    // this->getCurrentPlayer()->addOneTile();
    this->playerOnesTurn = !playerOnesTurn;
    return;
}


Player* Session::getPlayer(int playerNumber) {
    Player* currentPlayer = (playerNumber == 1) ? this->playerOne : this->playerTwo;
    return currentPlayer;
}


BoardVector* Session::getBoard() {
    return &board;
}


// The board positions are already validated as empty before running placeTile(), so no validation needed
void Session::placeTiles(std::vector<int>* handIndexes, std::vector<std::pair<int, int>>* tileCoords) {
    for (unsigned int i = 0; i < handIndexes->size(); i++) {
        Tile* tileInHand = this->getCurrentPlayer()->getTileInHand(handIndexes->at(i));
        // Add tile copy then delete it from the players hand.
        int curRow = tileCoords->at(i).first;
        int curCol = tileCoords->at(i).second;
        board[curRow][curCol] = tileInHand->letter;
    }

    // Since the handIndexes are retrieved based on a FULL HAND, when you delete one, the bigger indexes are no 
    // longer going to be accurate. So to fix this, these two loops will delete the biggest index each loop, which
    // means that no indexes are going to be offset upon deletion.
    int tilesToDelete = handIndexes->size();
    for (int i = 0; i < tilesToDelete; i++) { 
        // The biggestIndex is in regards to the biggest index number in the players hand, but the actual value stored is
        // the element in handIndexes that holds the biggest index.
        int biggestIndex = 0;
        for (unsigned int a = 0; a < handIndexes->size(); a++) {
            biggestIndex = (handIndexes->at(a) > handIndexes->at(biggestIndex)) ? a : biggestIndex;
        }
        // After finding the biggest index in the remaining indexes, delete them from both the hand and handIndexes.
        this->getCurrentPlayer()->getHand()->deleteAt(handIndexes->at(biggestIndex));
        handIndexes->erase(handIndexes->begin() + biggestIndex);
    }
    return;
}



bool Session::positionEmpty(std::pair<int, int> position) {
    bool isEmpty = false;
    // The validTilePlacement() function in gameEngine can pass in positions outside of the boards boundaries,
    // so this checks to make sure it doesn't try and access something like board[-1][15]
    if (position.first >= 0 && position.first < BOARD_SIZE && position.second >= 0 && position.second < BOARD_SIZE) {
        isEmpty = (board[position.first][position.second] == ' ');
    }
    // This check is only needed for the validTilePlacement function when checking the coords around a given coord
    else if (position.first < 0 || position.first >= BOARD_SIZE || position.second < 0 || position.second >= BOARD_SIZE) {
        isEmpty = true;
    }
    return isEmpty;
}


void Session::printPlayersHand(Player* player) {
    LinkedList* playersHand = player->getHand();
    for (int i = 0; i < playersHand->size(); i++ ) {
        Tile* curTile = playersHand->get(i)->tile;
        // Can't print the "," at the end of the hand printing statement so this if statement checks for the 
        // moment when i == hand size-1 (-1 because of the array index starting at 0) to change the print statement.
        if (i != (playersHand->size() - 1)) {
            std::cout << curTile->letter << "-" << curTile->value << ", ";
        }
        else {
            std::cout << curTile->letter << "-" << curTile->value;
        }
    }
    std::cout << std::endl << std::endl;
    return;
}

int Session::getTileBagSize() {
    return this->tileBag->size();
}

LinkedList* Session::getTileBag() {
    return this->tileBag;
}