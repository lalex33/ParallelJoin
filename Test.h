//
// Created by Alexis bayle on 19/04/2016.
//

#ifndef SORTMERGEJOIN_TEST_H
#define SORTMERGEJOIN_TEST_H

#include "ParallelSMJ.h"

namespace SMJ{

    // constant SMJ
    const int R_SIZE = 20; // number of rows in R
    const int S_SIZE = 20; // number of rows in S
    const int MAX_RAND_SMJ = 14; // max value for random integer

    // constant Parallel SMJ
    const size_t SIZE_R = 20;
    const size_t SIZE_S = 20;
    const uint MAX_RAND_PSMJ = 20;

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

}


#endif //SORTMERGEJOIN_TEST_H
