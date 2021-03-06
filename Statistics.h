#ifndef SORTMERGEJOIN_STATISTIC1_H
#define SORTMERGEJOIN_STATISTIC1_H

#include "Statistic.h"

using namespace std;
using namespace SMJ;

void benchmarkSort() {
    NB_THREAD = 8;
    double start;
    ofstream file(kF_PARALLEL_SORT, ofstream::out);
    ThreadPool threadPool(NB_THREAD);

    if(!file.fail()){
        file << "Number of rows;std::sort;1st radix sort;2nd radix sort;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;
        for(uint nbRows = 1000000; nbRows <= 20000000; nbRows += 1000000){
            cout << "Computing " << nbRows << endl;

            double avg_std_sort = 0.0, avg_sort_1 = 0.0, avg_sort_2 = 0.0;
            for(int i = 0; i < 1; ++i){
                int* R = new int[nbRows];
                auto pR = partitionArray(R, nbRows, NB_THREAD);

                fillTable(R, nbRows, INTEGER_MAX);
                start = sec();
                std::sort(R, R + nbRows);
                avg_std_sort += sec() - start;

                fillTable(R, nbRows, INTEGER_MAX);
                start = sec();
                parallelSort(R, nbRows, threadPool, pR);
                avg_sort_1 += sec() - start;

                fillTable(R, nbRows, INTEGER_MAX);
                start = sec();
                ParallelRadixSort(R, nbRows, threadPool, pR, NB_THREAD);
                avg_sort_2 += sec() - start;

                delete[] R;
            }

            file << nbRows << ";" << (avg_std_sort/NB_TRY_2) << ";" << (avg_sort_1/NB_TRY_2) << ";" << (avg_sort_2/NB_TRY_2) << "\r\n";
        }
        file.close();
    }else{
        cout << "ERROR : opening file failed" << endl;
    }
}

/*
 * start a benchmark : comparison between simple and parallel merge
 */
void benchmarkMerge() {
    NB_THREAD = 4;
    double start;
    vector<string> results;
    ofstream file(kF_MERGE, ofstream::out);
    ThreadPool threadPool(NB_THREAD);

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

                auto pR = partitionArray(R, nbRows, NB_THREAD);
                auto pS = partitionArray(S, nbRows, NB_THREAD);

                parallelSort(R, nbRows, threadPool, pR);
                parallelSort(S, nbRows, threadPool, pS);

                start = sec();
                mergeRelations(R, R + nbRows, S, S + nbRows, results, 0, 0);
                avg_merge += sec() - start;
                results.clear();

                start = sec();
                auto join = parallelMerge(threadPool, R, S, nbRows, nbRows, pR);
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

/*
 * start a benchmark of the implemented algorithm of sort-merge-join
 */
void benchmarkSMJ(){
    // init vars
    vector<string> join;
    double start, durationSort, durationMerge;

    // open a file to store results
    ofstream file(kF_JOIN, ofstream::out);

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

/*
 * start a benchmark of the parallel SMJ
 */
void benchmarkParallelSMJ(){
    vector<vector<string>> join;
    double start, durationSort, durationMerge;
    ofstream file(kF_PARALLEL_JOIN, ofstream::out);

    if(!file.fail()){
        file << "Number of rows;Parallel sort;Parallel merge;Number of threads : " << NB_THREAD << ";Integer range : 0-" << INTEGER_MAX << endl;

        for(int nb_rows = 10000; nb_rows <= NB_ROWS_MAX; nb_rows += 10000){
            int* R = new int[nb_rows];
            int* S = new int[nb_rows];

            auto part_r = partitionArray(R, nb_rows, NB_THREAD);
            auto part_s = partitionArray(S, nb_rows, NB_THREAD);

            fillTable(R, nb_rows, INTEGER_MAX);
            fillTable(S, nb_rows, INTEGER_MAX);

            ThreadPool threadPool(NB_THREAD);
            cout << "Computing " << nb_rows << " rows" << endl;
            start = sec();
            parallelSort(R, nb_rows, threadPool, part_r);
            parallelSort(S, nb_rows, threadPool, part_s);
            durationSort = sec() - start;
            assert(checkSorted(R, nb_rows));
            assert(checkSorted(S, nb_rows));

            start = sec();
            join = parallelMerge(threadPool, R, S, nb_rows, nb_rows, part_r);
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

/*
 * start a benchmark of increasing size of data (sort)
 */
void benchmarkData() {
    double start, d_stdSort, d_parallelRadix;
    NB_THREAD = 24;
    ofstream file(kF_DATA, ofstream::out);

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
            ThreadPool threadPool(NB_THREAD);
            start = sec();
            parallelSort(S, nbRows, threadPool, partitionArray(S, nbRows, NB_THREAD));
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

#endif //SORTMERGEJOIN_STATISTIC1_H
