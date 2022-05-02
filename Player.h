
#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#define MAX_TILES_IN_HAND          7
#include "Node.h"
#include "LinkedList.h"
#include "Tile.h"
#include <string>
#include <random>

class Player {
public:

   Player(std::string playerName, LinkedList* tileBag);
   ~Player();
   void replaceTile(Tile* replacementTile, Tile* tileInHand);
   // Used after placing a tile.
   void addTile(Tile* tile);
   // Used after placing a tile.
   void updateScore( int pointsToAdd);
   // When the players places a tile down in Session.cpp, it needs to be removed from their hand with this function.
   void removeTile(Tile* tile);
   // Used by removeTile and replaceTile when looking for a tile that matches the one supplied in the arguments, and returns
   // the index value its first match in the LinkedList
   int findTile(Tile* tile) {

   }

private:
   LinkedList* playerHand; 
   std::string playerName;
   LinkedList* tileBag;
   int playerScore;
   
};

#endif // ASSIGN2__BOARD_H
