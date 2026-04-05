#ifndef MERGE_SECOND_H
#define MERGE_SECOND_H
#include "vector.h"

void mergeSecond(Vector<Pair<int>>& array, int left, int mid, int right,
                 int64_t& crossings);
void mergeSortSecond(Vector<Pair<int>>& array, int left, int right,
                     int64_t& crossings);

#endif