#ifndef SORTMERGEJOIN_TEST_H
#define SORTMERGEJOIN_TEST_H

#include "ParallelSMJ.h"
#include "ParallelRadixSort.hpp"
#include "ParallelHashJoin.hpp"

#include <iostream>

namespace SMJ{

    // constant SMJ
    const int SIZE_R = 20; // number of rows in R
    const int SIZE_S = 20; // number of rows in S
    const int MAX_RAND_SMJ = 14; // max value for random integer

    // constant Parallel SMJ
    const size_t P_SIZE_R = 20;
    const size_t P_SIZE_S = 20;
    const uint MAX_RAND_PSMJ = 20;

    const uint MAX_SIZE = 50000000;
    const int MAX_RAND_TEST = 200000000;

    /*
     * launch a join and display result
     */
    void testSortMergeJoin();

    /*
     * test the parallel sort
     */
    void testParallelSort(bool printSort);

    /*
     * test the parallel version of sort-merge
     */
    void testParallelJoin();

    /*
     * test last parallel radix sort
     */
    void testParallelRadixSort();

    /*
     * radix sort on million elements
     */
    void radixSortMillionElements();

    /*
     * test parallel hash join
     */
    void testParallelHashJoin();

}


#endif //SORTMERGEJOIN_TEST_H
