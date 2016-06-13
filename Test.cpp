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

        auto results = assembleResults(parallelMerge(threadPool, R, S, P_SIZE_R, P_SIZE_S, pR));

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

    void radixSortMillionElements() {
        const int size = 10000000;

        int *table = new int[size];
        fillTable(table, size, INT_MAX);

        NB_THREAD = 40;
        ThreadPool threadPool(NB_THREAD);

        double t = sec();
        auto partition = partitionArray(table, size, NB_THREAD);
        ParallelRadixSort(table, size, threadPool, partition, NB_THREAD);
        cout << "sorted in " << (sec() - t) << " seconds" << endl;

        cout << "sorted? " << ((checkSorted(table, size))?"yes":"no") << endl;

        delete[] table;
    }

    void testParallelHashJoin() {
        const int size = 10000000;
        ParallelHashJoin parallelHashJoin(size, size, size*2, 16);
        parallelHashJoin.InitTables(INT_MAX);
        parallelHashJoin.ParallelHashJoinOnR();
        cout << parallelHashJoin.GetHashTime() << " + " << parallelHashJoin.GetJoinTime() << " = " << parallelHashJoin.GetProcessingTime() << endl;
    }

}

