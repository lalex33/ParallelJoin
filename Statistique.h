#ifndef SORTMERGEJOIN_STATISTIQUE_H
#define SORTMERGEJOIN_STATISTIQUE_H

#include "SortMergeJoin.h"

#include <fstream>
#include <cstdio>
#include <ctime>

const int NB_ROWS_MAX = 50000; // maximum value of rows to compute
const int INTEGER_MAX = 100; // maximum value of random integer generated

const string FILE_NAME = "join_stat.csv"; // output file for the results

void launchPerformanceTest();

#endif //SORTMERGEJOIN_STATISTIQUE_H
