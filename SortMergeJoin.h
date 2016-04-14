#ifndef SORTMERGEJOIN_SORTMERGEJOIN_H
#define SORTMERGEJOIN_SORTMERGEJOIN_H

#include <iostream>
#include <vector>

using namespace std;

// constant
const int R_SIZE = 10; // number of rows in R
const int S_SIZE = 10; // number of rows in S
const int MAX_RANDOM_VALUE = 20; // max value for random integer

typedef vector<int> Relation;

void testSortMergeJoin();

void printRelation(const Relation &relation);

void printSortMerge(const Relation &r, const Relation &s, const vector<string> &result);

void initRandomData(Relation &relation, int maxValue, int nbRows);

void sortRelation(Relation &relation);

vector<string> mergeRelations(const Relation &r, const Relation &s);

#endif //SORTMERGEJOIN_SORTMERGEJOIN_H
