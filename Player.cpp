
#include "Player.h"

Player::Player(std::string playerName, LinkedList* tileBag) {
    this->playerHand = new LinkedList();
    this->playerScore = 0;
    this->playerName = playerName;
    this->tileBag = tileBag;
    // This generates a randomizer using the current time as the seed, which is adequate for this implementation. 
    // NOTE: Keep this commented out for testing purposes since we need to have the randomizer not actually be
    // random by providing it the same seed everytime.
    // std::mt19937 mt(time(nullptr));
    std::mt19937 mt(1);
    // This loop assigns 7 (MAX_TILES_IN_HAND) tiles randomly to the players hand
    for (int i = 0; i < MAX_TILES_IN_HAND; i++) {
        // mt() will generate a different random number every time mt() is called, but the number will be something big like
        // 884430747, so to use it to randomly access LinkedList indexes, we take that number and get the remainder of 
        // BIG_NUMBER % TILE_BAG_SIZE, which will always return an element in bounds of the tilebag size.
        int randomIndex = (mt() % this->tileBag->size());
        // Need to add the tile from the tile bag to the players hand, then remove it from the tile bag
        this->playerHand->addFront(this->tileBag->get(randomIndex)->tile);
        tileBag->deleteAt(randomIndex);
        // std::cout << this->playerHand->get(i)->tile->letter << std::endl;
    }

    // NOTE: Probably need to make it so the tiles are added to the players hand in order
    // NOTE: Maybe make the random function a class attribute so I don't have to re-declare it everytime
    return;
}



Player::~Player() {
    //Deconstructor
    delete this->playerHand;
}



void Player::updateScore(int pointsToAdd) {
    this->playerScore = this->playerScore + pointsToAdd;
    return;
}



void Player::addTile(Tile* tile) {
    std::mt19937 mt(1);
    // Generate a random number and randomly choose a tile to add.
    int randomIndex = (mt() % this->tileBag->size());
    this->playerHand->addFront(this->tileBag->get(randomIndex)->tile);
    return;
    // NOTE: Probably need to make it so the tiles are added to the players hand in order
    // NOTE: Also this whole thing is not right, can't figure out what to do with it until we start making the game.
} 



void Player::removeTile(Tile* tile) {
    int tileIndex = findTile(tile);
    this->playerHand->deleteAt(tileIndex);
    return;
}



void Player::replaceTile(Tile* replacementTile, Tile* tileInHand) {
    // Get the first index value for the tileInHand in the players hand LinkedList, then delete it, and add the 
    // replacement at that location 
    int tileIndex = findTile(tileInHand);
    this->playerHand->deleteAt(tileIndex);
    this->playerHand->addAt(replacementTile, tileIndex);
    return;
    
    // NOTE: Since it's an ordered list, I think I'll have to re-do this part and add it where it belongs,
    // based on whatever we order it by, e.g. score or letter
}



int Player::findTile(Tile* tile) {
    // Search through the playerHand LinkedList until the tileInHand has been found by checking for the letter, 
    // then return the index location
    int tileIndex;
    for (int i = 0; i < this->playerHand->size(); i++) {
        if (this->playerHand->get(i)->tile->letter == tile->letter) {
            tileIndex = i;
        }
    }
    return tileIndex;
    // NOTE: Maybe add some additional error checking in case the tile can't be found? Depends on the rest of the implementation.
}