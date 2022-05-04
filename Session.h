
#ifndef ASSIGN2_SESSION_H
#define ASSIGN2_SESSION_H

#define BOARD_SIZE          15
// When reading in the ScrabbleTiles.txt file, instead of splitting each line on a space, I just 
// use the index value of the string to access it, the character is the first in each line so index 
// 0, then a space at index 1 which we ignore, and score follows immediately after at index 2.
#define CHAR_INDEX      0
#define SCORE_INDEX     2

#include "Node.h"
#include "LinkedList.h"
#include "Player.h"
#include "Tile.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
// using namespace std;

typedef std::vector<std::vector<char>> BoardVector;
//Maybe make a type def for coordinate pairs

class Session {
public:

   Session();
   ~Session();
   void generateTileBag();
   void generatePlayers();
   // Used to get the object of the current player. This saves time instead of having to do if else statements everytime.
   Player* getCurrentPlayer();
   // getOtherPlayer is needed because we need to print both players scores at the start of a new turn,
   // so getting the current player isn't good enough. There's likely a better solution here but it'll 
   // do for now.
   Player* getOtherPlayer();
   void placeTile(Tile* newTile, std::pair <int, int> coordinates);
   // Could change this to getPlayersHand so its only GameEngine controlling the printing
   void printPlayersHand(Player* player);
   BoardVector* getBoard();



private:
   Player* playerOne;
   Player* playerTwo;
   LinkedList* tileBag;
   // This initialises the 2D board vector as a 15x15 array, and all spaces are initialised with a single whitespace
   BoardVector board;
   bool playerOnesTurn;
   int movesThisTurn;

   
};

#endif // ASSIGN2__Session_H
