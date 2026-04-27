#include <fstream>
#include <iostream>
#include "aatree_class.h"
#include "aatree_set.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: input-file commands-file output-file\n";
        return 1;
    }

    std::ifstream input(argv[1], std::ios::binary);
    if (!input) {
        std::cout << "Couldn't open input file\n";
        return 2;
    }

    std::ofstream output(argv[2], std::ios::binary);
    if (!output) {
        std::cout << "Couldn't open output file\n";
        return 3;
    }

    aatree_set set;

    int N;
    input >> N;
    for (int i = 0; i < N; i++) {

    }

    return 0;
}
