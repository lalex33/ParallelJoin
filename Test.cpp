#include "Test.h"

using namespace std;

namespace SMJ{

    void testParallelJoin(){
        NB_THREAD = 2;

        int* R = new int[P_SIZE_R];
        int* S = new int[P_SIZE_S];

        fillTable(R, P_SIZE_R, MAX_RAND_PSMJ);
        fillTable(S, P_SIZE_S, MAX_RAND_PSMJ);

        ThreadPool threadPool(NB_THREAD);
        auto pR = partitionArray(R, P_SIZE_R, NB_THREAD);
        auto pS = partitionArray(S, P_SIZE_S, NB_THREAD);
        parallelSort(R, P_SIZE_R, threadPool, pR);
        parallelSort(S, P_SIZE_S, threadPool, pS);

        auto results = assembleResults(parallelMerge3(threadPool, R, S, P_SIZE_R, P_SIZE_S, pR));

        printSortMerge(R, S, P_SIZE_R, P_SIZE_S, results);

        cout << "array merged ? " << (checkMerge(R, P_SIZE_R, S, P_SIZE_S, results)?"yes":"no") << endl;

        delete[] R;
        delete[] S;
    }

    void testParallelSort(bool printSort){
        srand(time(NULL));

        int* R = new int[P_SIZE_R];
        fillTable(R, P_SIZE_R, MAX_RAND_PSMJ);

        ThreadPool threadPool(NB_THREAD);

        clock_t start = clock();
        parallelSort(R, P_SIZE_R, threadPool, partitionArray(R, P_SIZE_R, NB_THREAD));
        cout << (( clock() - start ) / (double) CLOCKS_PER_SEC) << endl;

        cout << "array sorted ? " << (checkSorted(R, P_SIZE_R)?"yes":"no") << endl;
        if(printSort) {
            for (int i = 0; i < P_SIZE_R; i++) {
                cout << R[i] << "  <=  ";
                if(i%10 == 9) cout << endl;
            }
        }

        delete[] R;
    }

    void testSortMergeJoin() {
        srand(time(NULL)); // generate a seed

        int* R = new int[SIZE_R];
        int* S = new int[SIZE_S];
        vector<string> result;

        // initialize relations with random data
        fillTable(R, SIZE_R, MAX_RAND_SMJ);
        fillTable(S, SIZE_S, MAX_RAND_SMJ);

        // sort the two relations
        sortRelation(R, R + SIZE_R);
        sortRelation(S, S + SIZE_S);

        // merge the two relations
        mergeRelations(R, R + SIZE_R, S, S + SIZE_S, result, 0, 0);

        // print datas
        printSortMerge(R, S, SIZE_R, SIZE_S, result);

        cout << "is merged? -> " << (checkMerge(R, SIZE_R, S, SIZE_S, result)?"yes":"no") << endl;

        delete[] R;
        delete[] S;
    }

    void testLotOfData(){
        srand(time(NULL));
        ThreadPool threadPool(NB_THREAD);

        int* R = new int[MAX_SIZE];
        int* S = new int[MAX_SIZE];

        double start;

        fillTable(R, MAX_SIZE, MAX_RAND_TEST);
        fillTable(S, MAX_SIZE, MAX_RAND_TEST);

        cout << "Start sort" << endl;
        start = sec();
        parallelSort(R, MAX_SIZE, threadPool, partitionArray(R, MAX_SIZE, NB_THREAD));
        cout << "Sorting R with parallel radix : " << (sec() - start) << " seconds" << endl;
        cout << "Sorted : " << checkSorted(R, MAX_SIZE) << endl;

        cout << "Start sort" << endl;
        start = sec();
        sort(S, S + MAX_SIZE);
        cout << "Sorting S with std::sort : " << (sec() - start) << " seconds" << endl;
        cout << "Sorted : " << checkSorted(S, MAX_SIZE) << endl;

        cout << "Start merge" << endl;
        start = sec();
        auto result = parallelMerge(R, S, MAX_SIZE, MAX_SIZE);
        cout << "Merging R with parallel merge 1: " << (sec() - start) << " seconds" << endl;
        //cout << "Merged? -> " << checkMerge(R, MAX_SIZE, S, MAX_SIZE, assembleResults(result)) << endl;

        cout << "Start merge" << endl;
        start = sec();
        result = parallelMerge2(R, S, MAX_SIZE, MAX_SIZE);
        cout << "Merging R with parallel merge 2: " << (sec() - start) << " seconds" << endl;
        //cout << "Merged? -> " << checkMerge(R, MAX_SIZE, S, MAX_SIZE, assembleResults(result)) << endl;

        cout << "Start merge" << endl;
        start = sec();
        result = parallelMerge3(threadPool, R, S, MAX_SIZE, MAX_SIZE, partitionArray(R, MAX_SIZE, NB_THREAD));
        cout << "Merging R with parallel merge 3: " << (sec() - start) << " seconds" << endl;

        cout << "Start merge" << endl;
        vector<string> join;
        start = sec();
        mergeRelations(R, R + MAX_SIZE, S, S + MAX_SIZE, join, 0, 0);
        cout << "Merging R with merge : " << (sec() - start) << " seconds" << endl;
        //cout << "Merged? -> " << checkMerge(R, MAX_SIZE, S, MAX_SIZE, join) << endl;

        delete[] R;
        delete[] S;
    }

    void testParallelRadixSort() {
        int *table = new int[20];
        fillTable(table, 20, 120);
        printTable(table, 20);
        cout << endl;

        NB_THREAD = 2;
        ThreadPool threadPool(NB_THREAD);
        auto partition = partitionArray(table, 20, NB_THREAD);

        ParallelRadixSort(table, 20, threadPool, partition, NB_THREAD);

        printTable(table, 20);

        cout << "sorted? " << ((checkSorted(table, 20))?"yes":"no") << endl;

        delete[] table;
    }

}

