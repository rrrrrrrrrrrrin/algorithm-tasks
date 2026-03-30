#include <fstream>
#include <iomanip>
#include <iostream>

#include "fixed.h"
// #include <chrono>

const int DIGITS = 15;  // 15 decimal points for double

std::ostream& operator<<(std::ostream& out, const Fixed<DIGITS>& x) {
  char* s = x.to_scientific();
  out << s;
  delete[] s;
  return out;
}

int main(int argc, char* argv[]) {
  // auto start = std::chrono::high_resolution_clock::now();

  if (argc != 2) {
    std::cout << "Usage: input-file\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  size_t N = 0;
  input >> N;

  Fixed<DIGITS> ans(0.0);
  char str[1000];
  for (size_t i = 0; i < N; ++i) {
    if (input >> str) {
      Fixed<DIGITS> x(str);
      ans += x;
    }
  }

  std::cout << ans << "\n";

  /*auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop -
  start); std::cout << "\nExecution time: " << duration.count() << "
  milliseconds" << std::endl;*/

  return 0;
}