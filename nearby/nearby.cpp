#include <fstream>
#include <iostream>

#include "vector.h"

int32_t binary_search(Vector<int32_t>& vec, int32_t& elem) {
  int32_t idx = 0;
  uint32_t n = vec.get_size();

  int32_t L = 0;
  int32_t R = static_cast<int32_t>(n) - 1;
  while (L <= R) {
    idx = L + ((R - L) / 2);  // middle of the vec
    int32_t cur = vec[idx];

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
  if (L >= static_cast<int32_t>(n)) {
    return vec[n - 1];
  }

  int32_t L_val = vec[R];
  int32_t R_val = vec[L];

  // distances can exceed int32_t, so use int64_t
  // + expression itself may overflow b4 being assigned to a wider type
  int64_t dL = static_cast<int64_t>(L_val) - elem;
  if (dL < 0) {
    dL = -dL;
  }

  int64_t dR = static_cast<int64_t>(R_val) - elem;
  if (dR < 0) {
    dR = -dR;
  }

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

  uint32_t n;
  uint32_t k;
  input >> n;
  input >> k;
  input.ignore();

  Vector<int32_t> vec(n);

  int32_t num;
  for (uint32_t i = 0; i < n; i++) {
    input >> num;
    vec.push_back(num);
  }
  input.ignore();

  for (uint32_t i = 0; i < k; i++) {
    input >> num;
    output << binary_search(vec, num) << '\n';
  }

  input.close();
  output.close();
  return 0;
}
