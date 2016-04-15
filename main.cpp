#include "Statistique.h"

int main(){

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merge between two tables
    //benchmarkSMJ();

    // possible use of Table when it will be fully implemented
    //testTableSortMergeJoin();

    // shows example of parallel sort-merge join
    //testParallelSort(true);
    testParallelJoin();

    return 0;
}