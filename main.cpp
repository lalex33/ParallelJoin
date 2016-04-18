#include "Statistique.h"

using namespace SMJ;

int main(){

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merge between two tables
    //benchmarkSMJ();

    // possible use of Table when it will be fully implemented
    //testTableSortMergeJoin();

    // set number of thread
    NB_THREAD = 4;

    // shows example of parallel sort-merge join
    //testParallelSort(true);
    //testParallelJoin();

    // launch test of parallel sort-merge
    //benchmarkParallelSMJ();
    benchmarkThreadPSMJ();

    return 0;
}