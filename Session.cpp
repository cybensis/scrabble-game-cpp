
#include "Session.h"

Session::Session() {
    playerOnesTurn = true;
    // Since you can't declare a 2D vectors size in the header file, it needs to be done here. This loops pushes 15 times,
    // another vector that has 15 char slots in it, all initialised with an empty space.
    // Tile emptyTile(' ',0);
    for (int i = 0; i < BOARD_SIZE; i++) {
        this->board.push_back(std::vector<Tile*>(BOARD_SIZE, new Tile(' ', 0)));
    }
    generateTileBag();
    generatePlayers();
    return;
}



Session::~Session() {
    // TODO
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
        std::cout << "Enter a name for player " + std::to_string(currentUser) +" (uppercase characters only)" << std::endl << "> "; 
        std::getline(std::cin, userInput);
        // std::cin >> userInput;
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
    this->playerOnesTurn = !playerOnesTurn;
    this->getCurrentPlayer()->fillHand();
    return;
}


Player* Session::getPlayer(int playerNumber) {
    Player* currentPlayer = (playerNumber == 1) ? this->playerOne : this->playerTwo;
    return currentPlayer;
}


BoardVector* Session::getBoard() {
    return &board;
}


bool Session::placeTile(int handIndex, std::pair <int, int> position) {
    // Check to make sure the board position is empty before placing it.
    Tile* tileInHand = this->getCurrentPlayer()->getTileInHand(handIndex);
    Tile* tileCopy = new Tile(tileInHand->letter, tileInHand->value);
    bool placeSuccess = false;
    if (board[position.first][position.second]->letter == ' ') {
        board[position.first][position.second] = tileCopy;
        getCurrentPlayer()->removeTile(tileCopy);
        // Tiles are added back into a players hand, and score is counted, at the end of a players turn so keep that in mind
        // Update movesThisTurn maybe if i keep it
        placeSuccess = true;
    }
    return placeSuccess;
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
