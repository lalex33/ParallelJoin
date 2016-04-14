#include <iostream>
#include "ParallelSort.h"

using namespace std;

void testParallelJoin(){
    srand(time(NULL));

    int* R = new int[TABLE_ROWS_R];
    int* S = new int[TABLE_ROWS_S];

    fillTable(R, TABLE_ROWS_R, MAX_RAND_VALUE);
    fillTable(S, TABLE_ROWS_S, MAX_RAND_VALUE);

    parallelSort(R, TABLE_ROWS_R);
    parallelSort(S, TABLE_ROWS_S);

    parallelMerge(R, S);

    delete[] R;
    delete[] S;
}

void test(){
    srand(time(NULL));

    int* R = new int[TABLE_ROWS_R];
    fillTable(R, TABLE_ROWS_R, MAX_RAND_VALUE);
    parallelSort(R, TABLE_ROWS_R);

    cout << "finished" << endl;
}

void fillTable(int* table, size_t size, uint maxValue){
    for(int i=0; i<size; i++){
        table[i] = rand() % maxValue;
    }
}

void parallelSort(int* table, int size){
    // compute size of the max int
    ulong digitLength = to_string(max(table, size)).length();

    // compute number of int to sort by a thread
    uint sizePerThread = size / NB_THREAD;

    vector<vector<int>> buckets(10);
    vector<thread> threads(NB_THREAD);

    // loop on each digit (from the least to the most)
    for(ulong nbDigit = 0 ; nbDigit < digitLength ; ++nbDigit){
        // clear temporary arrays
        for(auto bucket : buckets){
            bucket.clear();
        }
        threads.clear();

        // create <NB_THREAD> threads which will compute a sort
        for(uint nbThread = 0 ; nbThread < NB_THREAD ; ++nbThread){
            threads.push_back( thread(radixSort, table, ref(buckets), nbDigit,
                                      nbThread*sizePerThread, nbThread*sizePerThread + sizePerThread - 1));
        }
        // wait end of sort
        for(auto& thread : threads){
            thread.join();
        }

        // put sorted values in array
        int* pos = table;
        for(auto bucket : buckets){
            for(auto value : bucket){
                *table = value;
                ++table;
            }
        }
    }
}

void radixSort(int* table, vector<vector<int>>& buckets,
               ulong posDigit, uint start, uint end){
    for(uint n = start; n < end; n++){
        buckets[ getDigit(table[n], posDigit) ].push_back(table[n]);
    }
}

int max(int* table, int size){
    int max = table[0];
    for(int n = 1; n < size ; ++n){
        if(table[n] > max){
            max = table[n];
        }
    }
    return max;
}

int getDigit (int number, int pos)
{
    return (pos == 0) ? number % 10 : getDigit (number/10, --pos);
}

void parallelMerge(int* R, int* S){

}