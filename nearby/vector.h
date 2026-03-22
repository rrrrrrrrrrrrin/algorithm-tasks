#ifndef VECTOR_H
#include <cstdint> // For uint64_t

const int size_array = 100;

template <typename T> class Vector {
private:
  T *Data;
  uint64_t capacity = 0;
  uint64_t size = 0;

public:
  // Constructors
  Vector() {
    size = 0;
    capacity = 10;
    Data = new T[capacity];
  }

  Vector(uint64_t size) {
    this->size = 0;
    this->capacity = size;
    Data = new T[capacity];
  }

  // Copy constructor
  Vector(const Vector &other) {
    size = other.size;
    capacity = other.capacity;
    Data = new T[capacity];
    for (int i = 0; i < size; ++i) {
      Data[i] = other.Data[i];
    }
  }

  // Assignment operator
  Vector &operator=(const Vector &other) {
    if (this == &other) {
      return *this;
    }
    size = other.size;
    capacity = other.capacity;
    for (int i = 0; i < size; ++i) {
      Data[i] = other.Data[i];
    }
    return *this;
  }

  ~Vector() { delete[] Data; };

  uint64_t get_size() const { return size; }

  void push_back(const T &value) {
    if (capacity == size) {
      capacity = 2 * capacity;
      T *newData = new T[capacity];

      for (uint64_t i = 0; i < size; ++i) {
        newData[i] = Data[i];
      }

      delete[] Data;
      Data = newData;
    }

    Data[size] = value;
    size++;
  }

  void fill(const T &elem) {
    for (uint64_t i = 0; i < capacity; i++) {
      Data[i] = elem;
    }
    size = capacity;
  }

  T &operator[](uint64_t x) {
    if (x > size - 1) {
      throw "vector.h Vector: index out of range";
    }
    return Data[x];
  }

  const T &operator[](uint64_t x) const {
    if (x > size - 1) {
      throw "vector.h Vector: index out of range";
    }
    return Data[x];
  }
};

#endif