#include "SortMergeJoin.h"

using namespace std;

namespace SMJ{

    void testSortMergeJoin() {
        srand(time(NULL)); // generate a seed

        int* R = new int[R_SIZE];
        int* S = new int[S_SIZE];
        vector<string> result;

        // initialize relations with random data
        fillTable(R, R_SIZE, MAX_RAND_SMJ);
        fillTable(S, S_SIZE, MAX_RAND_SMJ);

        // sort the two relations
        sortRelation(R, R + R_SIZE);
        sortRelation(S, S + S_SIZE);

        // merge the two relations
        mergeRelations(R, R + R_SIZE, S, S + S_SIZE, result, 0, 0);

        // print datas
        printSortMerge(R, S, R_SIZE, S_SIZE, result);

        //cout << "is merged? -> " << (checkMerge(R, R_SIZE, S, S_SIZE, result)?"yes":"no") << endl;

        delete[] R;
        delete[] S;
    }

    void sortRelation(int* start, int* end){
        sort(start, end);
    }

    void mergeRelations(int* startR, int* endR, int* startS, int* endS,
                        vector<string>& results, uint rowR, uint rowS){
        int *tupleR = startR, *tupleS = startS;

        // loop until there is no tuples to read in both relation
        while(tupleR != endR && tupleS != endS){
            if(*tupleR > *tupleS){
                // move to a greater tuple in S
                tupleS++;
                rowS++;
            } else if(*tupleR < *tupleS){
                // move to a greater tuple in R
                tupleR++;
                rowR++;
            } else{
                // two equal tuples found -> record the rows
                results.push_back(getTuple(rowR, tupleR, rowS, tupleS));

                // loop on s to find other equal tuples after
                auto tupleS2 = tupleS + 1;
                auto rowS2 = rowS + 1;

                while(tupleS2 != endS && *tupleS2 == *tupleR){
                    results.push_back(getTuple(rowR, tupleR, rowS2++, tupleS2++));
                }

                // loop on r to find other equal tuples after
                auto tupleR2 = tupleR + 1;
                auto rowR2 = rowR + 1;

                while(tupleR2 != endR && *tupleR2 == *tupleS){
                    results.push_back(getTuple(rowR2++, tupleR2++, rowS, tupleS));
                }

                // go to higher tuples
                tupleR++;
                rowR++;

                tupleS++;
                rowS++;
            }
        }
    }

}