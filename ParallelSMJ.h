#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include <thread>
#include <cstdlib>
#include <time.h>
#include <vector>

// explanation of radix sort -> https://www.youtube.com/watch?v=xhr26ia4k38

typedef unsigned int uint;
typedef unsigned long ulong;
typedef std::vector<std::vector<int>> digits_bucket;

// constants
const size_t SIZE_R = 4;
const size_t SIZE_S = 4;
const uint MAX_RAND_PSMJ = 5;
const uint NB_THREAD = 4;
const uint MAX_DIGIT_EXCLUDED = 10;

/*
 * test the parallel sort
 */
void testParallelSort(bool printSort);

/*
 * test the parallel version of sort-merge
 */
void testParallelJoin();

/*
 * fill a table with random values
 */
void fillTable(int* table, size_t size, uint maxValue);

/*
 * start a radix sort using threads
 */
void parallelSort(int* table, int size);

/*
 * sort values in buckets by a digit
 */
void radixSort(int* table, digits_bucket& buckets,
               ulong posDigit, uint start, uint end);

/*
 * return the max value of an array
 * NOTE : can be improved with thread
 */
int max(int* table, int size);

/*
 * return the digit at the given position
 * pos of least digit = 0
 * credit : http://stackoverflow.com/a/7438270
 */
int getDigit (int number, int pos);

/*
 * return true if an array is sorted
 */
bool checkSorted(int* table, int size);

/*
 * start a merge using threads
 */
std::vector<std::vector<std::string>> parallelMerge(int* R, int* S, uint sizeR, uint sizeS);

/*
 * adaptation of SortMergeJoin's function
 */
void mergeRoutine(int* startR, int* endR, int* startS, int* endS,
           std::vector<std::string>& results, uint rowR, uint rowS);

/*
 * print a table
 */
void printTable(int* table, int size);

#endif //SORTMERGEJOIN_PARALLELSORT_H
