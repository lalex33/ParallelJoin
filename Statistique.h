#ifndef SORTMERGEJOIN_STATISTIQUE_H
#define SORTMERGEJOIN_STATISTIQUE_H

#include "ParallelSMJ.h"

#include <fstream>
#include <cstdio>
#include <ctime>

namespace SMJ{

    const int NB_ROWS_MAX = 1000000; // maximum value of rows to compute
    const int INTEGER_MAX = INT_MAX; // maximum value of random integer generated

    const std::string FILE_NAME_JOIN = "join_stat.csv"; // output file for the results

    const std::string FILE_NAME_PARALLEL_JOIN = "p_join_stat.csv"; // file for parallel results

    /*
     * start a benchmark of the implemented algorithm of sort-merge-join
     */
    void benchmarkSMJ();

    /*
     * start a benchmark of the parallel SMJ
     */
    void benchmarkParallelSMJ();

}

#endif //SORTMERGEJOIN_STATISTIQUE_H
