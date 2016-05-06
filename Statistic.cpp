#define NDEBUG
#include "Statistic.h"

using namespace std;

namespace SMJ {

    void benchmarkSMJ(){
        // init vars
        vector<string> join;
        double start, durationSort, durationMerge;

        // open a file to store results
        ofstream file(FILE_NAME_JOIN, ofstream::out);

        // write settings
        file << "MAX INT = " << INTEGER_MAX << "\r\n";

        // check if we have access to the file
        if(!file.fail()){
            file << "Number of rows;Sort;Merge;Integer range : 0-" << INTEGER_MAX << endl;
            // loop each 10000 rows
            for(int nb_rows = 10000; nb_rows <= NB_ROWS_MAX; nb_rows += 10000){
                // add new random integer into the two relations
                int* R = new int[nb_rows];
                int* S = new int[nb_rows];

                // add new random integer into the two relations
                fillTable(R, nb_rows, INTEGER_MAX);
                fillTable(S, nb_rows, INTEGER_MAX);

                cout << "Computing " << nb_rows << " rows" << endl;
                // start the chrono
                start = sec();

                // sort the two relation
                sortRelation(R, R + nb_rows);
                sortRelation(S, S + nb_rows);

                durationSort = sec() - start;
                assert(checkSorted(R, nb_rows));
                assert(checkSorted(S, nb_rows));

                start = sec();
                // merge both sorted relation
                mergeRelations(R, R + nb_rows, S, S + nb_rows, join, 0, 0);

                durationMerge = sec() - start;
                assert(checkMerge(R, nb_rows, S, nb_rows, join));

                cout << " DONE in " << (durationSort + durationMerge) << " seconds" << endl;

                // save the result in the file
                file << nb_rows << ";" << durationSort << ";" << durationMerge << "\r\n";

                // clear tables
                delete[] R;
                delete[] S;
            }

            // close the file
            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkParallelSMJ(){
        vector<vector<string>> join;
        double start, durationSort, durationMerge;
        ofstream file(FILE_NAME_PARALLEL_JOIN, ofstream::out);

        if(!file.fail()){
            file << "Number of rows;Parallel sort;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;

            for(int nb_rows = 10000; nb_rows <= NB_ROWS_MAX; nb_rows += 10000){
                int* R = new int[nb_rows];
                int* S = new int[nb_rows];

                fillTable(R, nb_rows, INTEGER_MAX);
                fillTable(S, nb_rows, INTEGER_MAX);

                cout << "Computing " << nb_rows << " rows" << endl;
                start = sec();
                parallelSort(R, nb_rows);
                parallelSort(S, nb_rows);
                durationSort = sec() - start;
                assert(checkSorted(R, nb_rows));
                assert(checkSorted(S, nb_rows));

                start = sec();
                join = parallelMerge(R, S, nb_rows, nb_rows);
                durationMerge = sec() - start;
                assert(checkMerge(R, nb_rows, S, nb_rows, assembleResults(join)));

                cout << " DONE in " << (durationSort + durationMerge) << " seconds" << endl;
                file << nb_rows << ";" << durationSort << ";" << durationMerge << "\r\n";

                delete[] R;
                delete[] S;
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkThreadPSMJ() {
        vector<vector<string>> join;
        double start;
        ofstream file(FILE_NAME_THREAD_PJOIN, ofstream::out);
        //ThreadPool threadPool(NB_THREAD);

        if(!file.fail()){
            file << "Number of thread;Parallel sort;Parallel merge;Number of rows : " << NB_ROWS_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;

            for(uint nbThread = NB_THREAD_MIN; nbThread <= NB_THREAD_MAX; ++nbThread){
                NB_THREAD = nbThread;

                cout << "Computing " << NB_ROWS_THREAD << " rows with " << nbThread << " threads" << endl;

                double avg_sort = 0.0, avg_merge = 0.0;

                //threadPool.Resize(nbThread);
                for(int i = 0; i < NB_TRY; ++i){

                    int* R = new int[NB_ROWS_THREAD];
                    int* S = new int[NB_ROWS_THREAD];

                    fillTable(R, NB_ROWS_THREAD, INTEGER_MAX);
                    fillTable(S, NB_ROWS_THREAD, INTEGER_MAX);

                    start = sec();
                    parallelSort(R, NB_ROWS_THREAD);
                    parallelSort(S, NB_ROWS_THREAD);
                    avg_sort += sec() - start;

                    ThreadPool threadPool(nbThread);
                    start = sec();
                    join = parallelMerge3(threadPool, R, S, NB_ROWS_THREAD, NB_ROWS_THREAD);
                    avg_merge += sec() - start;

                    delete[] R;
                    delete[] S;
                }

                cout << " DONE in " << (avg_sort/NB_TRY) << " and " << (avg_merge/NB_TRY) << " seconds" << endl;
                file << nbThread << ";" << (avg_sort/NB_TRY) << ";" << (avg_merge/NB_TRY) << "\r\n";
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkData() {
        double start, d_stdSort, d_parallelRadix;
        NB_THREAD = 24;
        ofstream file(FILE_NAME_DATA, ofstream::out);

        if(!file.fail()){
            file << "Number of rows;std::sort;Radix sort;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX_2 << endl;
            for(uint nbRows = 1000000; nbRows <= 100000000; nbRows += 1000000){
                cout << "Computing " << nbRows << endl;
                int* R = new int[nbRows];
                int* S = new int[nbRows];

                fillTable(R, nbRows, INTEGER_MAX);
                start = sec();
                sort(R, R + nbRows);
                d_stdSort = sec() - start;

                fillTable(S, nbRows, INTEGER_MAX);
                start = sec();
                parallelSort(S, nbRows);
                d_parallelRadix = sec() - start;

                file << nbRows << ";" << d_stdSort << ";" << d_parallelRadix << "\r\n";

                delete[] R;
                delete[] S;
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkData2() {
        double start;
        NB_THREAD = 48;
        vector<string> results;
        ofstream file(FILE_NAME_DATA2, ofstream::out);

        if(!file.fail()){
            file << "Number of rows;std::sort;Simple merge;Radix sort;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;
            for(uint nbRows = 1000000; nbRows <= 20000000; nbRows += 1000000){
                cout << "Computing " << nbRows << endl;
                double d_stdSort = 0.0, d_parallelRadix = 0.0, d_merge = 0.0, d_parallelMerge = 0.0;

                for(int i=0; i < NB_TRY; i++){
                    int *R = new int[nbRows];
                    int *S = new int[nbRows];

                    fillTable(R, nbRows, INTEGER_MAX_3);
                    start = sec();
                    sort(R, R + nbRows);
                    d_stdSort += sec() - start;

                    fillTable(S, nbRows, INTEGER_MAX_3);
                    start = sec();
                    parallelSort(S, nbRows);
                    d_parallelRadix += sec() - start;

                    start = sec();
                    mergeRelations(R, R + nbRows, S, S + nbRows, results, 0, 0);
                    d_merge += sec() - start;
                    results.clear();

                    start = sec();
                    auto join = parallelMerge(R, S, nbRows, nbRows);
                    d_parallelMerge += sec() - start;
                    join.clear();

                    delete[] R;
                    delete[] S;
                }

                file << nbRows << ";" << (d_stdSort/NB_TRY) << ";" << (d_merge/NB_TRY) <<
                        ";" << (d_parallelRadix/NB_TRY) << ";" << (d_parallelMerge/NB_TRY) << "\r\n";
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkMerge() {
        double start;
        vector<string> results;
        ofstream file(FILE_NAME_MERGE, ofstream::out);

        if(!file.fail()){
            file << "Number of rows;Simple merge;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX_3 << endl;
            for(uint nbRows = 1000000; nbRows <= 20000000; nbRows += 1000000){
                cout << "Computing " << nbRows << endl;

                double avg_merge = 0.0, avg_pmerge = 0.0;
                for(int i = 0; i < NB_TRY_2; ++i){
                    int* R = new int[nbRows];
                    int* S = new int[nbRows];
                    fillTable(R, nbRows, INTEGER_MAX_3);
                    fillTable(S, nbRows, INTEGER_MAX_3);
                    parallelSort(R, nbRows);
                    parallelSort(S, nbRows);

                    start = sec();
                    mergeRelations(R, R + nbRows, S, S + nbRows, results, 0, 0);
                    avg_merge += sec() - start;
                    results.clear();

                    start = sec();
                    auto join = parallelMerge(R, S, nbRows, nbRows);
                    avg_pmerge += sec() - start;

                    delete[] R;
                    delete[] S;
                }

                file << nbRows << ";" << (avg_merge/NB_TRY_2) << ";" << (avg_pmerge/NB_TRY_2) << "\r\n";
            }
            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

}