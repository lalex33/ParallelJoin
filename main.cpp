#include "Statistic1.h"
#include "ParallelRadixSort.hpp"

using namespace SMJ;

int main(){
    // generate a seed for random values
    srand(time(NULL));

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

    //benchmarkSort();

    //testLotOfData();

    //benchmarkData();
    //benchmarkData2();
    //benchmarkMerge();

    // launch test of parallel sort-merge
    //benchmarkParallelSMJ();
    //benchmarkThreadPSMJ();

    //benchmarkMergeThread();

    /*
     * Parallel radix sort, second version
     */
    //testParallelRadixSort();
    //radixSortMillionElements();

    /*
     * Hash Join
     */
    //benchmarkParallelHashJoin();
    //testParallelHashJoin();

    /*
     * Hash Join vs Sort Merge Join
     */
    //benchmarkHashVsSMJoin();

    /*
     * papi cache miss
     */
    benchmarkPAPI();

    return 0;
}