
#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#define MAX_TILES_IN_HAND          7
#define TILE_NOT_FOUND             -1
#include "Node.h"
#include "LinkedList.h"
#include "Tile.h"
#include <string>
#include <random>
#include <iostream>

class Player {
public:

   Player(std::string playerName, LinkedList* tileBag);
   ~Player();
   void replaceTile(int tileIndex);
   // Used after a player has finished their turn, and refills their hand with tiles from the tile bag
   void fillHand();
   // Used after placing a tile.
   void addScore(int pointsToAdd);
   // Used by when looking for the index location of a tile in the players hand based on a char
   int findTile(char tileChar);
   // This is the same as findTile() but this also uses usedIndexes to ignore tiles that have already
   // been placed, based on the hand index
   int findTile(char tileChar, std::vector<int>* usedIndexes);

   int getScore();  
   int getConsecutivePasses();
   std::string getName();
   LinkedList* getHand();
   Tile* getTileInHand(int handIndex);

   // This is called when the user places a tile, or they pass their turn. If the userPassedTurn, then consecutivePasses
   // is incremented, if the user did not pass (userPassedTurn == false), then that resets consecutivePasses to 0.
   void setConsecutivePasses(bool userPassedTurn);

private:
   LinkedList* playerHand; 
   std::string playerName;
   LinkedList* tileBag;
   int playerScore;
   int consecutivePasses;
   std::mt19937 seededRandomizer;
   
};

#endif // ASSIGN2__BOARD_H
