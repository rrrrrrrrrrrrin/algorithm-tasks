#ifndef EDIT_H
#include <cstdint>
#include "vector.h"

class Edit {
 private:
  Vector<char*> inBuffer;
  int64_t ptr = 0;
  int64_t start = 0;  // start of selection  
  Vector<char*> clipboard;  // clipboard buffer

 public:
  Edit(Vector<char*> copy){ 
	  inBuffer = copy;
      ptr = 0;
      start = 0;
  }


  // Treap is tree + heap. Stores pairs (x,y): key x - binary search tree, priority y - binary heap
  // Build: O(nlogn), search/insert/delete: O(log n) + bulk operations: split and join 
  // Implement implicit treap



};

#endif