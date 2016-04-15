#include <iostream>
#include "ParallelSMJ.h"

using namespace std;

void testParallelJoin(){
    srand(time(NULL));

    int* R = new int[SIZE_R];
    int* S = new int[SIZE_S];

    fillTable(R, SIZE_R, MAX_RAND_PSMJ);
    fillTable(S, SIZE_S, MAX_RAND_PSMJ);

    parallelSort(R, SIZE_R);
    parallelSort(S, SIZE_S);

    auto result = parallelMerge(R, S, SIZE_R, SIZE_S);

    cout << "--- R ---" << endl;
    printTable(R, SIZE_R);
    cout << "--- S ---" << endl;
    printTable(S, SIZE_S);
    cout << "   R    |    S" << endl;
    for(auto merge : result){
        for(auto value : merge){
            cout << value << endl;
        }
    }

    delete[] R;
    delete[] S;
}

void testParallelSort(bool printSort){
    srand(time(NULL));

    int* R = new int[SIZE_R];
    fillTable(R, SIZE_R, MAX_RAND_PSMJ);

    clock_t start = clock();
    parallelSort(R, SIZE_R);
    cout << (( clock() - start ) / (double) CLOCKS_PER_SEC) << endl;

    cout << "array sorted ? " << (checkSorted(R, SIZE_R)?"yes":"no") << endl;
    if(printSort) {
        for (int i = 0; i < SIZE_R; i++) {
            cout << R[i] << "  <=  ";
            if(i%10 == 9) cout << endl;
        }
    }

    delete[] R;
}

void fillTable(int* table, size_t size, uint maxValue){
    for(int i=0; i<size; i++){
        table[i] = rand() % maxValue;
    }
}

void parallelSort(int* table, int size){
    // compute size of the max int
    ulong digitLength = to_string(max(table, size)).length();

    // compute number of int to sort by a thread
    uint sizePerThread = size / NB_THREAD;

    // init a digit_bucket for each thread
    vector<digits_bucket> threadArrays;
    for(int n=0; n<NB_THREAD; n++){
        digits_bucket digitsBucket(MAX_DIGIT_EXCLUDED);
        threadArrays.push_back(digitsBucket);
    }

    vector<thread> threads;

    // loop on each digit (from the least to the most)
    for(ulong nbDigit = 0 ; nbDigit < digitLength ; ++nbDigit){

        // create <NB_THREAD> threads which will sort a sublist
        for(uint nbThread = 0 ; nbThread < NB_THREAD ; ++nbThread){
            threads.push_back( thread(radixSort, table, ref(threadArrays[nbThread]), nbDigit,
                                      nbThread*sizePerThread, nbThread*sizePerThread + sizePerThread ));
        }

        // wait end of sort
        for(auto& thread : threads){
            thread.join();
        }

        // put sorted digit values in array
        int* pos = table;
        for(uint digit=0; digit < MAX_DIGIT_EXCLUDED; ++digit){
            for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
                for(auto value : threadArrays[nbThread][digit]){
                    *pos = value;
                    ++pos;
                }
            }
        }

        // clear temporary arrays
        for(auto &digitBucket : threadArrays){
            for(auto &bucket : digitBucket){
                bucket.clear();
            }
        }

        // delete threads used for the sort
        threads.clear();
    }
}

void radixSort(int* table, digits_bucket& buckets,
               ulong posDigit, uint start, uint end){
    // loop over each values of the sub-list
    for(uint n = start; n < end; n++){
        // put the value in the digit bucket
        buckets[ getDigit(table[n], posDigit) ].push_back(table[n]);
    }
}

int max(int* table, int size){
    int *end = table + size, *p = table, *max = p;
    while(p != end){
        if(*p > *max)
            max = p;
        p++;
    }
    return *max;
}

int getDigit (int number, int pos)
{
    return (pos == 0) ? number % 10 : getDigit (number/10, --pos);
}

bool checkSorted(int* table, int size){
    int* p = table;
    int* end = table + size - 1;
    while(p != end){
        if(*p > *(p+1))
            return false;
        p++;
    }
    return true;
}

vector<vector<string>> parallelMerge(int* R, int* S, uint sizeR, uint sizeS){
    // allocate merge result for each thread
    vector<vector<string>> results(NB_THREAD);
    vector<thread> threads;

    // compute the number of rows merge by a thread
    uint rowsPerThread = sizeR / NB_THREAD;

    // start <NB_THREAD> threads to merge
    for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
        int* startR = R + nbThread*rowsPerThread;
        int* endR = R + nbThread*rowsPerThread + rowsPerThread;
        int* startS = S, *endS = S + sizeS;

        threads.push_back( thread(mergeRoutine, startR, endR, startS, endS,
                                  ref(results[nbThread]), nbThread*rowsPerThread, 0) );
    }

    // wait end of all merges
    for(auto& thread : threads){
        thread.join();
    }

    return results;
}

void mergeRoutine(int* startR, int* endR, int* startS, int* endS,
           vector<string>& results, uint rowR, uint rowS){
    uint row_R = rowR, row_S = rowS;
    string match;

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
            match = "  " + to_string(row_R) + "     |    " + to_string(row_S);
            results.push_back(match);

            // loop on s to find other equal tuples after
            auto tupleS2 = startS + 1;
            auto rowS2 = row_S + 1;

            while(tupleS2 != endS && *tupleS2 == *startR){
                match = "  " + to_string(row_R) + "     |    " + to_string(rowS2);
                results.push_back(match);
                tupleS2++;
            }

            // loop on r to find other equal tuples after
            auto tupleR2 = startR + 1;
            auto rowR2 = row_R + 1;

            while(tupleR2 != endR && *tupleR2 == *startS){
                match = "  " + to_string(rowR2) + "     |    " + to_string(row_S);
                results.push_back(match);
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

void printTable(int* table, int size){
    int *p = table;
    int *end = table + size;
    while(p != end){
        cout << *p << endl;
        p++;
    }
}