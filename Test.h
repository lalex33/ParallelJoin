//
// Created by Alexis bayle on 19/04/2016.
//

#ifndef SORTMERGEJOIN_TEST_H
#define SORTMERGEJOIN_TEST_H

#include "ParallelSMJ.h"

namespace SMJ{

    // constant SMJ
    const int SIZE_R = 20; // number of rows in R
    const int SIZE_S = 20; // number of rows in S
    const int MAX_RAND_SMJ = 14; // max value for random integer

    // constant Parallel SMJ
    const size_t P_SIZE_R = 20;
    const size_t P_SIZE_S = 20;
    const uint MAX_RAND_PSMJ = 20;

    const uint MAX_SIZE = 1000000;
    const int MAX_RAND_TEST = 32767;

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
     * test on large amount of data
     */
    void testLotOfData();

}


#endif //SORTMERGEJOIN_TEST_H
