#include "Test.h"

using namespace std;

namespace SMJ{

    void testParallelJoin(){
        srand(time(NULL));

        int* R = new int[SIZE_R];
        int* S = new int[SIZE_S];

        fillTable(R, SIZE_R, MAX_RAND_PSMJ);
        fillTable(S, SIZE_S, MAX_RAND_PSMJ);

        parallelSort(R, SIZE_R);
        parallelSort(S, SIZE_S);

        auto results = assembleResults(parallelMerge(R, S, SIZE_R, SIZE_S));

        printSortMerge(R, S, SIZE_R, SIZE_S, results);

        cout << "array merged ? " << (checkMerge(R, SIZE_R, S, SIZE_S, results)?"yes":"no") << endl;

        delete[] R;
        delete[] S;
    }

    void testParallelSort(bool printSort){
        srand(time(NULL));

        int* R = new int[SIZE_R];
        fillTable(R, SIZE_R, MAX_RAND_PSMJ);

        clock_t start = clock();
        parallelSort(R, SIZE_R);
        cout << (( clock() - start ) / (double) CLOCKS_PER_SEC) << endl;

        cout << "array sorted ? " << (checkSorted(R, SIZE_R)?"yes":"no") << endl;
        if(printSort) {
            for (int i = 0; i < SIZE_R; i++) {
                cout << R[i] << "  <=  ";
                if(i%10 == 9) cout << endl;
            }
        }

        delete[] R;
    }

    void testSortMergeJoin() {
        srand(time(NULL)); // generate a seed

        int* R = new int[R_SIZE];
        int* S = new int[S_SIZE];
        vector<string> result;

        // initialize relations with random data
        fillTable(R, R_SIZE, MAX_RAND_SMJ);
        fillTable(S, S_SIZE, MAX_RAND_SMJ);

        // sort the two relations
        sortRelation(R, R + R_SIZE);
        sortRelation(S, S + S_SIZE);

        // merge the two relations
        mergeRelations(R, R + R_SIZE, S, S + S_SIZE, result, 0, 0);

        // print datas
        printSortMerge(R, S, R_SIZE, S_SIZE, result);

        cout << "is merged? -> " << (checkMerge(R, R_SIZE, S, S_SIZE, result)?"yes":"no") << endl;

        delete[] R;
        delete[] S;
    }

    void testLotOfData(){
        srand(time(NULL));

        int* R = new int[MAX_SIZE];
        int* S = new int[MAX_SIZE];
        vector<vector<string>> result;

        double start;

        fillTable(R, MAX_SIZE, MAX_RAND_TEST);
        fillTable(S, MAX_SIZE, MAX_RAND_TEST);

        cout << "Start sort" << endl;
        start = sec();
        parallelSort(R, MAX_SIZE);
        cout << "Sorting R with parallel radix : " << (sec() - start) << " seconds" << endl;
        cout << "Sorted : " << checkSorted(R, MAX_SIZE) << endl;

        cout << "Start sort" << endl;
        start = sec();
        sort(S, S + MAX_SIZE);
        cout << "Sorting S with std::sort : " << (sec() - start) << " seconds" << endl;
        cout << "Sorted : " << checkSorted(S, MAX_SIZE) << endl;

        delete[] R;
        delete[] S;
    }

}

