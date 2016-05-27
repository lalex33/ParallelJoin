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

    std::vector<int> array{1,2,3,4,5,6,7,8,9,10};

    PartitionedArray<int> partitionedArray(array, 3);

    printTable(partitionedArray.GetPartition(0), partitionedArray.GetPartitionSize(0));
    printTable(partitionedArray.GetPartition(1), partitionedArray.GetPartitionSize(1));
    printTable(partitionedArray.GetPartition(2), partitionedArray.GetPartitionSize(2));

    return 0;
}