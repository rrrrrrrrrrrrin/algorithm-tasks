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

  Vector<int64_t> LS(K);
  Vector<int64_t> RS(K);

  int64_t current_sum = 0;
  int64_t fGCD = 0;
  for (int64_t j = 0; j < K; j++) {
    current_sum += vec[j];

    LS.push_back(vec[j]);
  }

  for (int64_t i = 0; i < K; i++) {
    fGCD = fGCD == 0 ? LS[i] : GCD(LS[i], fGCD);
  }

  for (int64_t i = 0; i < K; i++) {
    RS.push_back(LS.top());
    LS.pop();
  }
  RS.pop();

  for (int64_t i = 0; i < K - 1; i++) {
    LS.push_back(RS.top());
    RS.pop();
  }

  output << current_sum << ' ' << fGCD << "\n";

  for (int64_t j = K; j < N; j++) {
    current_sum = current_sum + vec[j] - vec[j - K];

    fGCD = 0;
    LS.push_back(vec[j]);
    for (int64_t i = 0; i < K; i++) {
      fGCD = fGCD == 0 ? LS[i] : GCD(LS[i], fGCD);
    }

    for (int64_t i = 0; i < K; i++) {
      RS.push_back(LS.top());
      LS.pop();
    }
    RS.pop();

    for (int64_t i = 0; i < K - 1; i++) {
      LS.push_back(RS.top());
      RS.pop();
    }

    output << current_sum << ' ' << fGCD << "\n";
  }

  output.close();

  return 0;
}