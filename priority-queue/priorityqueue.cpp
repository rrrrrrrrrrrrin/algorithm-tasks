#include <cstring>

#include "prqueue.h"

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

  prqueue priority_queue;

  char command[30] = {0};
  int lineID = 0;
  while (input >> command) {
    lineID++;

    if (std::strcmp(command, "push") == 0) {
      int x;
      input >> x;
      priority_queue.push(x, lineID);
    } else if (std::strcmp(command, "extract-min") == 0) {
      priority_queue.extractMin(output);
    } else if (std::strcmp(command, "decrease-key") == 0) {
      int x;
      int y;
      input >> x >> y;
      priority_queue.decreaseKey(x, y);
    }
  }

  input.close();
  output.close();
  return 0;
}
