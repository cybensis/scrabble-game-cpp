
#include "LinkedList.h"

#include <iostream>
#include "Session.h"

#define EXIT_SUCCESS    0

int main(void) {
   LinkedList* list = new LinkedList();
   delete list;
   Session* test = new Session();
   std::cout << typeid(test->getCurrentPlayer()).name() << std::endl;
   std::cout << "TODO: Implement Scrabble!" << std::endl;

   return EXIT_SUCCESS;
}
