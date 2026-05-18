#ifndef PRQUEUE
#define PRQUEUE
#include <fstream>
#include <iostream>

#include "vector.h"

struct Node {
  int val;  // value (lower vals have higher priority)
  int id;   // line number of push command
};

// Priority queue using min-heap O(logn)
class prqueue {
 private:
  Vector<Node> heap;  // min-heap
  Vector<int> pos;    // pos[id] is idx of node in heap

  static bool lessNode(const Node& a, const Node& b) {
    // Lower vals have higher priority
    if (a.val != b.val) {
      return a.val < b.val;
    }
    // If tie, compare insertion order:
    // earlier insertion has higher priotity
    return a.id < b.id;
  }

  // Swap nodes in heap and swap their idxs in pos
  void swapNodes(int i, int j) {
    Node temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;

    pos[heap[i].id] = i;
    pos[heap[j].id] = j;
  }

  // Used during push or after decreaseKey
  void siftUp(int i) {
    while (i > 0) {
      int j = (i - 1) / 2;  // idx of current node's parent

      if (lessNode(heap[i], heap[j])) {
        swapNodes(i, j);  // move smaller node up
        i = j;            // update current node's idx
      } else {
        break;
      }
    }
  }

  // Heapify (min-heap)
  // Used after extractMin
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
      swapNodes(i, smallest);
      i = smallest;  // continue sifting down from updated idx
    }
  }

 public:
  prqueue() : pos(100000) {
    pos.fill(-1);  // no elems in priority queue
  }

  void push(int x, int lineID) {
    heap.push_back({x, lineID});
    int idx = heap.get_size() - 1;
    pos[lineID] = idx;
    siftUp(idx);  // move elem to its correct position
  }

  void extractMin(std::ofstream& out) {
    if (heap.empty()) {
      out << "*\n";
      return;
    }

    out << heap[0].val << '\n';  // minimum

    pos[heap[0].id] = -1;  // node was deleted

    // Replace root node with last node
    heap[0] = heap[heap.get_size() - 1];
    heap.pop();  // delete that last node

    if (!heap.empty()) {
      pos[heap[0].id] = 0;  // update pos of new root
      siftDown(0);          // heapify (min-heap)
    }
  }

  void decreaseKey(int lineID, int x) {
    int idx = pos[lineID];  // O(1)

    // If elem wasn't deleted and new value x is smaller than it
    // (supposed to be, but checking anyway)
    if (idx != -1 && x < heap[idx].val) {
      heap[idx].val = x;  // update value
      siftUp(idx);        // fix the heap order by moving updated node up
    }
  }
};

#endif