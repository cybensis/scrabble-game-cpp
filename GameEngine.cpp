#include "GameEngine.h"

GameEngine::GameEngine()
{
    this->instanceData = new Session();
    this->currentPlayer = this->instanceData->getCurrentPlayer();
    this->scoreThisTurn = 0;
    this->playerOnesTurn = true;
    this->turnFinished = false;
    this->tilesPlacedThisRound = 0;
    std::cout << "Let's Play!" << std::endl << std::endl;
    gameController();
}

GameEngine::~GameEngine()
{
    delete this->instanceData;
}

void GameEngine::gameController() {
    // NOTE: The whole playerOne, playerTwo, getCurrentPlayer is very shoddy and could definitely be refactored.
    while (true) {
        Player* playerOne = this->instanceData->getPlayer(1);
        Player* playerTwo = this->instanceData->getPlayer(2);
        std::cout << this->currentPlayer->getName() << ", it's your turn" << std::endl;
        std::cout << "Score for " << playerOne->getName() << ": " << playerOne->getScore() << std::endl;
        std::cout << "Score for " << playerTwo->getName() << ": " << playerTwo->getScore() << std::endl;
        printBoard();
        std::cout << "Your hand is" << std::endl;
        this->instanceData->printPlayersHand(this->currentPlayer);
        // handlePlayerTurn handles the 7 moves a player can make per turn, then everything is reset for the next player
        handlePlayerTurn();
        bool gameFinished = checkEndConditions();
        if (gameFinished) {
            std::cout << "Game finito" << std::endl;
            exit(1);
        }
        // Reset all the variables that need to be reset
        this->currentPlayer->addScore(this->scoreThisTurn);
        this->instanceData->swapCurrentPlayer();
        this->currentPlayer = this->instanceData->getCurrentPlayer();
        this->turnFinished = false;
        this->tilesPlacedThisRound = 0;
        this->scoreThisTurn = 0;
        std::cout << std::endl;
    }

}

bool GameEngine::checkEndConditions() {
    bool conditionsMet = false;
    // The end game conditions are:
    // 1. The tile bag is completely empty
    // AND
    // 2. The user either has an empty hand (so they can't put any more pieces down), 
    // OR they skip their turn twice
    if (this->instanceData->getTileBagSize() == 0) {
        if (this->currentPlayer->getHand()->size() == 0 || this->currentPlayer->getConsecutivePasses() >= 2) {
            conditionsMet = true;
        }
    }
    return conditionsMet;
}



void GameEngine::handlePlayerTurn() {
    while ( !this->turnFinished ) {
        std::cout << "> ";
        std::string userInput;
        // getline is needed because just using std::cin >> userInput will ignore everything after whitespaces
        std::getline(std::cin, userInput);
        // NOTE: Probably change userInput so it passes an address not a copy
        if (!validInput(userInput)) {
            std::cout << "Invalid Input" << std::endl;
        }
    }
    return;
}

