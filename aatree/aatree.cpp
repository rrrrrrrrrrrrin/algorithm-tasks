#include <fstream>
#include <iostream>

#include "aatree_set.h"

enum class NewLine {
  CRLF,  // "\r\n"
  LF,    // '\n'
  CR     // '\r'
};

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

  NewLine ending = NewLine::LF;
  int c;
  while ((c = input.get()) != EOF) {
    char ch = static_cast<char>(c);
    if (ch == '\r') {
      if (input.peek() == '\n') {
        input.get();  // skip '\n'
        ending = NewLine::CRLF;
      } else {
        ending = NewLine::CR;
      }
      break;
    }
    if (ch == '\n') {
      ending = NewLine::LF;
      break;
    }
  }

  input.seekg(0, std::ios::beg);

  int N;
  input >> N;

  for (int i = 0; i < N; i++) {
    char command;
    input >> command;
    int x;
    input >> x;

    switch (command) {
      case '+':
        set.add(x);
        output << set.rootLevel();
        break;
      case '-':
        set.remove(x);
        output << set.rootLevel();
        break;
      case '?':
        bool f = set.belongs_to_set(x);
        if (f) {
          output << "true";
        } else {
          output << "false";
        }
        break;
    }

    if (ending == NewLine::CRLF) {
      output << "\r\n";
    } else if (ending == NewLine::LF) {
      output << '\n';
    } else {
      output << '\r';
    }
  }

  return 0;
}
