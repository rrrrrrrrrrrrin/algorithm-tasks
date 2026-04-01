#include <fstream>
#include <iostream>

#include "vector.h"

struct Student {
  int id;
  double grade;
};

// Merges 2 subarrays of array
// 1st subarray is array[left...mid]
// 2nd subarray is array[mid+1...right]
void merge(Vector<Student>& array, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // Create temp vectors
  Vector<Student> L(n1);
  Vector<Student> R(n2);

  // Copy data to temp vectors
  for (int i = 0; i < n1; i++) {
    L[i] = array[left + i];
  }

  for (int i = 0; i < n2; i++) {
    R[i] = array[mid + 1 + i];
  }

  int i = 0;
  int j = 0;
  int k = left;

  // Merge the temp vectors back
  // into array[left...right]
  while (i < n1 && j < n2) {
    if (L[i].grade <= R[j].grade) {
      array[k] = L[i];
      i++;
    } else {
      array[k] = R[j];
      j++;
    }
    k++;
  }

  // Copy the remaining elements of L[],
  // if there are any
  while (i < n1) {
    array[k] = L[i];
    i++;
    k++;
  }

  // Copy the remaining elements of R[],
  // if there are any
  while (j < n2) {
    array[k] = R[j];
    j++;
    k++;
  }
}

// Begin is left index and end is right index
// of subarray of array to be sorted
void mergeSort(Vector<Student>& array, int left, int right) {
  if (left >= right) {
    return;
  }

  int mid = left + ((right - left) / 2);
  mergeSort(array, left, mid);
  mergeSort(array, mid + 1, right);
  merge(array, left, mid, right);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cout << "Usage: input-file\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input) {
    std::cout << "Couldn't open input file\n";
    return 2;
  }

  int n = 0;
  input >> n;

  Vector<Student> students(n);

  // Assign ids to students
  for (int i = 0; i < n; i++) {
    students[i].id = i + 1;
    input >> students[i].grade;
  }

  // Sort students by grade
  mergeSort(students, 0, n - 1);

  int median = n / 2;
  std::cout << students[0].id << " " << students[median].id << " "
            << students[n - 1].id << '\n';

  return 0;
}