#include "Statistique.h"

using namespace std;

/*
 * start a benchmark of the implemented algorithm of sort-merge-join
 */
void launchPerformanceTest(){

    // init vars
    vector<int> R;
    vector<int> S;
    vector<string> join;

    clock_t start;
    double duration;

    // open a file to store results
    ofstream file(FILE_NAME, ofstream::out);

    // check if we have access to the file
    if(!file.fail()){

        // loop each 5000 rows
        for(int nb_rows = 5000; nb_rows <= NB_ROWS_MAX; nb_rows += 5000){
            // clear data
            R.clear();
            S.clear();

            // add new random integer into the two relations
            initRandomData(R, INTEGER_MAX, nb_rows);
            initRandomData(S, INTEGER_MAX, nb_rows);

            cout << "Computing " << nb_rows << " rows" << endl;
            // start the chrono
            start = std::clock();

            // sort the two relation
            sortRelation(R);
            sortRelation(S);

            // merge both sorted relation
            join = mergeRelations(R, S);

            // end the chrono
            duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
            cout << " DONE in " << duration << " seconds" << endl;

            // save the result in the file
            string result = to_string(nb_rows) + ";" + to_string(duration) + "\r\n";
            file.write(result.c_str(), result.size());
        }

        // close the file
        file.close();
    }else{
        cout << "ERROR : opening file failed" << endl;
    }
}