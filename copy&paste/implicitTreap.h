#ifndef TREAP_H
#include <random>
#include <cstdint>

class ImplicitTreap {
 private:
  int64_t key;
  int64_t prior;
  ImplicitTreap* left;
  ImplicitTreap* right;

 public:
  ImplicitTreap() : key(0), prior(std::rand()), left{nullptr}, right{nullptr} {}

  uint64_t cnt() const;
  void updateCnt();

  void join(ImplicitTreap* left, ImplicitTreap* right);
  void split(ImplicitTreap*& left, ImplicitTreap*& right, uint64_t key, uint64_t add = 0);
};

#endif