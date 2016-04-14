//
// Created by Alexis bayle on 14/04/2016.
//

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

void fillTable(int* table, size_t size, uint maxValue){
    for(int i=0; i<size; i++){
        table[i] = rand() % maxValue;
    }
}

void parallelSort(int* table, int size){

}

void parallelMerge(int* R, int* S){

}