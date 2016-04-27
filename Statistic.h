#ifndef SORTMERGEJOIN_STATISTIQUE_H
#define SORTMERGEJOIN_STATISTIQUE_H

#include "Test.h"

namespace SMJ{

    const int NB_ROWS_MAX = 2000000; // maximum value of rows to compute
    // some maximum values of random integer generated
    const int INTEGER_MAX = INT_MAX;
    const int INTEGER_MAX_2 = 32767;
    const int INTEGER_MAX_3 = 20000000;

    const std::string FILE_NAME_JOIN = "join.csv"; // output file for the results
    const std::string FILE_NAME_PARALLEL_JOIN = "parallel_join.csv"; // file for parallel results
    const std::string FILE_NAME_THREAD_PJOIN = "thread_join.csv"; // file for thread results
    const std::string FILE_NAME_DATA = "data.csv"; // file for data results
    const std::string FILE_NAME_DATA2 = "data2.csv"; // file for data 2 results
    const std::string FILE_NAME_MERGE = "merge.csv"; // file for merge results

    const int NB_THREAD_MIN = 1;
    const int NB_THREAD_MAX = 10;
    const int NB_ROWS_THREAD = 1000000;

    /*
     * start a benchmark of the implemented algorithm of sort-merge-join
     */
    void benchmarkSMJ();

    /*
     * start a benchmark of the parallel SMJ
     */
    void benchmarkParallelSMJ();

    /*
     * start a benchmark (number of threads) of PSMJ
     */
    void benchmarkThreadPSMJ();

    /*
     * start a benchmark of increasing size of data (sort)
     */
    void benchmarkData();

    /*
     * start a benchmark of increasing size of data (sort and merge)
     */
    void benchmarkData2();

    /*
     * start a benchmark : comparison between simple and parallel merge
     */
    void benchmarkMerge();
}

#endif //SORTMERGEJOIN_STATISTIQUE_H
