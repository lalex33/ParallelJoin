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

    unsigned long digitLength = to_string(max(table, size)).length();

    
}

int max(int* table, int size){
    int max = table[0];
    for(int n=1; n<size; n++){
        if(table[n]>max){
            max = table[n];
        }
    }
    return max;
}

int getDigit (int number, int pos)
{
    return (pos == 0) ? number % 10 : getDigit (number/10, --pos);
}

int pow(int n, int e){
    if(e == 0){
        return 1;
    } else{
        return n * pow(n, e-1);
    }
}

void parallelMerge(int* R, int* S){

}