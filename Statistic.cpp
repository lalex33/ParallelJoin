#define NDEBUG

#include "Statistic.h"
#include "ParallelHashJoin.hpp"

using namespace std;
using namespace SMJ;

void benchmarkThreadPSMJ() {
    vector<vector<string>> join;
    double start;
    ofstream file(kF_THREAD_PJOIN, ofstream::out);

    if(!file.fail()){
        file << "Number of thread;Parallel sort;Parallel merge;Number of rows : " << NB_ROWS_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;

        int* R1 = new int[NB_ROWS_THREAD];
        int* S1 = new int[NB_ROWS_THREAD];
        fillTable(R1, NB_ROWS_THREAD, INTEGER_MAX);
        fillTable(S1, NB_ROWS_THREAD, INTEGER_MAX);

        #ifdef __linux__
            const int NB_EVENTS = 4;
            int events[NB_EVENTS] = {PAPI_L1_DCM, PAPI_L2_DCM, PAPI_L2_DCA, PAPI_L1_LDM};
            int ret;
            long_long values[NB_EVENTS];

            if (PAPI_num_counters() < NB_EVENTS) {
                fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
                exit(1);
            }
            if ((ret = PAPI_start_counters(events, NB_EVENTS)) != PAPI_OK) {
                fprintf(stderr, "PAPI failed to start counters: %s\n", PAPI_strerror(ret));
                exit(1);
            }
        #endif

        for(uint nbThread = NB_THREAD_MIN; nbThread <= NB_THREAD_MAX; ++nbThread){
            NB_THREAD = nbThread;
            ThreadPool threadPool(nbThread);

            cout << "--> Computing " << NB_ROWS_THREAD << " rows with " << nbThread << " threads" << endl;

            double avg_sort = 0.0, avg_merge = 0.0;

            for(int i = 0; i < NB_TRY; ++i){
                int* R = new int[NB_ROWS_THREAD];
                int* S = new int[NB_ROWS_THREAD];

                copy(R1, R1 + NB_ROWS_THREAD, R);
                copy(S1, S1 + NB_ROWS_THREAD, S);

                start = sec();
                auto partitionsR = partitionArray(R, NB_ROWS_THREAD, NB_THREAD);
                auto partitionsS = partitionArray(S, NB_ROWS_THREAD, NB_THREAD);
                parallelSort(R, NB_ROWS_THREAD, threadPool, partitionsR);
                parallelSort(S, NB_ROWS_THREAD, threadPool, partitionsS);
                avg_sort += sec() - start;

                cout << "R sorted? = " << checkSorted(R, NB_ROWS_THREAD) << endl;
                cout << "S sorted? = " << checkSorted(S, NB_ROWS_THREAD) << endl;

                start = sec();
                join = parallelMerge3(threadPool, R, S, NB_ROWS_THREAD, NB_ROWS_THREAD, partitionsR);
                avg_merge += sec() - start;

                #ifdef __linux__
                    if ((ret = PAPI_read_counters(values, NB_EVENTS)) != PAPI_OK) {
                        fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
                        exit(1);
                    }
                    for(int i=0; i<NB_EVENTS; ++i){
                        cout << "values[" << i << "] = " << values[i] << endl;
                    }
                    cout << "Miss rate : " << (((double)values[1]/values[2])*100) << endl;
                #endif

                join.clear();
                delete[] R;
                delete[] S;
            }

            cout << "  DONE in " << (avg_sort/NB_TRY) << " and " << (avg_merge/NB_TRY) << " seconds" << endl;
            file << nbThread << ";" << (avg_sort/NB_TRY) << ";" << (avg_merge/NB_TRY) << "\r\n";
        }

        delete[] R1;
        delete[] S1;
        file.close();
    }else{
        cout << "ERROR : opening file failed" << endl;
    }
}

