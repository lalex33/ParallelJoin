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
        // init vars
        vector<vector<string>> join;
        double start, durationSort, durationMerge;

        // open a file to store results
        ofstream file(FILE_NAME_PARALLEL_JOIN, ofstream::out);

        // check if we have access to the file
        if(!file.fail()){
            file << "Number of rows;Parallel sort;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;
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
                parallelSort(R, nb_rows);
                parallelSort(S, nb_rows);

                durationSort = sec() - start;
                assert(checkSorted(R, nb_rows));
                assert(checkSorted(S, nb_rows));

                start = sec();
                // merge both sorted relation
                join = parallelMerge(R, S, nb_rows, nb_rows);

                durationMerge = sec() - start;
                assert(checkMerge(R, nb_rows, S, nb_rows, assembleResults(join)));

                cout << " DONE in " << (durationSort + durationMerge) << " seconds" << endl;

                // save the result in the file
                file << nb_rows << ";" << durationSort << ";" << durationMerge << "\r\n";

                // clear tables
                delete[] R;
                delete[] S;
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkThreadPSMJ() {
        // init vars
        vector<vector<string>> join;
        double start, durationSort, durationMerge;

        // open a file to store results
        ofstream file(FILE_NAME_THREAD_PJOIN, ofstream::out);

        // check if we have access to the file
        if(!file.fail()){
            file << "Number of thread;Parallel sort;Parallel merge;Number of rows : " << NB_ROWS_THREAD << "Integer range : 0-" << INTEGER_MAX_2 << endl;
            // loop each 10000 rows
            for(uint nbThread = NB_THREAD_MIN; nbThread <= NB_THREAD_MAX; ++nbThread){
                // set number of threads
                NB_THREAD = nbThread;

                // add new random integer into the two relations
                int* R = new int[NB_ROWS_THREAD];
                int* S = new int[NB_ROWS_THREAD];

                // add new random integer into the two relations
                fillTable(R, NB_ROWS_THREAD, INTEGER_MAX_2);
                fillTable(S, NB_ROWS_THREAD, INTEGER_MAX_2);

                cout << "Computing " << NB_ROWS_THREAD << " rows with " << nbThread << " threads" << endl;
                // start the chrono
                start = sec();

                // sort the two relation
                parallelSort(R, NB_ROWS_THREAD);
                parallelSort(S, NB_ROWS_THREAD);

                durationSort = sec() - start;
                assert(checkSorted(R, NB_ROWS_THREAD));
                assert(checkSorted(S, NB_ROWS_THREAD));

                start = sec();
                // merge both sorted relation
                join = parallelMerge(R, S, NB_ROWS_THREAD, NB_ROWS_THREAD);

                durationMerge = sec() - start;
                assert(checkMerge(R, NB_ROWS_THREAD, S, NB_ROWS_THREAD, assembleResults(join)));

                cout << " DONE in " << (durationSort + durationMerge) << " seconds" << endl;

                // save the result in the file
                file << nbThread << ";" << durationSort << ";" << durationMerge << "\r\n";

                // clear tables
                delete[] R;
                delete[] S;
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

                fillTable(R, nbRows, INTEGER_MAX_2);
                start = sec();
                sort(R, R + nbRows);
                d_stdSort = sec() - start;

                fillTable(S, nbRows, INTEGER_MAX_2);
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
        double start, d_stdSort, d_parallelRadix, d_merge, d_parallelMerge;
        NB_THREAD = 24;
        vector<string> results;
        ofstream file(FILE_NAME_DATA2, ofstream::out);

        if(!file.fail()){
            file << "Number of rows;std::sort;Simple merge;Radix sort;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;
            for(uint nbRows = 1000000; nbRows <= 10000000; nbRows += 1000000){
                cout << "Computing " << nbRows << endl;
                int* R = new int[nbRows];
                int* S = new int[nbRows];

                fillTable(R, nbRows, INTEGER_MAX);
                start = sec();
                sort(R, R + nbRows);
                d_stdSort = sec() - start;
                cout << checkSorted(R, nbRows) << endl;

                fillTable(S, nbRows, INTEGER_MAX);
                start = sec();
                parallelSort(S, nbRows);
                d_parallelRadix = sec() - start;
                cout << checkSorted(S, nbRows) << endl;

                start = sec();
                mergeRelations(R, R + nbRows, S, S + nbRows, results, 0, 0);
                d_merge = sec() - start;
                results.clear();

                start = sec();
                parallelMerge(R, S, nbRows, nbRows);
                d_parallelMerge = sec() - start;

                file << nbRows << ";" << d_stdSort << ";" << d_merge << ";" << d_parallelRadix << ";" << d_parallelMerge << "\r\n";

                delete[] R;
                delete[] S;
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

    void benchmarkMerge() {
        double start, d_merge, d_parallelMerge;
        NB_THREAD = 10;
        vector<string> results;
        ofstream file(FILE_NAME_MERGE, ofstream::out);

        if(!file.fail()){
            file << "Number of rows;Simple merge;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX_2 << endl;
            for(uint nbRows = 10000; nbRows <= 300000; nbRows += 10000){
                cout << "Computing " << nbRows << endl;
                int* R = new int[nbRows];
                int* S = new int[nbRows];
                fillTable(R, nbRows, INTEGER_MAX_2);
                fillTable(S, nbRows, INTEGER_MAX_2);
                parallelSort(R, nbRows);
                parallelSort(S, nbRows);

                start = sec();
                mergeRelations(R, R + nbRows, S, S + nbRows, results, 0, 0);
                d_merge = sec() - start;
                results.clear();

                start = sec();
                parallelMerge(R, S, nbRows, nbRows);
                d_parallelMerge = sec() - start;

                file << nbRows << ";" << d_merge << ";" << d_parallelMerge << "\r\n";

                delete[] R;
                delete[] S;
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

}