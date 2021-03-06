#include "SortMergeJoin.h"

using namespace std;

namespace SMJ{

    void sortRelation(int* start, int* end){
        sort(start, end);
    }

    void mergeRelations(int* startR, int* endR, int* startS, int* endS,
                        vector<string>& results, uint rowR, uint rowS){
        int *tupleR = startR, *tupleS = startS;
        int *tupleR2, rowR2, *tupleS2, rowS2;
        int valueR, valueS, valueR2, valueS2;

        tupleS = lower_bound(startS, endS, *startR);
        endS = upper_bound(startS, endS, *(endR-1));

        // loop until there is no tuples to read in both relation
        while(tupleR != endR && tupleS != endS){
            valueR = *tupleR;
            valueS = *tupleS;

            if(valueR > valueS){
                // move to a greater tuple in S
                ++tupleS;
                ++rowS;
            } else if(valueR < valueS){
                // move to a greater tuple in R
                ++tupleR;
                ++rowR;
            } else{
                // two equal tuples found -> record the rows
                results.push_back(getTuple(rowR, valueR, rowS, valueS));

                // loop on r to find other equal tuples after
                tupleR2 = tupleR + 1;
                rowR2 = rowR + 1;

                while(tupleR2 != endR && *tupleR2 == *tupleS){
                    valueR2 = *tupleR2;
                    results.push_back(getTuple(rowR2++, valueR2, rowS, valueS));
                    ++tupleR2;
                }

                // loop on s to find other equal tuples after
                tupleS2 = tupleS + 1;
                rowS2 = rowS + 1;

                while(tupleS2 != endS && *tupleS2 == *tupleR){
                    valueS2 = *tupleS2;
                    results.push_back(getTuple(rowR, valueR, rowS2++, valueS2));
                    ++tupleS2;
                }

                // go to higher tuples
                ++tupleR;
                ++rowR;

                ++tupleS;
                ++rowS;
            }
        }
    }

}