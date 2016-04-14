#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include <thread>
#include <cstdlib>
#include <time.h>
#include <vector>

typedef unsigned int uint;
typedef unsigned long ulong;

// constants
const size_t TABLE_ROWS_R = 16;
const size_t TABLE_ROWS_S = 16;
const uint MAX_RAND_VALUE = 506;
const uint NB_THREAD = 8;

void test();

/*
 * test the parallel version of sort-merge
 */
void testParallelJoin();

/*
 * fill a table with random values
 */
void fillTable(int* table, size_t size, uint maxValue);

/*
 * IN DEVELOPMENT
 * start a radix sort using threads
 */
void parallelSort(int* table, int size);

void radixSort(int* table, std::vector<std::vector<int>>& buckets,
               ulong posDigit, uint start, uint end);

// utility function
int max(int* table, int size);
int getDigit (int number, int pos);

/*
 * NOT IMPLEMENTED
 * start a merge using threads
 */
void parallelMerge(int* R, int* S);

#endif //SORTMERGEJOIN_PARALLELSORT_H
