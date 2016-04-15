#include "Statistique.h"

using namespace std;

void benchmarkSMJ(){

    // init vars
    vector<int> R;
    vector<int> S;
    vector<string> join;

    clock_t start;
    double durationSort;
    double durationMerge;

    // open a file to store results
    ofstream file(FILE_NAME_JOIN, ofstream::out);

    // check if we have access to the file
    if(!file.fail()){

        // loop each 10000 rows
        for(int nb_rows = 10000; nb_rows <= NB_ROWS_MAX; nb_rows += 10000){
            // clear data
            R.clear();
            S.clear();

            // add new random integer into the two relations
            initRandomData(R, INTEGER_MAX, nb_rows);
            initRandomData(S, INTEGER_MAX, nb_rows);

            cout << "Computing " << nb_rows << " rows" << endl;
            // start the chrono
            start = clock();

            // sort the two relation
            sortRelation(R);
            sortRelation(S);

            durationSort = ( clock() - start ) / (double) CLOCKS_PER_SEC;

            start = clock();
            // merge both sorted relation
            join = mergeRelations(R, S);

            durationMerge = ( clock() - start ) / (double) CLOCKS_PER_SEC;
            cout << " DONE in " << (durationSort + durationMerge) << " seconds" << endl;

            // save the result in the file
            string result = to_string(nb_rows) + ";" + to_string(durationSort) + ";" + to_string(durationMerge) + "\r\n";
            file.write(result.c_str(), result.size());
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

            fillTable(R, nb_rows, INTEGER_MAX);
            fillTable(S, nb_rows, INTEGER_MAX);

            cout << "Computing " << nb_rows << " rows" << endl;
            // start the chrono
            start = clock();

            // sort the two relation
            parallelSort(R, nb_rows);
            parallelSort(S, nb_rows);

            durationSort = ( clock() - start ) / (double) CLOCKS_PER_SEC;

            start = clock();
            // merge both sorted relation
            join = parallelMerge(R, S, nb_rows, nb_rows);

            durationMerge = ( clock() - start ) / (double) CLOCKS_PER_SEC;
            cout << " DONE in " << (durationSort + durationMerge) << " seconds" << endl;

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