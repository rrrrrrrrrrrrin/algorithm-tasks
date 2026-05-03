#include <fstream>
#include <iostream>
#include "vector.h"

struct Node {
    int address;  // current node address
    bool isleaf;

    int key_cnt;  // amount of keys in node
    Vector<int> keys;  // keys sorted non-descendingly

    int child_cnt;
    Vector<int> children;  // children's addresses

    Node()
        : address(0), isleaf(false), key_cnt(0), keys(), child_cnt(0), children() {}

    Node(int address, bool isleaf, int key_cnt, const Vector<int>& keys, int child_cnt, const Vector<int>& children)
        : address(address), isleaf(isleaf), key_cnt(key_cnt), keys(keys), child_cnt(child_cnt), children(children) {}
};

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

    // Every node has t (>= 2) children max; 
    // node (non-root, non-leaf) has at min t/2 children
    // node (non-leaf) with k children contains k-1 keys
    int t; 

    int root;  // address of root
    input >> N >> t >> root;

    Vector<Node> btree;

    char ch;
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

        btree.push_back(Node(address, isleaf, key_cnt, keys, static_cast<int>(children.get_size()), children));
    }
    
    input.close();
	return 0;
}