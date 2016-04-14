//
// Created by Alexis bayle on 13/04/2016.
//

#ifndef SORTMERGEJOIN_SORTMERGEJOIN_H
#define SORTMERGEJOIN_SORTMERGEJOIN_H

#include <iostream>
#include <vector>

using namespace std;

// constant
const int R_SIZE = 10; // number of rows in R
const int S_SIZE = 10; // number of rows in S
const int MAX_VALUE = 20; // max value for random integer

void testSortMergeJoin();

void printRelation(const vector<int> &relation);

void printSortMerge(const vector<int> &r, const vector<int> &s, const vector<string> &result);

void initRandomData(vector<int> &relation, int maxValue, int nbRows);

void sortRelation(vector<int> &relation);

vector<string> mergeRelations(const vector<int> &r, const vector<int> &s);

#endif //SORTMERGEJOIN_SORTMERGEJOIN_H
