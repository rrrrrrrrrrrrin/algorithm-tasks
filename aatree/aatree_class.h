#ifndef AATREE_H
#define AATREE_H

// AA tree is a variation of the red-black tree
// Child's level equals its parent's — horizontal link
// Individual right horizontal links are allowed, but consecutive ones are
// forbidden; all left horizontal links are forbidden.
class aatree {
 private:
  int val_;
  int level_;
  aatree* l = nullptr;
  aatree* r = nullptr;

 public:
  explicit aatree(int val, int level)
      : val_(val), level_(level), l{nullptr}, r{nullptr} {}

  aatree(const aatree&) = delete;
  aatree& operator=(const aatree&) = delete;

  static aatree* skew(aatree* t);
  static aatree* split(aatree* t);
  static aatree* insert(int x, aatree* t);

  static inline int get_level(aatree* t) {
    return t != nullptr ? t->level_ : 0;
  }

  static aatree* decreaseLevel(aatree* t);

  // Smallest elem in right subtree
  static inline aatree* successor(aatree* t) {
    aatree* temp = t->r;
    while (temp != nullptr && temp->l != nullptr) {
      temp = temp->l;
    }
    return temp;
  }

  // Largest elem in left subtree
  static inline aatree* predecessor(aatree* t) {
    aatree* temp = t->l;
    while (temp != nullptr && temp->r != nullptr) {
      temp = temp->r;
    }
    return temp;
  }

  static aatree* delete_x(int x, aatree* t, bool& deleted);

  static bool belongs(int x, aatree* t) {
    while (t != nullptr) {
      if (x < t->val_) {
        t = t->l;
      } else if (x > t->val_) {
        t = t->r;
      } else {
        return true;
      }
    }
    return false;
  }

  ~aatree() {
    delete l;
    delete r;
  }
};

#endif