// NOTE: When placing a tile, this code places it right after the input has been validated, but I believe they want us to queue the place commands, 
//       then execute them all at once, at the end of their turn. Shouldn't be too hard to do if we need to change it.
bool GameEngine::validInput(std::string input) {
    std::stringstream inputStream(input); 
    std::vector<std::string> splitInput; 
    std::string tempString;
    // Start by assuming there are errors, then set to true if input is fully validated. Helps remove repetitive else statements
    bool noErrors = false;
    // Idea taken from https://java2blog.com/split-string-space-cpp/
    // This while loop will take the input thats been read into a string stream, then it will read through the stream
    // until it finds a space, then all the characters up until that space are placed into tempString, which is then
    // pushed onto the splitInput vector.
    while (std::getline(inputStream, tempString, SPACE_DELIMITER)) { 
        // We need to allow for any amount of spaces to be entered, so this checks that once the delimiter picks
        // up a space, that it actually has a string and its not just an empty space.
        if (tempString.length() > 0) { splitInput.push_back(tempString); }
    }
    std::string givenCommand = splitInput.at(COMMAND_INDEX_LOC);
    if (splitInput.size() > 4) { noErrors = false; }
    else if (givenCommand == "place") {
        // The tilesPlacedThisRound != MAX_MOVES_PER_TURN isn't necessary because you can't place a tile if your hand is empty
        // but it's more efficient since it doesn't have to run through all this code to figure the hand is empty. 
        if (splitInput.size() == PLACE_CMD_COMPONENTS && this->tilesPlacedThisRound != MAX_MOVES_PER_TURN) {
            int tileHandIndex = TILE_NOT_FOUND;
            char chosenTileChar;
            if (splitInput.at(TILE_INDEX_LOC).length() == 1) {
                // The splitInput.at(..) returns a string, but we only want the char given, and since a string is an array
                // of chars, using [0] we can get the char value of the provided tile, given the input is only 1 character long
                chosenTileChar = splitInput.at(TILE_INDEX_LOC)[0];
                tileHandIndex = this->instanceData->getCurrentPlayer()->findTile(chosenTileChar);
            }
            std::string coordinates = splitInput[COORDS_INDEX_LOC];
            // Here we check first if the given tile is actually in the players hand, then we check if the third component
            // is equal to "at" like it always should be, then we check the coordinates provided are valid.
            if (tileHandIndex != TILE_NOT_FOUND && splitInput[PLACE_AT_INDEX_LOC] == "at" && validCoordinates(coordinates)) {
                // Using [0] again because we only want the char, and if it passes validCoordinates, then we know its only 1 char,
                // then once we have the char, we can subtract INT_ASCII_OFFSET from it, which will return an int in range 0-14
                int rowCoord = coordinates.substr(0, coordinates.find(COORDINATE_DELIMITER))[0] - INT_ASCII_OFFSET;
                // Need coordinates.find(COORDINATE_DELIMITER) + 1 because we want it to be AFTER The '-'
                int colCoord = std::stoi(coordinates.substr(coordinates.find(COORDINATE_DELIMITER) + 1, coordinates.size()));
                std::pair<int,int> coordinatePair(rowCoord, colCoord);
                // This will only work if the location on the board isn't already taken
                if ( this->instanceData->placeTile(tileHandIndex, coordinatePair) ) {
                    this->scoreThisTurn += this->instanceData->getCurrentPlayer()->getTileInHand(chosenTileChar)->value;
                    this->tilesPlacedThisRound += 1;
                    noErrors = true;
                }
            }
        }
        // If all 4 components aren't there, they could still be trying to enter "place Done", but you can only enter "place Done"
        // if you've placed something down first, otherwise you'll have to use "pass".
        else if (splitInput[DONE_INDEX_LOC] == "Done" && this->tilesPlacedThisRound != 0) { 
            this->instanceData->getCurrentPlayer()->setConsecutivePasses(USER_DID_NOT_PASS);
            this->turnFinished = true;
            noErrors = true;    
        }

    }
    // Need to make sure replace can only be ran once per turn.
    else if (givenCommand == "replace") {
        // Make sure that the tile they've given is only 1 character long, and that they haven't placed a tile this round.
        if (splitInput.at(TILE_INDEX_LOC).length() == 1 && this->tilesPlacedThisRound == 0) { 
            // No need to validate input beyond ".length() == 1" because findTile() will return -1 regardless of its validity
            int tileHandIndex = this->instanceData->getCurrentPlayer()->findTile(splitInput.at(TILE_INDEX_LOC)[0]);
            if (tileHandIndex != -1) {
                this->instanceData->getCurrentPlayer()->replaceTile(tileHandIndex);
                this->turnFinished = true;
                noErrors = true;
            }
        }
        // NOTE: The user can only replace one tile per turn, but I think they can still place tiles, so need to sort that out
    }
    else if (givenCommand == "pass") {
        // Check that only one command was given, AND that they haven't placed any tiles yet. If they have placed a tile, then
        // they need to use "place Done" to end their turn.
        if (splitInput.size() == 1 && this->tilesPlacedThisRound == 0) {
            this->instanceData->getCurrentPlayer()->setConsecutivePasses(USER_PASSED);
            this->turnFinished = true;
            noErrors = true;
        }
    }
    else if (givenCommand == "save") { /* Do save here */ } 
    else if (givenCommand == "quit") {
        // Quit gracefully here 
        // I was thinking of having a boolean class attribute and every time a players turn finishes, it 
        // gets checked, then it exits the GameEngine. 
    }

    return noErrors;

}




bool GameEngine::validCoordinates(std::string coordinates) {
    bool noErrors = true;
    // Need to use string not char or int for coordinates incase they try to enter CC-1A or something like 
    // that which would throw an error.
    std::string rowCoord = coordinates.substr(0, coordinates.find(COORDINATE_DELIMITER));
    // Need coordinates.find(COORDINATE_DELIMITER) + 1 because we want it to be AFTER The '-'
    std::string colCoord = coordinates.substr(coordinates.find(COORDINATE_DELIMITER) + 1, coordinates.size());

    // This if checks for 3 things, first if the provided row coordinate is more than one character long, then two and 
    // three check if the provided character is within the range of the boards row coordinates (A-F), if any one 
    // these fail, then an error is set.
    if (rowCoord.size() > 1 || rowCoord[0] < ROW_RANGE_MIN ||  rowCoord[0] > ROW_RANGE_MAX) {
        noErrors = false;
    }
    // This checks the size of the column coordinate to see if its size is greater than 2 or equal to nothing (0), then it 
    // checks to see if the first given digit is actually a number.
    if ( colCoord.size() > 2 || colCoord.size() == 0 || !std::isdigit(colCoord[0]) ) {
        noErrors = false;
    }
    // The else if first checks if the digit is two characters long, then it checks if the first character is a 0 (because 
    // they can try place C at A-01), then it checks if the second char is an int or not.
    else if (colCoord.size() == 2 && (colCoord[0] == '0' || !std::isdigit(colCoord[1]) ) ) {
        noErrors = false;
    }

    return noErrors;
}




void GameEngine::printBoard()
{
    BoardVector* board = this->instanceData->getBoard();
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
            std::cout << " " << board->at(i).at(a)->letter << " |";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    return;
}