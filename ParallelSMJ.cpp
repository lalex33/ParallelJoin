#include "ParallelSMJ.h"

using namespace std;

namespace SMJ{

    uint NB_THREAD;

    void parallelSort(int *table, uint size){
        ostringstream oss;
        oss << parallelMax(table, size);

        // compute size of the max int
        ulong digitLength = oss.str().size();

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
        for(uint nbDigit = 0 ; nbDigit < digitLength ; ++nbDigit){

            // create <NB_THREAD> threads which will sort a sublist
            for(uint nbThread = 0 ; nbThread < NB_THREAD ; ++nbThread){
                uint start = nbThread * sizePerThread;
                uint end = start + sizePerThread;
                end += (nbThread == NB_THREAD-1)? (size%end):0;
                threads.push_back( thread(radixSort, table, ref(threadArrays[nbThread]), nbDigit, start, end));
            }

            // wait end of sort and delete threads
            for(auto thread = threads.begin(); thread != threads.end(); thread++){
                thread->join();
            }
            threads.clear();

            // put sorted digit values in array
            int* pos = table;
            for(uint digit=0; digit < MAX_DIGIT_EXCLUDED; ++digit){
                for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
                    auto array = &threadArrays[nbThread][digit];
                    for(auto value = array->begin(); value != array->end(); value++){
                        *(pos++) = *value;
                    }
                    // clear data
                    array->clear();
                }
            }
        }
    }

    void radixSort(int *table, digits_bucket &buckets,
                   uint posDigit, uint start, uint end){
        // loop over each values of the sub-list
        for(uint n = start; n < end; n++){
            // put the value in the digit bucket
            buckets[ getDigit(table[n], posDigit) ].push_back(table[n]);
        }
    }

    int parallelMax(int *table, uint size){
        // init vars
        vector<thread> threads;
        uint rowsPerThread = size / NB_THREAD;
        int* maxFromThreads = new int[NB_THREAD];

        // compute max by each thread
        for(int nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* start = table + nbThread * rowsPerThread;
            int* end = start + rowsPerThread;
            end += (nbThread == NB_THREAD-1)? (size % (nbThread * rowsPerThread + rowsPerThread)):0;
            threads.push_back( thread(maxRoutine, start, end, maxFromThreads+nbThread) );
        }

        // wait the end of threads
        for(auto thread = threads.begin(); thread != threads.end(); thread++){
            thread->join();
        }
        threads.clear();

        // find max of threads' max
        int* start = maxFromThreads, *end = start + NB_THREAD, max = *(start++);
        while(start != end){
            if(*start > max)
                max = *start;
            start++;
        }

        // free memory
        delete[] maxFromThreads;

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

        // compute the number of rows merged by a thread
        uint rowsPerThread = sizeR / NB_THREAD;

        // start <NB_THREAD> threads for merge
        for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* startR = R + nbThread*rowsPerThread;
            int* endR = startR + rowsPerThread;
            endR += (nbThread == NB_THREAD-1)? (sizeR % (nbThread * rowsPerThread + rowsPerThread)):0;

            int* startS = S, *endS = S + sizeS;

            threads.push_back( thread(mergeRelations, startR, endR, startS, endS,
                                      ref(results[nbThread]), nbThread*rowsPerThread, 0) );
        }

        // wait end of all merges
        for(auto thread = threads.begin(); thread != threads.end(); ++thread){
            thread->join();
        }
        threads.clear();

        return results;
    }

    vector<vector<string>> parallelMerge2(int* R, int* S, uint sizeR, uint sizeS){
        // allocate merge result for each thread
        vector<vector<string>> results(NB_THREAD);
        vector<thread> threads;

        // compute the number of rows merged by a thread
        uint rowsPerThread = sizeR / NB_THREAD;
        uint partitionS = sizeS / NB_THREAD;

        // non-blocking way to scan S
        for(uint rotation=0; rotation < NB_THREAD; ++rotation) {

            // start <NB_THREAD> threads for merge
            for (uint nbThread = 0; nbThread < NB_THREAD; ++nbThread) {
                int *startR = R + nbThread * rowsPerThread;
                int *endR = startR + rowsPerThread;
                endR += (nbThread == NB_THREAD - 1) ? (sizeR % (nbThread * rowsPerThread + rowsPerThread)) : 0;

                int* startS = S + partitionS * ((nbThread + rotation)%NB_THREAD);
                int* endS = startS + partitionS;
                endS += (nbThread + rotation == NB_THREAD - 1)?
                        (sizeS % (nbThread * partitionS + partitionS)) : 0;

                threads.push_back(thread(mergeRelations, startR, endR, startS, endS,
                                         ref(results[nbThread]), nbThread * rowsPerThread,
                                         partitionS * ((nbThread + rotation)%NB_THREAD)));
            }

            // wait end of all merges
            for (auto thread = threads.begin(); thread != threads.end(); ++thread) {
                thread->join();
            }
            threads.clear();
        }

        return results;
    }

    vector<vector<string>> parallelMerge3(ThreadPool& threadPool, int* R, int* S, uint sizeR, uint sizeS){
        // allocate merge result for each thread
        vector<vector<string>> results(NB_THREAD);

        // compute the number of rows merged by a thread
        uint rowsPerThread = sizeR / NB_THREAD;

        // start <NB_THREAD> threads for merge
        for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* startR = R + nbThread*rowsPerThread;
            int* endR = startR + rowsPerThread;
            endR += (nbThread == NB_THREAD-1)? (sizeR % (nbThread * rowsPerThread + rowsPerThread)):0;

            int* startS = S, *endS = S + sizeS;
            threadPool.Enqueue( bind(mergeRelations, startR, endR, startS, endS,
                                     ref(results[nbThread]), nbThread*rowsPerThread, 0) );
        }

        // wait end of all merges
        while( !threadPool.IsWorkFinished() ){}
        threadPool.ShutDown();

        return results;
    }

    vector<string> assembleResults(vector<vector<string>> results){
        vector<string> result;
        for(auto match = results.begin(); match != results.end(); ++match){
            for(auto value = match->begin(); value != match->end(); ++value){
                result.push_back(*value);
            }
        }
        return result;
    }

}