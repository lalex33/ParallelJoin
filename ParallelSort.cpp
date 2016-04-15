#include <iostream>
#include "ParallelSort.h"

using namespace std;

mutex threadLock;

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

void testParallelSort(){
    srand(time(NULL));

    int* R = new int[TABLE_ROWS_R];
    fillTable(R, TABLE_ROWS_R, MAX_RAND_VALUE);

    clock_t start = clock();

    parallelSort(R, TABLE_ROWS_R);

    cout << (( clock() - start ) / (double) CLOCKS_PER_SEC) << endl;
    for(int i=0; i<TABLE_ROWS_R; i++){
        cout << R[i] << "  <  ";
    }

    cout << endl << "array sorted? " << checkSorted(R, TABLE_ROWS_R) << endl;

    delete[] R;
}

void fillTable(int* table, size_t size, uint maxValue){
    for(int i=0; i<size; i++){
        table[i] = rand() % maxValue;
    }
}

/*
 * issue found : it's not stable so we need to keep same places as previous loop
 */
void parallelSort(int* table, int size){
    // compute size of the max int
    ulong digitLength = to_string(max(table, size)).length();

    // compute number of int to sort by a thread
    uint sizePerThread = size / NB_THREAD;

    vector<vector<int>> buckets(10);
    vector<thread> threads;

    // loop on each digit (from the least to the most)
    for(ulong nbDigit = 0 ; nbDigit < digitLength ; ++nbDigit){

        // create <NB_THREAD> threads which will sort a sublist
        for(uint nbThread = 0 ; nbThread < NB_THREAD ; ++nbThread){
            threads.push_back( thread(radixSort, table, ref(buckets), nbDigit,
                                      nbThread*sizePerThread, nbThread*sizePerThread + sizePerThread ));
        }

        // wait end of sort
        for(auto& thread : threads){
            thread.join();
        }

        // put sorted digit values in array
        int* pos = table;
        for(auto bucket : buckets){
            for(auto value : bucket){
                *pos = value;
                ++pos;
            }
        }

        // clear temporary arrays
        for(auto &bucket : buckets){
            bucket.clear();
        }
        threads.clear();
    }
}

void radixSort(int* table, vector<vector<int>>& buckets,
               ulong posDigit, uint start, uint end){
    int position;
    for(uint n = start; n < end; n++){
        position = getDigit(table[n], posDigit);
        threadLock.lock();
            buckets[ position ].push_back(table[n]);
        threadLock.unlock();
    }
}

int max(int* table, int size){
    int* end = table + size;
    int* p = table;
    int* max = p;
    while(p != end){
        if(*p > *max){
            max = p;
        }
        p++;
    }
    return *max;
}

int getDigit (int number, int pos)
{
    return (pos == 0) ? number % 10 : getDigit (number/10, --pos);
}

bool checkSorted(int* table, int size){
    int* p = table;
    int* end = table + size - 1;
    while(p != end){
        if(*p > *(p+1))
            return false;
        p++;
    }
    return true;
}

void parallelMerge(int* R, int* S){

}