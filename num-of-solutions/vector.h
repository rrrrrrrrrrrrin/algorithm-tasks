#pragma once
#include <cstdint>  // For uint64_t

const int size_array = 100;

struct Command {
	char array[size_array];

	// Constructor
	Command() {
		for (int i = 0; i < size_array; ++i) {
			array[i] = 0;
		}
	}

	const char& operator[](uint64_t x) const {
		if (x > 99) {
			throw "vector.h Command: index out of range";
		}
		return array[x];
	}
};

template <typename T>
struct Pair {
	T array[2] = { 0 };

	T& operator[](uint64_t x) {
		if (x > 1) {
			throw "vector.h Pair: index out of range";
		}
		return array[x];
	}

	const T& operator[](uint64_t x) const {
		if (x > 1) {
			throw "vector.h Pair: index out of range";
		}
		return array[x];
	}
};

template <typename T>
class Vector {
private:
	T* Data;
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
	Vector(const Vector& other) {
		size = other.size;
		capacity = other.capacity;
		Data = new T[capacity];
		for (int i = 0; i < size; ++i) {
			Data[i] = other.Data[i];
		}
	}

	// Assignment operator
	Vector& operator=(const Vector& other) {
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

	~Vector() {
		delete[] Data;
	};

	uint64_t get_size() const {
		return size;
	}

	int get_size_array() const {
		return size_array;
	}
 
	void push_back(const char* cmd) {
		if (capacity == size) {
			capacity = 2 * capacity;
			T* newData = new T[capacity];

			for (int i = 0; i < size; ++i) {
				newData[i] = Data[i];
			}

			delete[] Data;
			Data = newData;
		}

		for (int i = 0; i < size_array; ++i) {
			Data[size].array[i] = cmd[i];
		}
		size++;
	}

	void fill(T elem)
	{
		for (uint64_t i = 0; i < capacity; i++)
		{
			Data[i] = elem;
		}
	}

	T& operator[](uint64_t x) {
		if (x > size - 1) {
			throw "vector.h Vector: index out of range";
		}
		return Data[x];
	}

	const T& operator[](uint64_t x) const {
		if (x > size - 1) {
			throw "vector.h Vector: index out of range";
		}
		return Data[x];
	}
};
