#include "merge_first.h"

// Merges 2 subarrays of array
// 1st subarray is array[left...mid]
// 2nd subarray is array[mid+1...right]
void mergeFirst(Vector<Pair<int>>& array, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;

  // Create temp vectors
  Vector<Pair<int>> L(n1);
  Vector<Pair<int>> R(n2);

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
    if (L[i].first() < R[j].first()) {
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
void mergeSortFirst(Vector<Pair<int>>& array, int left, int right) {
  if (left >= right) {
    return;
  }

  int mid = left + ((right - left) / 2);
  mergeSortFirst(array, left, mid);
  mergeSortFirst(array, mid + 1, right);
  mergeFirst(array, left, mid, right);
}
