#include "aatree_class.h"

// Skew is a right rotation to replace a subtree
// containing a left horizontal link with one
// containing a right horizontal link instead
aatree* aatree::skew(aatree* t) {
  // t is node (aatree) that needs to be rebalanced
  if (t == nullptr || t->l == nullptr) {
    return t;
  }

  // Swap the pointers of horizontal left links
  if (t->l->level_ == t->level_) {
    aatree* L = t->l;
    t->l = L->r;
    L->r = t;
    return L;
  }

  return t;
}

// Split is a left rotation and level increase to replace a subtree
// containing two or more consecutive right horizontal links with one
// containing two fewer consecutive right horizontal links
aatree* aatree::split(aatree* t) {
  if (t == nullptr || t->r == nullptr || t->r->r == nullptr) {
    return t;
  }

  // We have two horizontal right links
  // Take the middle node R, elevate R, and return R
  if (t->level_ == t->r->r->level_) {
    aatree* R = t->r;
    t->r = R->l;
    R->l = t;
    R->level_ = R->level_ + 1;
    return R;
  }

  return t;
}

aatree* aatree::insert(int x, aatree* t, bool& inserted) {
  // value x to be inserted at root t
  if (t == nullptr) {
    // Create a new leaf node with value x
    inserted = true;
    return new aatree(x, 1);
  }
  if (x < t->val_) {
    t->l = insert(x, t->l, inserted);
  } else if (x > t->val_) {
    t->r = insert(x, t->r, inserted);
  }
  // x == t->val
  else {
    inserted = false;  // elem already exists in set
    return t;
  }

  // Balance only if new node (elem) was added
  if (inserted) {
    t = skew(t);
    t = split(t);
  }

  return t;
}

aatree* aatree::decreaseLevel(aatree* t) {
  int l_lev = get_level(t->l);
  int r_lev = get_level(t->r);

  auto new_level = (l_lev < r_lev ? l_lev : r_lev) + 1;
  if (new_level < t->level_) {
    t->level_ = new_level;
    if (t->r != nullptr && new_level < t->r->level_) {
      t->r->level_ = new_level;
    }
  }
  return t;
}

// aatree* aatree::decreaseLevel(aatree* t) {
//   int l_lev = get_level(t->l);
//   int r_lev = get_level(t->r);

//   // Original A. Andersson's implementation of decreaseLevel;
//   // aatree will never have children with levels greater than parent's
//   if (l_lev < t->level_ - 1 || r_lev < t->level_ - 1) {
//     --t->level_;
//     if (t->r != nullptr && t->r->level_ > t->level_) {
//       t->r->level_ = t->level_;
//     }
//   }
//   return t;
// }

// Seletion of internal node can be turned into deletion
// of leaf node by swapping internal node with either
// its closest predecessor or successor
//
// Because of AA tree property of all nodes of level greater than 1
// having 2 children, successor or predecessor node will be in level 1,
// making their removal trivial
//
// If nothing was removed (deleted == false),
// don't rebalance aatree
aatree* aatree::delete_x(int x, aatree* t, bool& deleted) {
  if (t == nullptr) {
    return nullptr;
  }
  if (x > t->val_) {
    t->r = delete_x(x, t->r, deleted);
  } else if (x < t->val_) {
    t->l = delete_x(x, t->l, deleted);
  } else {
    // t is leaf
    deleted = true;

    if (t->l == nullptr && t->r == nullptr) {
      delete t;
      return nullptr;
    }
    // Reduce to leaf case
    if (get_level(t->l) < get_level(t->r)) {
      aatree* s = successor(t);
      t->val_ = s->val_;
      t->r = delete_x(s->val_, t->r, deleted);
    } else {
      aatree* p = predecessor(t);
      t->val_ = p->val_;
      t->l = delete_x(p->val_, t->l, deleted);
    }
  }

  if (!deleted) {
    return t;
  }
  // else: rebalance aatree

  int old_level = t->level_;
  t = decreaseLevel(t);

  if (t->level_ < old_level) {
    // Skew and split entire level (not just node)
    t = skew(t);
    if (t->r != nullptr) {
      t->r = skew(t->r);
    }
    if (t->r != nullptr && t->r->r != nullptr) {
      t->r->r = skew(t->r->r);
    }

    t = split(t);
    if (t->r != nullptr) {
      t->r = split(t->r);
    }
  }

  return t;
}
