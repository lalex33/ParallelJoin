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

        for(int p = 1; p <= NB_POW; p++){
            int nb_rows = powInteger(10, p);

            R.clear();
            S.clear();

            initRandomData(R, INTEGER_MAX, nb_rows);
            initRandomData(S, INTEGER_MAX, nb_rows);

            cout << "Starting compute " << nb_rows << endl;
            start = std::clock();

            sortRelation(R);
            sortRelation(S);

            join = mergeRelations(R, S);

            duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            cout << nb_rows << " DONE : " << duration << endl;

            string result = to_string(nb_rows) + ";" + to_string(duration) + "\r\n";
            file.write(result.c_str(), result.size());
        }

        file.close();
    }else{
        cout << "error at file opening" << endl;
    }
}

int powInteger(int number, int exponent){
    if(exponent == 0){
        return 1;
    }else{
        return number * powInteger(number, exponent-1);
    }
}