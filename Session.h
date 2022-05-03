
#ifndef ASSIGN2_SESSION_H
#define ASSIGN2_SESSION_H

#define BOARD_SIZE          15
#include "Node.h"
#include "LinkedList.h"
#include "Player.h"
#include "Tile.h"
#include <vector>
#include <iostream>

typedef std::vector<std::vector<char>> BoardVector;
//Maybe make a type def for coordinate pairs

class Session {
public:

   Session();
   ~Session();
   void placeTile(Tile* newTile, std::pair <int, int> coordinates); // Read the rules to figure out how to add scores to each player
   // Used to get the object of the current player. This saves time instead of having to do if else statements everytime.
   Player* getCurrentPlayer();
   // Maybe create a function that takes in user input here, and that's what controls the flow of each turn



private:
   Player* playerOne;
   Player* playerTwo;
   LinkedList* tileBag;
   // This initialises the 2D board vector as a 15x15 array, and all spaces are initialised with a single whitespace
   BoardVector board{BOARD_SIZE, std::vector<char>{BOARD_SIZE, ' '}};
   bool playerOnesTurn;
   int movesThisTurn;

   
};

#endif // ASSIGN2__Session_H
