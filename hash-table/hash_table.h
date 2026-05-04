#ifndef HASH_T
#define HASH_T
#include "vector.h"

// Basically implementing std::unordered_set

struct Node {
  int key;
  Node* next;  // ptr to next node in same bucket
};

// Collision resolution: separate chaining;
// each bucket in vec stores a linked list of Nodes
class HashTable {
 private:
  int size_;
  Vector<Node*> vec;

  // -10^9 < key < 10^9 (key < |10^9|)
  int get_hash(int key) const {
    return ((key % size_) + size_) %
           size_;  // to find remainder of negative key
  }

 public:
  HashTable(int size) : size_(size), vec(size) {}

  void insert(int key);
  void remove(int key);
  bool keyIsInSet(int key);

  ~HashTable() {
    for (int i = 0; i < size_; i++) {
      Node* current = vec[i];
      while (current != nullptr) {
        Node* next = current;
        current = current->next;
        delete next;
      }
      vec[i] = nullptr;
    }
  }
};

#endif