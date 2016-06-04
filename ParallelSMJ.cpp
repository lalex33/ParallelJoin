#include "ParallelSMJ.h"

using namespace std;

namespace SMJ{

    uint NB_THREAD;

    vector<Partition> partitionArray(int *table, uint size, uint num_threads){
        vector<Partition> partitions(num_threads);
        uint sizePerThread = size / num_threads;

        for(uint nbThread = 0 ; nbThread < num_threads ; ++nbThread){
            partitions[nbThread].start = table + nbThread * sizePerThread;
            partitions[nbThread].end = partitions[nbThread].start + sizePerThread;
            if(nbThread == num_threads-1){
                partitions[nbThread].end += size % (nbThread * sizePerThread + sizePerThread);
            }
        }

        return partitions;
    }

    void parallelSort(int *table, uint size, ThreadPool& threadPool, const std::vector<Partition> &partitions){
        //double time = sec();

        ostringstream oss;
        oss << parallelMax(table, size, threadPool, partitions);

        //cout << "max : " << (sec() - time) << endl << endl;

        // compute size of the max int
        ulong digitLength = oss.str().size();
        //cout << oss.str() << endl;

        // init a digit_bucket for each thread
        vector<digits_bucket> threadArrays;
        for(int i=0; i<NB_THREAD; ++i){
            digits_bucket digitsBucket(MAX_DIGIT);
            threadArrays.push_back(move(digitsBucket));
        }

        // loop on each digit (from the least to the most)
        for(uint nbDigit = 0 ; nbDigit < digitLength ; ++nbDigit){
            //cout << "one digit :" << endl;
            //time = sec();

            // create <NB_THREAD> threads which will sort a sublist
            for(uint nbThread = 0 ; nbThread < NB_THREAD ; ++nbThread){
                int *start = partitions[nbThread].start, *end = partitions[nbThread].end;
                threadPool.Enqueue( bind(radixSort, start, end, ref(threadArrays[nbThread]), nbDigit) );
            }

            //cout << "   prepare threads : " << (sec() - time) << endl;
            //time = sec();

            threadPool.WaitEndOfWork();

            //cout << "   compute radix : " << (sec() - time) << endl;
            //time = sec();

            // put sorted digit values in array
            int* pos = table;
            for(uint digit=0; digit < MAX_DIGIT; ++digit){
                for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
                    for(auto const &value : threadArrays[nbThread][digit]){
                        *(pos++) = value;
                    }
                    // clear data
                    threadArrays[nbThread][digit].clear();
                }
            }

            //cout << "   store integers : " << (sec() - time) << endl << endl;
        }
    }

    void radixSort(int *start, int *end, digits_bucket &buckets,
                   uint posDigit){
        int mem;
        // loop over each values of the sub-list
        while(start != end){
            mem = *(start++);
            // put the value in the digit bucket
            buckets[ getDigit(mem, posDigit) ].push_back(mem);
        }
    }

    int parallelMax(int *table, uint size, ThreadPool& threadPool, const vector<Partition> &partitions){
        // init vars
        int maxFromThreads[NB_THREAD];

        // compute max by each thread
        for(int nbThread=0; nbThread < NB_THREAD; ++nbThread){
            int* start = partitions[nbThread].start;
            int* end = partitions[nbThread].end;
            auto max_value = &maxFromThreads[nbThread];
            threadPool.Enqueue([start, end, max_value](){
                *max_value = *max_element(start, end);
            });
        }

        threadPool.WaitEndOfWork();

        // find max of threads' max
        return *max_element(maxFromThreads, maxFromThreads + NB_THREAD);
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

    vector<vector<string>> parallelMerge3(ThreadPool& threadPool, int* R, int* S,
                                          uint sizeR, uint sizeS, const vector<Partition> &partitions){
        // allocate merge result for each thread
        vector<vector<string>> results(NB_THREAD);
        uint rowsPerThread = sizeR / NB_THREAD;

        // start <NB_THREAD> threads for merge
        for(uint nbThread=0; nbThread < NB_THREAD; ++nbThread){
            threadPool.Enqueue( bind(mergeRelations, partitions[nbThread].start, partitions[nbThread].end, S, S + sizeS,
                                     ref(results[nbThread]), nbThread*rowsPerThread, 0) );
        }

        // wait end of all merges
        threadPool.WaitEndOfWork();

        return results;
    }

    vector<vector<string>> parallelMerge4(ThreadWork& threadWork, int *R, int *S, uint sizeR,
                                                         uint sizeS) {
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