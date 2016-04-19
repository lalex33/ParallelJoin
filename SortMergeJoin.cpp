#include "SortMergeJoin.h"

using namespace std;

namespace SMJ{

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