#include <iostream>
#include <fstream>
#include <cstring>
#include "edit.h"

int main(int argc, char* argv[]) {
  if (argc != 4) {
    std::cout << "Usage: input-file output-file\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  std::ifstream commands(argv[2]);
  if (!commands) {
    std::cout << "Couldn't open input file\n";
    return 3;
  }

  std::ofstream output(argv[3]);
  if (!output) {
    std::cout << "Couldn't open input file\n";
    return 4;
  }

  // =============================== Parse the input and commands ===============================
  char ch;
  char* str =  new char[1001]{0};
  int i = 0;
  Vector<char*> inBuffer;
  while ((ch = input.get()) != EOF) {
    if (ch == '\n') {
      str[i] = '\0';  // cut remnants of previous string

      // inBuffer is storing pointers: store distinct allocation each line
      char* copy = new char[i + 1]{0};
      std::memcpy(copy, str, i + 1);
      inBuffer.push_back(copy);

      i = 0;
      continue;
    }
    str[i++] = ch;
  }
  input.close();

  // After EOF
  str[i] = '\0';
  char* copy = new char[i + 1]{0};
  std::memcpy(copy, str, i + 1);
  inBuffer.push_back(copy);
  i = 0;

  Vector<char*> cmdBuffer;
  while ((ch = input.get()) != EOF) {
    if (ch == '\n') {
      str[i] = '\0';

      char* copy = new char[i + 1]{0};
      std::memcpy(copy, str, i + 1);
      cmdBuffer.push_back(copy);

      i = 0;
      continue;
    }
    str[i++] = ch;
  }
  commands.close();

  str[i] = '\0';
  char* copy = new char[i + 1]{0};
  std::memcpy(copy, str, i + 1);
  inBuffer.push_back(copy);
  
  delete[] str;


}