
#include "LinkedList.h"

#include <iostream>
#include "GameEngine.h"

#define EXIT_SUCCESS    0

int main(void) {
   LinkedList* list = new LinkedList();
   delete list;
   GameEngine* gameInstance = new GameEngine();
   // Need this so it doesn't have a sook at compile time about gameInstance not being used
   std::cout << typeid(gameInstance).name() << std::endl;
   std::cout << "TODO: Implement Scrabble!" << std::endl;

   return EXIT_SUCCESS;
}
