#include <iostream>
#include "ParallelSMJ.h"

using namespace std;

namespace SMJ{

    void testParallelJoin(){
        srand(time(NULL));

        int* R = new int[SIZE_R];
        int* S = new int[SIZE_S];

        fillTable(R, SIZE_R, MAX_RAND_PSMJ);
        fillTable(S, SIZE_S, MAX_RAND_PSMJ);

        parallelSort(R, SIZE_R);
        parallelSort(S, SIZE_S);

        auto results = parallelMerge(R, S, SIZE_R, SIZE_S);

        vector<string> result;
        for(auto match : results){
            for(auto value : match){
                result.push_back(value);
            }
        }

        printSortMerge(R, S, SIZE_R, SIZE_S, result);

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

    void parallelSort(int* table, size_t size){
        // compute size of the max int
        ulong digitLength = to_string(parallelMax(table, size)).length();

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

    int parallelMax(int* table, size_t size){
        // init vars
        vector<thread> threads;
        uint rowsPerThread = size / NB_THREAD;
        int* maxFromThreads = new int[NB_THREAD];

        // compute max by each thread
        for(int nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* start = table + nbThread * rowsPerThread;
            int* end = start + rowsPerThread;

            threads.push_back( thread(maxRoutine, start, end, maxFromThreads+nbThread) );
        }

        // wait the end of thread
        for(auto& thread : threads){
            thread.join();
        }
        threads.clear();

        // find max of threads' max
        int max = maxFromThreads[0];
        for(int n=1; n<NB_THREAD; ++n){
            if(maxFromThreads[n] > max){
                max = maxFromThreads[n];
            }
        }

        return max;
    }

    void maxRoutine(int* start, int* end, int* max){
        *max = *(start++);
        while(start != end){
            if(*start > *max){
                *max = *start;
            }
            start++;
        }
    }

    int getDigit (int number, int pos) {
        return (pos == 0) ? number % 10 : getDigit (number/10, --pos);
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

            threads.push_back( thread(mergeRelations, startR, endR, startS, endS,
                                      ref(results[nbThread]), nbThread*rowsPerThread, 0) );
        }

        // wait end of all merges
        for(auto& thread : threads){
            thread.join();
        }

        return results;
    }

}