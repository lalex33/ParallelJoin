#include "Statistique.h"

using namespace std;

void launchPerformanceTest(){

    vector<int> R;
    vector<int> S;
    vector<string> join;

    ofstream file(FILE_NAME, ofstream::out);

    if(!file.fail()){
        std::clock_t start;
        double duration;

        for(int nb_rows = 5000; nb_rows <= NB_ROWS_MAX; nb_rows += 5000){
            R.clear();
            S.clear();

            initRandomData(R, INTEGER_MAX, nb_rows);
            initRandomData(S, INTEGER_MAX, nb_rows);

            cout << "Computing " << nb_rows << " rows" << endl;
            start = std::clock();

            sortRelation(R);
            sortRelation(S);

            join = mergeRelations(R, S);

            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            cout << " DONE in " << duration << " seconds" << endl;

            string result = to_string(nb_rows) + ";" + to_string(duration) + "\r\n";
            file.write(result.c_str(), result.size());
        }

        file.close();
    }else{
        cout << "error at file opening" << endl;
    }
}