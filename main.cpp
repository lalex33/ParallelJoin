#include "Statistique.h"

using namespace SMJ;

int main(){

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merge between two tables
    //benchmarkSMJ();

    // set number of thread
    NB_THREAD = 4;

    // shows example of parallel sort-merge join
    //testParallelSort(false);
    //testParallelJoin();

    // launch test of parallel sort-merge
    //benchmarkParallelSMJ();
    benchmarkThreadPSMJ();

    return 0;
}