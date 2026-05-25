#ifndef PRQUEUE
#define PRQUEUE
#include <fstream>
#include <iostream>

#include "vector.h"

// Priority queue using min-heap O(logn)
class prqueue {
 private:
  Vector<Pair<int>> heap;  // min-heap

  static bool lessNode(const Pair<int>& a, const Pair<int>& b) {
    // Smaller weight first
    if (a[0] != b[0]) {
      return a[0] < b[0];
    }
    // If tie, compare vertex
    return a[1] < b[1];
  }

  // Used during push
  void siftUp(int i) {
    while (i > 0) {
      int j = (i - 1) / 2;  // idx of current node's parent

      if (lessNode(heap[i], heap[j])) {
        // Move smaller node up
        Pair<int> tmp = heap[i];
        heap[i] = heap[j];
        heap[j] = tmp;
        i = j;  // update current node's idx
      } else {
        break;
      }
    }
  }

  // Heapify (min-heap)
  // Used during pop
  void siftDown(int i) {
    int n = heap.get_size();
    while (true) {
      int smallest = i;   // current node
      int l = 2 * i + 1;  // left child
      int r = 2 * i + 2;  // right child

      // Find smallestt key among 3 nodes

      if (l < n && lessNode(heap[l], heap[smallest])) {
        smallest = l;
      }

      if (r < n && lessNode(heap[r], heap[smallest])) {
        smallest = r;
      }

      if (smallest == i) {
        break;
      }

      // If one of children is smaller than current node,
      // swap with smallest child
      Pair<int> tmp = heap[i];
      heap[i] = heap[smallest];
      heap[smallest] = tmp;
      i = smallest;  // continue sifting down from updated idx
    }
  }

 public:
  bool empty() const { return heap.get_size() == 0; }

  void push(const Pair<int>& x) {
    heap.push_back(x);
    siftUp(heap.get_size() - 1);
  }

  const Pair<int>& top() { return heap[0]; }

  void pop() {
    int n = heap.get_size();
    if (n == 0) {
      return;
    }

    heap[0] = heap[n - 1];
    heap.pop();

    if (!heap.empty()) {
      siftDown(0);
    }
  }
};

#endif