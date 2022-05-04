
#ifndef ASSIGN2_GAMEENGINE_H
#define ASSIGN2_GAMEENGINE_H

// When printing out the rows for the board a loop 0-14 is used, 0+65 is the decimal
// value for ASCII A, then 1+65 is B, etc, etc.
#define INT_ASCII_OFFSET      65
// This value represents the maximum amount of chars that can be displayed per column in the column 
// header values, e.g. " 1  " and then " 10 ". This is needed because the column needed have 2 spaces 
// appended, but for double digit numbers this makes it too big, so one space needs to be removed.
#define COL_HEADER_LENGTH       4
#define MOVES_PER_TURN          7
#define SPACE_DELIMITER        ' '
#include "Session.h"
#include <sstream>

class GameEngine {
public:

   GameEngine();
   ~GameEngine();

    // inputController is what controls the entire flow of the game (not including menu)
   void gameController();
   void handlePlayerTurn();
   void validateInput(std::string input);
   void printBoard();



private:
    Session* instanceData;

   
};

#endif // ASSIGN2_GAMEENGINE_H
