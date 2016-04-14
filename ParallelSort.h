//
// Created by Alexis bayle on 14/04/2016.
//

#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include <thread>
#include <cstdlib>
#include <time.h>

typedef unsigned int uint;

const size_t TABLE_ROWS_R = 100;
const size_t TABLE_ROWS_S = 100;

const uint MAX_RAND_VALUE = 10;

void testParallelJoin();

void fillTable(int* table, size_t size, uint maxValue);

void parallelSort(int* table, int size);

void parallelMerge(int* R, int* S);

#endif //SORTMERGEJOIN_PARALLELSORT_H
