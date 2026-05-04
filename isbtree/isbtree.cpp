#include <climits>
#include <cstdint>
#include <fstream>
#include <iostream>

#include "vector.h"

struct Node {
  int address;  // current node address
  bool isleaf;

  int key_cnt;       // amount of keys in node
  Vector<int> keys;  // keys are sorted non-descendingly

  int child_cnt;
  Vector<int> children;  // children's addresses

  bool visited;  // node was visited

  Node()
      : address(0), isleaf(false), key_cnt(0), child_cnt(0), visited(false) {}

  Node(int address, bool isleaf, int key_cnt, const Vector<int>& keys,
       int child_cnt, const Vector<int>& children)
      : address(address),
        isleaf(isleaf),
        key_cnt(key_cnt),
        keys(keys),
        child_cnt(child_cnt),
        children(children),
        visited(false) {}
};

// Find node by its address
Node* find_node(Vector<Node>& btree, int address) {
  for (int i = 0; i < btree.get_size(); i++) {
    if (btree[i].address == address) {
      return &btree[i];
    }
  }
  return nullptr;
}

int leaf_level = -1;
bool isBtree(Vector<Node>& btree, Node* node, int t, int level, int& N,
             int min_val, int max_val) {
  // nullptr or cycle detected
  if (node == nullptr || node->visited) {
    return false;
  }

  node->visited = true;
  --N;

  // Every node can contain at most 2t - 1 keys;
  // every branch has at most 2t children

  // 1) check: key_cnt limits (at min for non-root: t-1;
  // as root is passed in isBtree(), this condition will be checked for children
  // nodes later, at most: 2t-1)
  if (node->key_cnt > (2 * t) - 1) {
    return false;
  }

  // 2) check: keys are sorted non-descendingly
  for (int i = 0; i < node->key_cnt; i++) {
    if (node->keys[i] < min_val || node->keys[i] > max_val) {
      return false;
    }

    if (i > 0 && node->keys[i] <= node->keys[i - 1]) {
      return false;
    }
  }

  // 3) check: all leaves are at the same level
  if (node->isleaf) {
    if (leaf_level == -1) {
      leaf_level = level;
    } else if (level != leaf_level) {
      return false;
    }
    return true;
  }

  // 4) check: branch (internal nodes) checks

  // Every node (non-root) contains at min t-1 keys;
  // every branch has at min t children
  if (node->child_cnt != node->key_cnt + 1) {
    return false;
  }

  for (int i = 0; i < node->child_cnt; i++) {
    Node* child = find_node(btree, node->children[i]);

    if (child == nullptr) {
      return false;
    }

    // every node (non-root) contains at min t-1 keys
    if (child->key_cnt < t - 1) {
      return false;
    }

    // Every node (non-leaf) contains keys [k_1; k_t], has t+1 children
    // i-th child contains keys from range [k_(i-1); k_i]
    int next_min = (i == 0) ? min_val : node->keys[i - 1];
    int next_max = (i == node->key_cnt) ? max_val : node->keys[i];

    if (!isBtree(btree, child, t, level + 1, N, next_min, next_max)) {
      return false;
    }
  }

  return true;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: input-file\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  int N;  // amount of nodes

  int t;  // (>= 2) minimum degree

  int root_address;
  input >> N >> t >> root_address;

  Vector<Node> btree;

  char node_type[20];  // branch or leaf
  while (input >> node_type) {
    bool isleaf = false;
    if (node_type[0] == 'l') {
      isleaf = true;
    }

    char zero;
    char x;
    int address;
    input >> zero >> x >> address;

    char bracket;
    char colon;
    int key_cnt;
    input >> bracket >> key_cnt >> colon;

    Vector<int> keys;
    for (int i = 0; i < key_cnt; i++) {
      int key;
      input >> key;
      keys.push_back(key);
    }
    input >> bracket;

    Vector<int> children;
    if (!isleaf) {
      int child_cnt;
      input >> bracket >> child_cnt >> colon;

      for (int i = 0; i < child_cnt; i++) {
        int child;
        input >> child;
        children.push_back(child);
      }
      input >> bracket;
    }

    btree.push_back(
        Node(address, isleaf, key_cnt, keys, children.get_size(), children));
  }
  input.close();

  Node* root = find_node(btree, root_address);

  // 0) check: root exists - ?
  //        root should contain at least 1 key
  //        every node can contain at most 2t - 1 keys;
  if (root == nullptr || root->key_cnt == 0 || root->key_cnt > (2 * t) - 1) {
    std::cout << "no\n";
    return 0;
  }

  int level = 0;
  // Recursive validation
  bool isbtree = isBtree(btree, root, t, level, N, INT_MIN, INT_MAX);

  if (isbtree && N == 0) {
    std::cout << "yes\n";
  } else {
    std::cout << "no\n";
  }

  return 0;
}