#include <fstream>
#include <iostream>

#include "hash_table.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    std::cout << "Usage: input-file output-file\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  std::ofstream output(argv[2]);
  if (!output) {
    std::cout << "Couldn't open output file\n";
    return 3;
  }

  int N;
  input >> N;

  char command;
  int x;
  HashTable set(N);
  for (int i = 0; i < N; i++) {
    input >> command >> x;
    if (command == '+') {
      set.insert(x);
    } else if (command == '-') {
      set.remove(x);
    } else {  // '?'
      if (set.keyIsInSet(x)) {
        output << "true\n";
      } else {
        output << "false\n";
      }
    }
  }

  input.close();
  output.close();
}