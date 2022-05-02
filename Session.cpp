
#include "Session.h"

Session::Session() {
    // Need to initialise the tileBag first before creating the players so we can pass tiles to them.
    tileBag = new LinkedList();
    // Need to pass usernames into the new player objects, use temp names for now
    playerOne = new Player("NAME1", tileBag);
    playerOne = new Player("NAME2", tileBag);
    // This initialises the 2D board vector as a 15x15 array, and all spaces are initialised with a single whitespace
    board(BOARD_SIZE, std::vector<char>(BOARD_SIZE, ' '));
    playerOnesTurn = true;
    movesThisTurn = 0;
    // This is just for testing until we can get the tile bag going
    char tempChars[24] = {'A','A','B','B','C','C','D','D','E','E','F','F','G','G','H','H','I','I','J','J','K','K','L','L'};
    for (int i = 24; i < 24; i++) {
        Tile tmpTile(tempChars[i], 1);
        this->tileBag->addFront(tmpTile);
    }
    return;
}



Session::~Session() {
    // TODO
}



Player* Session::getCurrentPlayer() {
    Player* currentPlayer;
    if (playerOnesTurn) {
        currentPlayer = this->playerOne
    }
    else {
        currentPlayer = this->playerTwo
    }
    return currentPlayer;
}



void Session::placeTile(Tile* newTile, std::pair <int, int> position) {
    // Check to make sure the board position is empty before placing it.
    if (board[position.first][position.second] == ' ') {
        board[position.first][position.second] = newTile->letter;
        getCurrentPlayer()->removeTile(newTile);
        // Tiles are added back into a players hand, and score is counted, at the end of a players turn so keep that in mind
        // Update movesThisTurn maybe if i keep it
    }
    return;
}
