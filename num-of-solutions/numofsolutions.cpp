#include <iostream>

#include "vector.h"

unsigned long long num_of_solutions(int n, unsigned long long mod);

int main() {
  int n = 0;
  unsigned long long mod = 1ULL << 63;
  std::cin >> n >> mod;
  std::cout << num_of_solutions(n, mod) << std::endl;
  return 0;
}

// Integer partition problem using the recursive formula:
//
// P_k(n) = P_k-1(n-1) + P_k(n-k) —  number of ways to partition n using numbers
// up to k P_k-1(n-1) number of partitions of n not using k, these partitions
// have a part of size 1 P_k(n-k) have no part of size 1, use k
//
// The order of parts in partitions doesn't matter (3+2 = 2+3), don't include duplicates
unsigned long long num_of_solutions(int n, unsigned long long mod) {
  Vector<unsigned long long> equation(static_cast<unsigned long long>(n) + 1);
  equation.fill(0);

  equation[0] = 1 % mod;  // const term — one solution
  for (unsigned long long k = 1; k <= static_cast<unsigned long long>(n);
       k++)  // current k (size of the part in a partition)
  {
    for (unsigned long long j = k; j <= static_cast<unsigned long long>(n);
         ++j)  // the total number of partitions (j plays the role of current n)
    {
      // equation[j] is P_k-1(j) number of partitions of j using numbers up to k
      // equation[j - k] is P_k(j-k)
      // After addition: equation[j] is P_k(j)
      //
      // The code line below is P_k(j) = P_k-1(j) + P_k(j-k)
      //
      // j-k is the remaining sum
      equation[j] += equation[j - k];

      if (equation[j] >= mod) {
        equation[j] -= mod;
      }
    }
  }
  return equation[static_cast<unsigned long long>(n)] % mod;
}
