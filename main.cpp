#include "Statistic.h"
#include "PartitionedArray.hpp"

using namespace SMJ;

int main(){

    srand(time(NULL));

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merge between two tables
    //benchmarkSMJ();

    // set number of thread
    // NB_THREAD < NB_ROWS
    NB_THREAD = 4;

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

    return 0;
}