
#include "Session.h"

Session::Session() {
    playerOnesTurn = true;
    // Since you can't declare a 2D vectors size in the header file, it needs to be done here. This loops pushes 15 times,
    // another vector that has 15 char slots in it, all initialised with an empty space.
    for (int i = 0; i < BOARD_SIZE; i++) {
        this->board.push_back(std::vector<char>(BOARD_SIZE, ' '));
    }
    generateTileBag();
    generatePlayers();
    return;
}



Session::~Session() {
    // TODO
    delete this->tileBag;
    delete this->playerOne;
    delete this->playerTwo;
}



void Session::generateTileBag() {
// Need to create the tile bag from ScrabbleTiles.txt first before adding tiles to players hands.
    this->tileBag = new LinkedList();
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
                // NOTE: Exit gracefully here, maybe add bool variable: bool errors = false; then set true here.
                std::cout << "Please make sure there is a valid ScrabbleTiles.txt file in your working directory" << std::endl;
                exit(1);
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
        // NOTE: Need to EXIT GRACEFULLY here, so delete everything that's been made, then return which might be hard since this
        // is a constructor with no return type? I'll figure it out eventually.
        exit(1);
    }
}



void Session::generatePlayers() {
    bool inputtingUsernames = true;
    int currentUser = 1;
    while (inputtingUsernames) {
        std::string userInput;
        std::cout << std::endl << "Enter a name for player " + std::to_string(currentUser) +" (uppercase characters only)" << std::endl << "> "; 
        std::getline(std::cin, userInput);
        // Code yoinked from https://stackoverflow.com/questions/48082092/c-check-if-whole-string-is-uppercase
        // This will check if all characters in a string are uppercase characters AND if they are only valid chars (A-Z)
        if (std::all_of(userInput.begin(), userInput.end(), [](unsigned char c){ return std::isupper(c); }) && userInput.length() > 0) { 
            // If the currentUser is 1 then create the first player object and add to currentUser, otherwise
            // create playerTwo and end the loop
            if (currentUser == 1) {
                this->playerOne = new Player(userInput, this->tileBag);
                currentUser += 1;
            }
            else {
                this->playerTwo = new Player(userInput, this->tileBag);
                inputtingUsernames = false;
            }
            // Adding an empty line after each input
            std::cout << std::endl;
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


// The tile positions should already be validated as empty before running placeTile(), so no validation needed
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
        // The biggestIndex is in regards to the biggest index in the players hand, but the actual value stored is
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
        // moment when i == 7-1 (-1 because of the array index starting at 0) to change the print statement.
        if (i != (MAX_TILES_IN_HAND - 1)) {
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