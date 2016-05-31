#include "Statistic.h"
#include "ParallelHashJoin.hpp"

using namespace SMJ;

int main(){
    srand((unsigned int) time(NULL));

    // set default number of thread
    NB_THREAD = 4;

    /*
     * Sort Merge Join
     */

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merge between two tables
    //benchmarkSMJ();

    // shows example of parallel sort-merge join
    //testParallelSort(false);
    //testParallelJoin();

    //testLotOfData();

    //benchmarkData();
    //benchmarkData2();
    //benchmarkMerge();

    // launch test of parallel sort-merge
    //benchmarkParallelSMJ();
    //benchmarkThreadPSMJ();

    //benchmarkMergeThread();


    /*
     * Hash Join
     */
    ParallelHashJoin parallelHashJoin(100000, 100000, 5000);
    parallelHashJoin.HashJoinOnR(INT_MAX);
    parallelHashJoin.PrintResult();

    return 0;
}