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

        //double start = sec();

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

                // loop on r to find other equal tuples after
                tupleR2 = tupleR + 1;
                rowR2 = rowR + 1;

                while(tupleR2 != endR && *tupleR2 == *tupleS){
                    results.push_back(getTuple(rowR2++, tupleR2++, rowS, tupleS));
                    //rowR2++;tupleR2++;
                }

                // loop on s to find other equal tuples after
                tupleS2 = tupleS + 1;
                rowS2 = rowS + 1;

                while(tupleS2 != endS && *tupleS2 == *tupleR){
                    results.push_back(getTuple(rowR, tupleR, rowS2++, tupleS2++));
                    //rowS2++;tupleS2++;
                }

                // go to higher tuples
                tupleR++;
                rowR++;

                tupleS++;
                rowS++;
            }
        }

        //cout << "merge " << (sec() - start) << " on " << this_thread::get_id() << endl;
    }

}