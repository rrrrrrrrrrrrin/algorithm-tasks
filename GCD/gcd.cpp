#include <cstdint>
#include <fstream>
#include <iostream>

#include "vector.h"

int64_t GCD(int64_t a, int64_t b) {
  int64_t A = a < 0 ? -a : a;
  int64_t B = b < 0 ? -b : b;

  while (B != 0) {
    int64_t r = A % B;
    A = B;
    B = r;
  }

  return A == 0 ? int64_t(1) : A;
}

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

  int64_t N = 0;
  int64_t K = 0;

  input >> N >> K;
  input.ignore();

  Vector<int64_t> vec(N);

  for (int64_t i = 0; i < N; i++) {
    int64_t num = 0;
    input >> num;
    vec.push_back(num);
  }

  input.close();

  for (int64_t i = 0; i <= N - K; i++) {
    int64_t current_sum = 0;
    int64_t fGCD = 0;

    for (int64_t j = 0; j < K; j++) {
      fGCD = fGCD == 0 ? vec[i + j] : GCD(vec[i + j], fGCD);
      current_sum += vec[i + j];
    }

    output << current_sum << ' ' << fGCD << "\n";
  }

  output.close();

  return 0;
}