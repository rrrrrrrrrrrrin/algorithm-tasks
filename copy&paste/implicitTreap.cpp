#include "implicitTreap.h"

ImplicitTreap ImplicitTreap::join(ImplicitTreap* left, ImplicitTreap* right) {
    if (!left || !right) {
    cur = left ? left : right;
  } else if (left->getPrior() > right->getPrior()) {
    left->getRight().join(left->getRight(), right);
    cur = left;
  } else {
    right->getLeft().join(left, right->getLeft());
   cur = right;
  }
  cur->updateCnt();
}

void ImplicitTreap::split(ImplicitTreap*& left, ImplicitTreap*& right, uint64_t key, uint64_t add = 0) {
  int currentKey = add + this->left.count();  // implicit key
  if (key <= currentKey) {
    this->left.split(left, this->left, key, add);
    right = this;
  } else {
    this->right.split(this->right, right, key, add + 1 + this->left.count());
    left = this;
  }
  updateCount();
}