void benchmarkParallelHashJoin() {
    ofstream file(kF_PARALLEL_HASHJOIN, ofstream::out);

    if(!file.fail()){
        file << "Number of thread;Parallel hash join;Number of rows : " << NB_ROWS_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;

        for(uint nbThread = NB_THREAD_MIN; nbThread <= NB_THREAD_MAX; ++nbThread){
            NB_THREAD = nbThread;

            cout << "--> Computing " << NB_ROWS_THREAD << " rows with " << nbThread << " threads" << endl;

            double avg_merge = 0.0;

            for(int i = 0; i < NB_TRY_2; ++i){
                ParallelHashJoin parallelHashJoin(NB_ROWS_THREAD, NB_ROWS_THREAD, 5000000, nbThread);
                parallelHashJoin.InitTables(INTEGER_MAX);

                parallelHashJoin.ParallelHashJoinOnR();

                avg_merge += parallelHashJoin.GetProcessingTime();
            }

            cout << "  DONE in " << (avg_merge/NB_TRY_2) << " seconds" << endl;
            file << nbThread << ";" << (avg_merge/NB_TRY_2) << "\r\n";
        }

        file.close();
    }else{
        cout << "ERROR : opening file failed" << endl;
    }
}

void benchmarkHashVsSMJoin() {
    ofstream file(kF_COMPARISON, ofstream::out);
    double start = 0.0;
    const int num_buckets = NB_ROWS_THREAD * 2;

    if(!file.fail()){
        file << "Number of thread;Parallel hash join;Parallel SMJ;Number of rows : " << NB_ROWS_THREAD << ";Integer range : 0-" << INTEGER_MAX
        << "; number of buckets : " << num_buckets << endl;

        for(uint nbThread = NB_THREAD_MIN; nbThread <= NB_THREAD_MAX; ++nbThread){
            NB_THREAD = nbThread;

            cout << "--> Computing " << NB_ROWS_THREAD << " rows with " << nbThread << " threads" << endl;

            double avg_merge_hash = 0.0, avg_merge_smj = 0.0;

            for(int i = 0; i < NB_TRY_2; ++i){
                // hash join
                cout << "Hash join :" << endl;
                ParallelHashJoin parallelHashJoin(NB_ROWS_THREAD, NB_ROWS_THREAD, num_buckets, nbThread);
                parallelHashJoin.InitTables(INTEGER_MAX);
                parallelHashJoin.ParallelHashJoinOnR();
                avg_merge_hash += parallelHashJoin.GetProcessingTime();
                cout << "hash = " << parallelHashJoin.GetHashTime() << endl;
                cout << "join = " << parallelHashJoin.GetJoinTime() << endl;
            }

            for(int i = 0; i < NB_TRY_2; ++i) {
                // sort merge join
                cout << "Sort merge join :" << endl;
                int *R = new int[NB_ROWS_THREAD];
                int *S = new int[NB_ROWS_THREAD];
                fillTable(R, NB_ROWS_THREAD, INTEGER_MAX);
                fillTable(S, NB_ROWS_THREAD, INTEGER_MAX);

                ThreadPool threadPool(nbThread);
                auto partitionsR = partitionArray(R, NB_ROWS_THREAD, nbThread);
                auto partitionsS = partitionArray(S, NB_ROWS_THREAD, nbThread);

                start = sec();
                //parallelSort(R, NB_ROWS_THREAD, threadPool, partitionsR);
                ParallelRadixSort(R, NB_ROWS_THREAD, threadPool, partitionsR, nbThread);
                avg_merge_smj += sec() - start;
                cout << "sort R = " << (sec() - start) << endl;

                start = sec();
                //parallelSort(S, NB_ROWS_THREAD, threadPool, partitionsS);
                ParallelRadixSort(S, NB_ROWS_THREAD, threadPool, partitionsS, nbThread);
                avg_merge_smj += sec() - start;
                cout << "sort S = " << (sec() - start) << endl;

                cout << "sorted ? " << checkSorted(R, NB_ROWS_THREAD) << " , " << checkSorted(S, NB_ROWS_THREAD) << endl;

                start = sec();
                parallelMerge3(threadPool, R, S, NB_ROWS_THREAD, NB_ROWS_THREAD, partitionsR);
                avg_merge_smj += sec() - start;
                cout << "merge = " << (sec() - start) << endl;

                delete[] R;
                delete[] S;
            }

            cout << "  DONE in " << (avg_merge_hash/NB_TRY_2) << " seconds and " << (avg_merge_smj/NB_TRY_2) << " seconds." << endl;
            file << nbThread << ";" << (avg_merge_hash/NB_TRY_2) << ";" << (avg_merge_smj/NB_TRY_2) << "\r\n";
        }

        file.close();
    }else{
        cout << "ERROR : opening file failed" << endl;
    }
}


