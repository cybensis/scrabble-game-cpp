#include "GameEngine.h"

GameEngine::GameEngine() {
    // Ask for user input here for usernames, tilebag location, etc
    
    // const char delimiter = ' ';
    this->instanceData = new Session();
    // Need this so it doesn't have a sook at compile time about instanceData not being used
    std::cout << typeid(instanceData).name() << std::endl;


}

GameEngine::~GameEngine() {

}