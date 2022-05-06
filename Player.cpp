
#include "Player.h"

Player::Player(std::string playerName, LinkedList* tileBag) {
    this->playerHand = new LinkedList();
    this->playerScore = 0;
    this->consecutivePasses = 0;
    this->playerName = playerName;
    this->tileBag = tileBag;
    // This generates a randomizer using the current time as the seed, which is adequate for this implementation. 
    // NOTE: Keep this commented out for testing purposes since we need to have the randomizer not actually be
    // random by providing it the same seed everytime.
    // seededRandomizer = std::mt19937(time(nullptr));
    this->seededRandomizer = std::mt19937(1);
    // This loop assigns 7 (MAX_TILES_IN_HAND) tiles randomly to the players hand
    for (int i = 0; i < MAX_TILES_IN_HAND; i++) {
        // seededRandomizer() will generate a different random number every time seededRandomizer() is called, but the number 
        // will be something big like 884430747, so to use it to randomly access LinkedList indexes, we take that number and 
        // get the remainder of BIG_NUMBER % TILE_BAG_SIZE, which will always return an element in bounds of the tilebag size.
        int randomIndex = (this->seededRandomizer() % this->tileBag->size());
        // Need to add the tile from the tile bag to the players hand, then remove it from the tile bag
        this->playerHand->addFront(this->tileBag->get(randomIndex)->tile);
        tileBag->deleteAt(randomIndex);
    }

    // NOTE: Probably need to make it so the tiles are added to the players hand in order
    // NOTE: Maybe make the random function a class attribute so I don't have to re-declare it everytime
    return;
}



Player::~Player() {
    //Deconstructor
    delete this->playerHand;
}



void Player::addScore(int pointsToAdd) {
    this->playerScore = this->playerScore + pointsToAdd;
    return;
}



void Player::fillHand() {
    // Generate a random number and randomly choose a tile to add.
    int tilesToAdd = MAX_TILES_IN_HAND - this->playerHand->size();
    int tilesAdded = 0;
    while (this->tileBag->size() > 0 && tilesAdded < tilesToAdd) {
        int randomIndex = (this->seededRandomizer() % this->tileBag->size());
        this->playerHand->addBack(this->tileBag->get(randomIndex)->tile);
        tilesAdded += 1;
    }
    return;
} 



void Player::replaceTile(int tileIndex) {
    this->playerHand->deleteAt(tileIndex);
    // Get a random index with seedRandomizer, then use it to pull and delete a random tile from the tile bag
    int randomIndex = (this->seededRandomizer() % this->tileBag->size());
    this->playerHand->addBack(this->tileBag->get(randomIndex)->tile);
    this->tileBag->deleteAt(randomIndex);
    return;
}



int Player::findTile(char tileChar) {
    // Search through the playerHand LinkedList until the tileInHand has been found by checking for the letter, 
    // then return the index location
    int tileIndex = TILE_NOT_FOUND;
    for (int i = 0; i < this->playerHand->size(); i++) {
        if (this->playerHand->get(i)->tile->letter == tileChar) {
            tileIndex = i;
        }
    }
    return tileIndex;
    // NOTE: Maybe add some additional error checking in case the tile can't be found? Depends on the rest of the implementation.
}

int Player::findTile(char tileChar, std::vector<int>* usedIndexes) {
    // Search through the playerHand LinkedList until the tileInHand has been found by checking for the letter, 
    // then return the index location
    int tileIndex = TILE_NOT_FOUND;
    for (int i = 0; i < this->playerHand->size(); i++) {
        // this checks that the tile this loop matches the tile the player wants AND it also checks that they aren't trying to
        // use a tile thats already been used before
        if (this->playerHand->get(i)->tile->letter == tileChar && std::find(usedIndexes->begin(), usedIndexes->end(), i) == usedIndexes->end()) {
            tileIndex = i;
        }
    }
    return tileIndex;
    // NOTE: Maybe add some additional error checking in case the tile can't be found? Depends on the rest of the implementation.
}



std::string Player::getName() {
    return this->playerName;
}


int Player::getScore() {
    return this->playerScore;
}

LinkedList* Player::getHand() {
    return this->playerHand;
}

Tile* Player::getTileInHand(int handIndex) {
    return this->playerHand->get(handIndex)->tile;
}

int Player::getConsecutivePasses() {
    return this->consecutivePasses;
}

void Player::setConsecutivePasses(bool userPassedTurn ) {
    if (userPassedTurn) {
        this->consecutivePasses += 1;
    }
    else {
        this->consecutivePasses = 0;
    }
}