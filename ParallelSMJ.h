#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include <thread>
#include <cstdlib>
#include <time.h>

#include "SortMergeJoin.h"

namespace SMJ{

    // typedef
    typedef unsigned int uint;
    typedef unsigned long ulong;
    typedef std::vector<std::vector<int>> digits_bucket;

    // constants
    const size_t SIZE_R = 20;
    const size_t SIZE_S = 20;
    const uint MAX_RAND_PSMJ = 14;
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
     * start a radix sort using threads
     */
    void parallelSort(int* table, size_t size);

    /*
     * sort values in buckets by a digit
     */
    void radixSort(int* table, digits_bucket& buckets,
                   ulong posDigit, uint start, uint end);

    /*
     * find max value of array using thread
     */
    int parallelMax(int* table, size_t size);

    /*
     * find the max value of an array and store it in <max>
     */
    void maxRoutine(int* start, int* end, int* max);

    /*
     * return the digit at the given position
     * pos of least digit = 0
     * credit : http://stackoverflow.com/a/7438270
     */
    int getDigit (int number, int pos);

    /*
     * start a merge using threads
     */
    std::vector<std::vector<std::string>> parallelMerge(int* R, int* S, uint sizeR, uint sizeS);

}

#endif //SORTMERGEJOIN_PARALLELSORT_H
