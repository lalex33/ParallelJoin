#ifndef SORTMERGEJOIN_SMJUTILITY_H
#define SORTMERGEJOIN_SMJUTILITY_H

#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <time.h>
#include <string>
#include <cstdint>
#include <fstream>
#include <cstdio>
#include <ctime>
#include <limits.h>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <sys/time.h>

namespace SMJ {

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
    std::string getTuple(int rowR, int *tupleR, int rowsS, int *tupleS);

    /*
     * get time
     * https://github.com/iwiwi/parallel-radix-sort/blob/master/measure.cc
     */
    inline double sec() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
}

#endif //SORTMERGEJOIN_SMJUTILITY_H
