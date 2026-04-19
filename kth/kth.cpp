#include <iostream>
#include "heap.h"

static inline int next_elem(int A, int B, int C, int x1, int x2) {
    // Use uint to wrap around if overflows (according to task)
    uint64_t res = uint64_t(uint32_t(A)) * uint32_t(x1) + uint64_t(uint32_t(B)) * uint32_t(x2) + uint64_t(uint32_t(C));
    return int32_t(uint64_t(res));
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: input-file\n";
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

    int n;
    int k1;
    int k2;

    input >> n >> k1 >> k2;

    int A;
    int B;
    int C;
    int x1;
    int x2;

    input >> A >> B >> C >> x1 >> x2;
    input.close();

    Heap heap;

    int lim = n < k2 ? n : k2;

    if (lim >= 1) { heap.insert(x1); }
    if (lim >= 2) { heap.insert(x2); }

    int prev1 = x1;
    int prev2 = x2;

    for (int i = 3; i <= lim; i++) {
        int x = next_elem(A, B, C, prev1, prev2);
        heap.insert(x);
        prev2 = prev1;
        prev1 = x;
    }

    for (int i = k1 + 1; i <= n; i++) {
        int x = next_elem(A, B, C, prev1, prev2);
        prev2 = prev1;
        prev1 = x;

        if (x < heap.top()) {
            heap.replaceTop(x);
        }
    }

    heap.sort();  // ascending sort

    heap.print(output, k2, k1);

    output.close();

    return 0;
}