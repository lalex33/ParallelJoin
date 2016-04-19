//
// Created by Alexis bayle on 19/04/2016.
//

#ifndef SORTMERGEJOIN_TEST_H
#define SORTMERGEJOIN_TEST_H

#include "ParallelSMJ.h"

namespace SMJ{

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
