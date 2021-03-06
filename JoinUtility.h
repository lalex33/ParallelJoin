#ifndef SORTMERGEJOIN_SMJUTILITY_H
#define SORTMERGEJOIN_SMJUTILITY_H

#include <cstdlib>
#include <string>
#include <vector>
#include <sys/time.h>
#include <iostream>
#include <sstream>

// typedef
typedef unsigned int uint;
typedef unsigned long ulong;

/*
 * fill a table of rows with random integer between 0 and <maxValue>
 */
void fillTable(int *table, size_t size, uint maxValue);

/*
 * return true if an array is sorted
 */
bool checkSorted(int *table, size_t size);

/*
 * return true if two arrays are merged
 */
bool checkMerge(int* R, int sizeR, int* S, int sizeS,  std::vector<std::string> resultFound);

/*
 * print rows of a table
 */
void printTable(int *table, size_t size);

/*
 * print sorted relations then the result of sort-merge join
 */
void printSortMerge(int *r, int *s, int sizeR, int sizeS, const std::vector<std::string> &result);

/*
 * print a match
 */
std::string getTuple(int rowR, int tupleR, int rowsS, int tupleS);

/*
 * get time
 * https://github.com/iwiwi/parallel-radix-sort/blob/master/measure.cc
 */
inline double sec() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

/*
 * return max value in the range [start,end)
 */
inline int max(int *start, int *end){
    int max = *(start++);
    while(start != end){
        if(*start > max){
            max = *start;
        }
        ++start;
    }
    return max;
}

/*
 * return the final merge list
 */
std::vector<std::string> assembleResults(std::vector<std::vector<std::string>> results);


#endif //SORTMERGEJOIN_SMJUTILITY_H
