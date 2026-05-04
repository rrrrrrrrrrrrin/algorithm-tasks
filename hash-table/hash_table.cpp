#include "hash_table.h"

void HashTable::insert(int key) {
  int hash = get_hash(key);
  Node* current = vec[hash];  // bucket for key

  // Search key starting at first node in bucket
  while (current != nullptr) {
    if (current->key == key) {
      return;  // key already exists
    }
    current = current->next;  // continue search through linked list
  }

  // If key not found, then create new node and
  // insert it at front of linked list of the bucket
  Node* newNode = new Node{key, vec[hash]};
  vec[hash] = newNode;  // new head of vec[hash]
}

void HashTable::remove(int key) {
  int hash = get_hash(key);
  Node* current = vec[hash];
  Node* prev = nullptr;

  while (current != nullptr) {
    // Key was found in node
    if (current->key == key) {
      // it isn't the 1st node
      // link prev and next of current node
      // (prev -> current -> next  =>  prev -> next)
      if (prev != nullptr) {
        prev->next = current->next;
      }
      // it is the 1st node
      else {
        // bucket heads moves to next node
        vec[hash] = current->next;
      }

      delete current;
      return;
    }
    prev = current;
    current = current->next;
  }
}

bool HashTable::keyIsInSet(int key) {
  int hash = get_hash(key);
  Node* current = vec[hash];

  while (current != nullptr) {
    if (current->key == key) {
      return true;
    }
    current = current->next;
  }
  return false;
}