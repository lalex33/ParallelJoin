#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include <thread>
#include <cstdlib>
#include <time.h>

typedef unsigned int uint;

const size_t TABLE_ROWS_R = 16;
const size_t TABLE_ROWS_S = 16;

const uint MAX_RAND_VALUE = 10;

const uint NB_THREAD = 8;

void testParallelJoin();

void fillTable(int* table, size_t size, uint maxValue);

void parallelSort(int* table, int size);

// utility function
int max(int* table, int size);
int pow(int n, int e);
int getDigit (int number, int pos);

void parallelMerge(int* R, int* S);

#endif //SORTMERGEJOIN_PARALLELSORT_H
