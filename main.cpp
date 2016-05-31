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
    /*ParallelHashJoin parallelHashJoin(10000, 10000, 100);
    parallelHashJoin.InitTables(100000);
    parallelHashJoin.HashJoinOnR();
    parallelHashJoin.PrintResult();*/

    ParallelHashJoin parallelHashJoin1(10000, 10000, 100, 2);
    parallelHashJoin1.InitTables(100000);
    parallelHashJoin1.ParallelHashJoinOnR();
    parallelHashJoin1.PrintResult();

    return 0;
}