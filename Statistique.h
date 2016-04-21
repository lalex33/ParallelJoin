#ifndef SORTMERGEJOIN_STATISTIQUE_H
#define SORTMERGEJOIN_STATISTIQUE_H

#include "Test.h"

namespace SMJ{

    const int NB_ROWS_MAX = 2000000; // maximum value of rows to compute
    const int INTEGER_MAX = INT_MAX; // maximum value of random integer generated

    const std::string FILE_NAME_JOIN = "join.csv"; // output file for the results
    const std::string FILE_NAME_PARALLEL_JOIN = "parallel_join.csv"; // file for parallel results
    const std::string FILE_NAME_THREAD_PJOIN = "thread_join.csv"; // file for thread results

    const int NB_THREAD_MIN = 1;
    const int NB_THREAD_MAX = 24;

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
     * get time
     * https://github.com/iwiwi/parallel-radix-sort/blob/master/measure.cc
     */
    double sec();

}

#endif //SORTMERGEJOIN_STATISTIQUE_H
