#ifndef HEAP_H
#define HEAP_H

#include <cstdint>
#include <fstream>

#include "vector.h"

class Heap {
 private:
  Vector<int> array;

  // Function to maintain heap property — parent nodes are always greater than
  // (max-heap) their children Sifting nodes down O(n)
  //
  // Restores heap property starting from node i, moving downward
  void heapify(int i, int size) {
    // Current node
    int largest = i;
    // Its children
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    // Find largest key among 3 nodes

    if (left < size && array[left] > array[largest]) {
      largest = left;
    }

    if (right < size && array[right] > array[largest]) {
      largest = right;
    }

    // If one of children is larger than current node,
    // swap with largest child
    if (largest != i) {
      int temp = array[i];
      array[i] = array[largest];
      array[largest] = temp;

      heapify(largest,
              size);  // Continue sifting down from new pos (largest node)
    }
  }

 public:
  Heap() = default;

  // TODO: top(), replaceTop(x), sort() (ascending)

  // The largest element (max-heap)
  int top() { return array[0]; }

  // We need to sort ascendingly,
  // heap already has k2 elements (max size)
  // and key (new value) < top => push it to array, get rid of current top
  void replaceTop(int key) {
    array[0] = key;                // replace top with key (new value)
    heapify(0, array.get_size());  // restore heap property
  }

  // Function to insert new key into heap
  // while keeping heap property
  void insert(int key) {
    // Push new value
    array.push_back(key);
    int i = array.get_size() - 1;

    // Compare new value with its parent
    // Keep heap property: swap them if parent < new value
    // Repeat comparison until reaching the root (array[0]; top)
    //                         or parent > new value
    while (i != 0 && array[(i - 1) / 2] < array[i]) {
      int temp = array[i];
      array[i] = array[(i - 1) / 2];
      array[(i - 1) / 2] = temp;

      i = (i - 1) / 2;
    }
  }

  void sort() {
    int size = array.get_size();
    for (int k = size - 1; k > 0; k--) {
      // Swap root (largest element) with last element of current heap
      int temp = array[0];
      array[0] = array[k];
      array[k] = temp;

      // Shrink, considered for heapifying, heap size by 1
      heapify(0, k);  // restore heap property
    }

    // k2 smallest elems; largest is last elem in array
  }

  // print k2 smallest elems
  void print(std::ofstream& out, int k2, int k1) const {
    for (int i = k1 - 1; i < k2; i++) {
      out << array[i];
      if (i < k2 - 1) {
        out << ' ';
      }
    }
  }
};

#endif