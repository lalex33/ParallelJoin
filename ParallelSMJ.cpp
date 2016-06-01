#include "ParallelSMJ.h"

using namespace std;

namespace SMJ{

    uint NB_THREAD;

    void parallelSort(int *table, uint size, ThreadPool& threadPool){
        double start = sec();

        ostringstream oss;
        oss << parallelMax(table, size, threadPool);

        cout << "max : " << (sec() - start) << endl << endl;

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
            cout << "one digit :" << endl;
            start = sec();

            // create <NB_THREAD> threads which will sort a sublist
            for(uint nbThread = 0 ; nbThread < NB_THREAD ; ++nbThread){
                int *start = table + nbThread * sizePerThread;
                int *end = start + sizePerThread;
                end += (nbThread == NB_THREAD-1)? (size % (nbThread * sizePerThread + sizePerThread)):0;
                threadPool.Enqueue( bind(radixSort, start, end, ref(threadArrays[nbThread]), nbDigit) );
            }

            cout << "   prepare threads : " << (sec() - start) << endl;
            start = sec();

            threadPool.WaitEndOfWork();

            cout << "   compute radix : " << (sec() - start) << endl;
            start = sec();

            // put sorted digit values in array
            int* pos = table;
            for(uint digit=0; digit < MAX_DIGIT_EXCLUDED; ++digit){
                for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
                    auto array = &threadArrays[nbThread][digit];
                    for(auto value = array->begin(); value != array->end(); ++value){
                        *(pos++) = *value;
                    }
                    // clear data
                    array->clear();
                }
            }

            cout << "   store integers : " << (sec() - start) << endl << endl;
        }
    }

    void radixSort(int *start, int *end, digits_bucket &buckets,
                   uint posDigit){
        // loop over each values of the sub-list
        while(start != end){
            // put the value in the digit bucket
            buckets[ getDigit(*start, posDigit) ].push_back(*start);
            ++start;
        }
    }

    int parallelMax(int *table, uint size, ThreadPool& threadPool){
        // init vars
        vector<thread> threads;
        uint rowsPerThread = size / NB_THREAD;
        int maxFromThreads[NB_THREAD];

        // compute max by each thread
        for(int nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* start = table + nbThread * rowsPerThread;
            int* end = start + rowsPerThread;
            end += (nbThread == NB_THREAD-1)? (size % (nbThread * rowsPerThread + rowsPerThread)):0;
            threadPool.Enqueue( bind(maxRoutine, start, end, &maxFromThreads[nbThread]) );
        }

        threadPool.WaitEndOfWork();

        // find max of threads' max
        int* start = maxFromThreads, *end = start + NB_THREAD, max = *(start++);
        while(start != end){
            if(*start > max)
                max = *start;
            ++start;
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

        // compute the number of rows merged by a thread
        uint rowsPerThread = sizeR / NB_THREAD;

        // start <NB_THREAD> threads for merge
        for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* startR = R + nbThread*rowsPerThread;
            int* endR = startR + rowsPerThread;
            endR += (nbThread == NB_THREAD-1)? (sizeR % (nbThread * rowsPerThread + rowsPerThread)):0;

            int* startS = S, *endS = S + sizeS;

            double start = sec();
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
        threadPool.WaitEndOfWork();

        return results;
    }

    vector<vector<string>> parallelMerge4(ThreadWork& threadWork, int *R, int *S, uint sizeR,
                                                         uint sizeS) {
        double start = sec();

        // allocate merge result for each thread
        vector<vector<string>> results(NB_THREAD);

        // compute the number of rows merged by a thread
        uint rowsPerThread = sizeR / NB_THREAD;

        // add <NB_THREAD> tasks for merge
        for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* startR = R + nbThread*rowsPerThread;
            int* endR = startR + rowsPerThread;
            endR += (nbThread == NB_THREAD-1)? (sizeR % (nbThread * rowsPerThread + rowsPerThread)):0;

            int* startS = S, *endS = S + sizeS;
            threadWork.AddTask( bind(mergeRelations, startR, endR, startS, endS,
                                  ref(results[nbThread]), nbThread*rowsPerThread, 0) );
        }

        /*cout << "      prepare : " << (sec() - start) << endl;
        start = sec();*/

        // launch all merge
        threadWork.LaunchWork();

        // wait end of all merges
        threadWork.WaitEndOfWork();

        //cout << "      compute : " << (sec() - start) << endl;
        return results;
    }

    vector<vector<string>> parallelMerge5(int *R, int *S, uint sizeR, uint sizeS) {
        vector<vector<string>> results(NB_THREAD);

        uint rowsPerThread = sizeR / NB_THREAD;
        uint partitionS = sizeS / NB_THREAD;

        for(uint rotation=0; rotation < NB_THREAD; ++rotation) {
            ThreadWork threadWork((u_int8_t) NB_THREAD);

            for (uint nbThread = 0; nbThread < NB_THREAD; ++nbThread) {
                int *startR = R + nbThread * rowsPerThread;
                int *endR = startR + rowsPerThread;
                endR += (nbThread == NB_THREAD - 1) ? (sizeR % (nbThread * rowsPerThread + rowsPerThread)) : 0;

                int* startS = S + partitionS * ((nbThread + rotation)%NB_THREAD);
                int* endS = startS + partitionS;
                endS += (nbThread + rotation == NB_THREAD - 1)?
                        (sizeS % (nbThread * partitionS + partitionS)) : 0;

                threadWork.AddTask( bind(mergeRelations, startR, endR, startS, endS,
                                                    ref(results[nbThread]), nbThread * rowsPerThread,
                                                    partitionS * ((nbThread + rotation)%NB_THREAD)));
            }

            double start = sec();
            threadWork.LaunchWork();
            threadWork.WaitEndOfWork();
            cout << "       compute (rotation=" << rotation << ") : " << (sec() - start) << endl;
        }

        return results;
    }

    vector<vector<string>> parallelMerge6(ThreadPool &threadPool, PartitionedArray<int>& R,
                                          PartitionedArray<int>& S) {
        vector<vector<string>> results(NB_THREAD);

        for(uint rotation=0; rotation < NB_THREAD; ++rotation) {
            for (uint nbThread = 0; nbThread < NB_THREAD; ++nbThread) {
                int* startR = R.GetPartition(nbThread);
                int* endR = startR + R.GetPartitionSize(nbThread);

                int* startS = S.GetPartition((nbThread + rotation) % NB_THREAD);
                int* endS = startS + S.GetPartitionSize((nbThread + rotation) % NB_THREAD);

                threadPool.Enqueue( bind(mergeRelations, startR, endR, startS, endS,
                                         ref(results[nbThread]), nbThread * R.GetPartitionSize(0),
                                         S.GetPartitionSize(0) * ((nbThread + rotation)%NB_THREAD)));
            }

            double start = sec();
            threadPool.WaitEndOfWork();
            cout << "       compute (rotation=" << rotation << ") : " << (sec() - start) << endl;
        }

        return results;
    }

}