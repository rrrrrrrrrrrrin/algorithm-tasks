#include <fstream>
#include <iostream>

#include "vector.h"

int64_t binary_search(Vector<int64_t>& vec, int64_t& elem) {
  int64_t idx = 0;
  uint64_t n = vec.get_size();

  int64_t L = 0;
  int64_t R = static_cast<int64_t>(n) - 1;
  while (L <= R) {
    idx = L + ((R - L) / 2);  // middle of the vec
    int64_t cur = vec[idx];

    if (cur < elem) {
      L = idx + 1;
    } else if (cur > elem) {
      R = idx - 1;
    } else {
      return cur;
    }
  }

  // The exact elem wasn't found
  // R = L-1, so R and L switched roles: vec[R] is on the left, vec[L] on the
  // right
  if (R < 0) {
    return vec[0];
  }
  if (L >= static_cast<int64_t>(n)) {
    return vec[n - 1];
  }

  int64_t L_val = vec[R];
  int64_t R_val = vec[L];

  int64_t dL = L_val > elem ? L_val - elem : elem - L_val;
  int64_t dR = R_val > elem ? R_val - elem : elem - R_val;

  // Compare distances (between val and a searched elem),
  // choose the closest to elem val
  if (dL <= dR) {
    return L_val;  // choose the smallest val if distances are equal
  }
  return R_val;
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

  uint64_t n;
  uint64_t k;
  input >> n;
  input >> k;
  input.ignore();

  Vector<int64_t> vec(n);

  int64_t num;
  for (uint64_t i = 0; i < n; i++) {
    input >> num;
    vec.push_back(num);
  }
  input.ignore();

  for (uint64_t i = 0; i < k; i++) {
    input >> num;
    output << binary_search(vec, num) << '\n';
  }

  input.close();
  output.close();
  return 0;
}
