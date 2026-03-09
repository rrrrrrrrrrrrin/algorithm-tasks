#include <cstdint>
#include <fstream>
#include <iostream>

#include "vector.h"

int64_t GCD(int64_t a, int64_t b) {
  if (a == 0 && b == 0) {
    return int64_t(0);
  }

  int64_t A = a < 0 ? -a : a;
  int64_t B = b < 0 ? -b : b;

  while (B != 0) {
    int64_t r = A % B;
    A = B;
    B = r;
  }

  return A;
}

struct vecGCD {
  Vector<int64_t> val;  // store vals
  Vector<int64_t> g;    // store prefix gcd
  int64_t top = 0;      // amount of elems

  void push(int64_t x) {
    val.push_back(x);
    if (top == 0) {
      g.push_back(x);
    } else {
      g.push_back(GCD(x, g[top - 1]));
    }
    ++top;
  }

  void pop() {
    val.pop();
    g.pop();
    --top;
  }

  bool empty() const { return top == 0; }

  int64_t top_val() const { return val[top - 1]; }
  int64_t top_gcd() const { return g[top - 1]; }
};

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

  Vector<int64_t> vec;

  for (int64_t i = 0; i < N; i++) {
    int64_t num = 0;
    input >> num;
    vec.push_back(num);
  }

  input.close();

  vecGCD LS;
  vecGCD RS;

  int64_t current_sum = 0;
  for (int64_t j = 0; j < K; j++) {
    current_sum += vec[j];
    LS.push(vec[j]);
  }

  output << current_sum << ' ' << LS.top_gcd() << '\n';

  // Use lambda function
  auto move_LS_to_RS = [&]() {
    while (!LS.empty()) {
      RS.push(LS.top_val());
      LS.pop();
    }
  };

  for (int64_t j = K; j < N; j++) {
    current_sum = current_sum + vec[j] - vec[j - K];

    LS.push(vec[j]);

    // Remove the firstly pushed element of window from RS (FIFO)
    if (RS.empty()) {
      move_LS_to_RS();
    }
    if (!RS.empty()) {
      RS.pop();
    }

    int64_t fGCD = 0;
    if (LS.empty() && RS.empty()) {
      fGCD = 0;
    } else if (RS.empty()) {
      fGCD = LS.top_gcd();
    } else if (LS.empty()) {
      fGCD = RS.top_gcd();
    } else {
      fGCD = GCD(RS.top_gcd(), LS.top_gcd());
    }

    output << current_sum << ' ' << fGCD << '\n';
  }

  output.close();

  return 0;
}