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

        delete[] R;
        delete[] S;
    }

    void sortRelation(int* start, int* end){
        sort(start, end);
    }

    void mergeRelations(int* startR, int* endR, int* startS, int* endS,
                        vector<string>& results, uint rowR, uint rowS){
        uint row_R = rowR, row_S = rowS;

        // loop until there is no tuples to read in both relation
        while(startR != endR && startS != endS){
            if(*startR > *startS){
                // move to a greater tuple in S
                startS++;
                row_S++;
            } else if(*startR < *startS){
                // move to a greater tuple in R
                startR++;
                row_R++;
            } else{
                // two equal tuples found -> record the rows
                results.push_back(getTuple(row_R, startR, row_S, startS));

                // loop on s to find other equal tuples after
                auto tupleS2 = startS + 1;
                auto rowS2 = row_S + 1;

                while(tupleS2 != endS && *tupleS2 == *startR){
                    results.push_back(getTuple(row_R, startR, rowS2, tupleS2));
                    tupleS2++;
                }

                // loop on r to find other equal tuples after
                auto tupleR2 = startR + 1;
                auto rowR2 = row_R + 1;

                while(tupleR2 != endR && *tupleR2 == *startS){
                    results.push_back(getTuple(rowR2, tupleR2, row_S, startS));
                    tupleR2++;
                }

                // go to higher tuples
                startR++;
                row_R++;

                startS++;
                row_S++;
            }
        }
    }

}