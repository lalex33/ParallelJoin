#ifndef SORTMERGEJOIN_STATISTIQUE_H
#define SORTMERGEJOIN_STATISTIQUE_H

#include "SortMergeJoin.h"

#include <fstream>
#include <cstdio>
#include <ctime>

const int NB_POW = 5;
const int INTEGER_MAX = 100;

const string FILE_NAME = "join_stat.csv";

void launchPerformanceTest();

int powInteger(int number, int exponent);

#endif //SORTMERGEJOIN_STATISTIQUE_H
