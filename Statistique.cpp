#define NDEBUG
#include <cassert>
#include "Statistique.h"

using namespace std;

namespace SMJ {

    void benchmarkSMJ(){

        // init vars
        vector<string> join;

        clock_t start;
        double durationSort;
        double durationMerge;

        // open a file to store results
        ofstream file(FILE_NAME_JOIN, ofstream::out);

        // write settings
        string settings = "MAX INT = " + to_string(INTEGER_MAX) + "\r\n";
        file.write(settings.c_str(), settings.size());

        // check if we have access to the file
        if(!file.fail()){

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
                start = clock();

                // sort the two relation
                sortRelation(R, R + nb_rows);
                sortRelation(S, S + nb_rows);

                durationSort = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                assert(checkSorted(R, nb_rows));
                assert(checkSorted(S, nb_rows));

                start = clock();
                // merge both sorted relation
                mergeRelations(R, R + nb_rows, S, S + nb_rows, join, 0, 0);

                durationMerge = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                assert(checkMerge(R, nb_rows, S, nb_rows, join));

                cout << " DONE in " << to_string(durationSort + durationMerge) << " seconds" << endl;

                // save the result in the file
                string result = to_string(nb_rows) + ";" + to_string(durationSort) + ";" + to_string(durationMerge) + "\r\n";
                file.write(result.c_str(), result.size());

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
        clock_t start;
        double durationSort;
        double durationMerge;

        // open a file to store results
        ofstream file(FILE_NAME_PARALLEL_JOIN, ofstream::out);

        // write settings
        string settings = "NB_THREAD = " + to_string(NB_THREAD) + "; MAX INT = " + to_string(INTEGER_MAX) + "\r\n";
        file.write(settings.c_str(), settings.size());

        // check if we have access to the file
        if(!file.fail()){

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
                start = clock();

                // sort the two relation
                parallelSort(R, nb_rows);
                parallelSort(S, nb_rows);

                durationSort = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                assert(checkSorted(R, nb_rows));
                assert(checkSorted(S, nb_rows));

                start = clock();
                // merge both sorted relation
                join = parallelMerge(R, S, nb_rows, nb_rows);

                durationMerge = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                assert(checkMerge(R, nb_rows, S, nb_rows, assembleResults(join)));

                cout << " DONE in " << to_string(durationSort + durationMerge) << " seconds" << endl;

                // save the result in the file
                string result = to_string(nb_rows) + ";" + to_string(durationSort) + ";" + to_string(durationMerge) + "\r\n";
                file.write(result.c_str(), result.size());

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
        clock_t start;
        double durationSort;
        double durationMerge;

        // open a file to store results
        ofstream file(FILE_NAME_THREAD_PJOIN, ofstream::out);

        // check if we have access to the file
        if(!file.fail()){

            // loop each 10000 rows
            for(uint nbThread = NB_THREAD_MIN; nbThread <= NB_THREAD_MAX; ++nbThread){
                // set number of threads
                NB_THREAD = nbThread;

                // add new random integer into the two relations
                int* R = new int[NB_ROWS_MAX];
                int* S = new int[NB_ROWS_MAX];

                // add new random integer into the two relations
                fillTable(R, NB_ROWS_MAX, INTEGER_MAX);
                fillTable(S, NB_ROWS_MAX, INTEGER_MAX);

                cout << "Computing " << NB_ROWS_MAX << " rows with " << to_string(nbThread) << " threads" << endl;
                // start the chrono
                start = clock();

                // sort the two relation
                parallelSort(R, NB_ROWS_MAX);
                parallelSort(S, NB_ROWS_MAX);

                durationSort = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                assert(checkSorted(R, NB_ROWS_MAX));
                assert(checkSorted(S, NB_ROWS_MAX));

                start = clock();
                // merge both sorted relation
                join = parallelMerge(R, S, NB_ROWS_MAX, NB_ROWS_MAX);

                durationMerge = ( clock() - start ) / (double) CLOCKS_PER_SEC;
                assert(checkMerge(R, NB_ROWS_MAX, S, NB_ROWS_MAX, assembleResults(join)));

                cout << " DONE in " << to_string(durationSort + durationMerge) << " seconds" << endl;

                // save the result in the file
                string result = to_string(nbThread) + ";" + to_string(durationSort) + ";" + to_string(durationMerge) + "\r\n";
                file.write(result.c_str(), result.size());

                // clear tables
                delete[] R;
                delete[] S;
            }

            file.close();
        }else{
            cout << "ERROR : opening file failed" << endl;
        }
    }

}