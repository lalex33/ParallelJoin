#ifndef SORTMERGEJOIN_STATISTIQUE_H
#define SORTMERGEJOIN_STATISTIQUE_H

#include "Test.h"

const int NB_ROWS_MAX = 2000000;

const int INTEGER_MAX = INT_MAX;
const int INTEGER_MAX_2 = 32767;
const int INTEGER_MAX_3 = 200000000;

const std::string kF_JOIN = "join.csv"; // output file for the results
const std::string kF_PARALLEL_JOIN = "parallel_join.csv"; // file for parallel results
const std::string kF_THREAD_PJOIN = "thread_join.csv"; // file for thread results
const std::string kF_DATA = "data.csv"; // file for data results
const std::string kF_DATA2 = "data2.csv"; // file for data 2 results
const std::string kF_MERGE = "merge.csv"; // file for merge results
const std::string kF_PARALLEL_HASHJOIN = "p_hashjoin.csv"; // file for parallel hash join results
const std::string kF_COMPARISON = "join_comparison.csv"; // file for join algo comparison

const int NB_THREAD_MIN = 1;
const int NB_THREAD_MAX = 16;
const int NB_ROWS_THREAD = 10000000;

const int NB_TRY = 5;
const int NB_TRY_2 = 5;

/*
 * cache miss stats
 */
void benchmarkPAPI();

/*
 * start a benchmark (number of threads) of PSMJ
 */
void benchmarkThreadPSMJ();

/*
 * start a benchmark : parallel hash join
 */
void benchmarkParallelHashJoin();

/*
 * start a benchmark : compare hash join and sort merge join (parallel)
 */
void benchmarkHashVsSMJoin();


#endif //SORTMERGEJOIN_STATISTIQUE_H
