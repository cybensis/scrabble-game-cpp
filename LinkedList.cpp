
#include "LinkedList.h"

LinkedList::LinkedList() :head(nullptr), tail(nullptr), length(0) { // Apparently it is faster doing it this way
//    this->head = nullptr;
//    this->tail = nullptr;
//    this->length = 0;
}

LinkedList::~LinkedList() {
    clear();
}

int LinkedList::size() {
    return this->length;
}

void LinkedList::clear() {
    Node* curNode = this->head;
    Node* tmpNode = nullptr;

    while (curNode != nullptr) {
        tmpNode = curNode;
        curNode = curNode->next;
        delete tmpNode;
    }

    this->head = nullptr;
    this->tail = nullptr;
    this->length = 0;
}

Node* LinkedList::get(int i) {
    Node* newNode = this->head;
    while (i < this->length && i >= 0) {
        newNode = newNode->next;
        i--;
    }
    return newNode;
}

// Adds a copy tile to the front of the linked list
void LinkedList::addFront(Tile* tile) {
    this->head = new Node(new Tile(*tile), this->head);
    if (this->tail == nullptr) {
        this->tail = this->head;
    }
    this->length++;

}
void LinkedList::addBack(Tile* tile) {
    if (this->length == 0) {
        addFront(tile);
    } else {
        this->tail->next = new Node(new Tile(*tile), nullptr);
        this->length++;
    }
}

void LinkedList::deleteFront() {
    if (this->length > 0) {
        Node* tmpNode = this->head;
        this->head = this->head->next; // Move head pointer to next node
        this->tail = (this->head == nullptr) ? nullptr : this->tail; // If there is only one node initially, and it is about to be deleted...
        delete tmpNode;
        this->length--;
    }
}

void LinkedList::deleteBack() {

    if (this->length > 0) {
        if (this->head->next == nullptr) { // If there is only one node in the linked list
            Node* tmpNode = this->head;
            this->head = nullptr;
            this->tail = nullptr;
            delete tmpNode;
        } else {
            // Find the second last node and change its 'next' pointer to null
            Node* prevNode = this->head;
            Node* tmpNode = this->tail;
            for (int i = 0; i < this->length - 1; i++) { // Iterate to find the second last node
                prevNode = prevNode->next;
            }
            prevNode->next = nullptr;
            this->tail = prevNode; // Move the tail back
            delete tmpNode; // Delete the initial tail

        }
        this->length--;
    }

}

void LinkedList::addAt(Tile* tile, int i) {
    // TODO
//    Node* newNode = new Node(new Tile(*tile), nullptr);
    this->length++;
}
void LinkedList::deleteAt(int i) {
    // TODO
    this->length--;
}
