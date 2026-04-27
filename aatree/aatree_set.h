#ifndef AATREE_SET_H
#define AATREE_SET_H
#include "aatree_class.h"

class aatree_set {
 private:
  aatree* root = nullptr;

 public:
  aatree_set() = default;

  void add(int x) {
    bool inserted = false;
    root = aatree::insert(x, root, inserted);
  }

  void remove(int x) {
    bool deleted = false;
    root = aatree::delete_x(x, root, deleted);
  }

  bool belongs_to_set(int x) const { return aatree::belongs(x, root); }

  int rootLevel() const { return aatree::get_level(root); }

  ~aatree_set() { delete root; }
};

#endif