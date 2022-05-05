
#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

// When printing out the rows for the board a loop 0-14 is used, 0+65 is the decimal
// value for ASCII A, then 1+65 is B, etc, etc.
#define INT_ASCII_OFFSET       65
// This value represents the maximum amount of chars that can be displayed per column in the column 
// header values, e.g. " 1  " and then " 10 ". This is needed because the column needed have 2 spaces 
// appended, but for double digit numbers this makes it too big, so one space needs to be removed.
#define COL_HEADER_LENGTH       4
#define MAX_MOVES_PER_TURN      7
#define SPACE_DELIMITER        ' '
#define COORDINATE_DELIMITER   '-'
// These values are used when validating user input. If a user enters "place A at C-4", this
// statement is split into 4 components, and the first 4 values represent each of those index locations.
// There is also one for "Done" when entering "place Done"
#define COMMAND_INDEX_LOC       0
#define TILE_INDEX_LOC          1
#define PLACE_AT_INDEX_LOC      2
#define COORDS_INDEX_LOC        3
#define PLACE_CMD_COMPONENTS    4
#define DONE_INDEX_LOC          1
// These two are for the maximum and minimum ranges of the Y coordinate on the board
#define ROW_RANGE_MIN            'A'
#define ROW_RANGE_MAX            'F'
#include "Session.h"
#include <sstream>

class GameEngine {
public:

   GameEngine();
   ~GameEngine();

    // inputController is what controls the entire flow of the game (not including menu)
   void gameController();
   void handlePlayerTurn();
   bool validInput(std::string input);
   void printBoard();
   // validCoordinates checks the validity of the coordinates provided in the "place _ at _-_"
   // then returns true or false is they are valid or not.
   bool validCoordinates(std::string coordinates);



private:
    Session* instanceData;
    int movesThisTurn;
    int scoreThisTurn;
    bool playerOnesTurn;

   
};

#endif // ASSIGN2_GAMEENGINE_H
