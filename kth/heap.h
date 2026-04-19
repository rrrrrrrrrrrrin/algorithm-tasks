#ifndef HEAP_H
#define HEAP_H

#include <cstdint>
#include <fstream>
#include "vector.h"

class Heap {
private:
	Vector<int> array;

    // Function to maintain heap property — parent nodes are always greater than (max-heap) their children
    // Sifting nodes down O(n)
    void heapify(int i)
    {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int size = array.get_size();

        if (left < size && array[left] > array[largest]) {
            largest = left;
        }

        if (right < size && array[right] > array[largest]) {
            largest = right;
        }

        if (largest != i) {
            int temp = array[i];
            array[i] = array[largest];
            array[largest] = temp;

            heapify(largest);
        }
    }

public:
	Heap() = default;

    // TODO: top(), replaceTop(x), sort() (ascending)

    int top() {
        return array[0];
    }

    void replaceTop(int key) {
        array[0] = key;
        heapify(0);
    }

    int& operator[](int i) {
        return i;
    }

    // Function to insert new key into heap
    void insert(int key)
    {
        array.push_back(key);
        int i = array.get_size() - 1;

        while (i != 0 && array[(i - 1) / 2] < array[i]) {
            int temp = array[i];
            array[i] = array[(i - 1) / 2];
            array[(i - 1) / 2] = temp;

            i = (i - 1) / 2;
        }
    }

    void sort() {
        int size = array.get_size();
        for (int k = size - 1; k > 0; k--) {
            int temp = array[0];
            array[0] = array[k];
            array[k] = temp;

            heapify(0);
        }
    }

    void print(std::ofstream& out, int k2, int k1) {
        for (int i = k1-1; i < k2; i++) {
            out << array[i] << (i != k2 - 1) ? " " : "";
        }
        out << '\n';
	}
};

#endif