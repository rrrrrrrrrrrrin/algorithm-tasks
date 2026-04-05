#include <fstream>
#include <iostream>

#include "merge_first.h"
#include "merge_second.h"

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

  int n = 0;
  input >> n;

  Vector<Pair<int>> segments;
  for (int i = 0; i < n; i++) {
    int x0 = 0;
    int x1 = 0;
    input >> x0 >> x1;
    segments.push_back({x0, x1});
  }

  // Two segments intersect if their order at y=0
  // differs from their order at y=1

  mergeSortFirst(segments, 0, n - 1);  // sort segments by x1

  // Sort segments by x2 to determine the amount of crossings
  int64_t crossings = 0;
  mergeSortSecond(segments, 0, n - 1, crossings);

  std::cout << crossings << '\n';

  return 0;
}