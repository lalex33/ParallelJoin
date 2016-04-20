#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include "SortMergeJoin.h"

namespace SMJ{

    typedef std::vector<std::vector<int>> digits_bucket;
    const uint MAX_DIGIT_EXCLUDED = 10;
    extern uint NB_THREAD;

    /*
     * start a radix sort using threads
     */
    void parallelSort(int *table, uint size);

    /*
     * sort values in buckets by a digit
     */
    void radixSort(int *table, digits_bucket &buckets,
                   uint posDigit, uint start, uint end);

    /*
     * find max value of array using thread
     */
    int parallelMax(int *table, uint size);

    /*
     * find the max value of an array and store it in <max>
     */
    void maxRoutine(int* start, int* end, int* max);

    /*
     * return the digit at the given position
     * pos of least digit = 0
     * credit : http://stackoverflow.com/a/7438270
     */
    int getDigit (int number, int pos);

    /*
     * start a merge using threads
     */
    std::vector<std::vector<std::string>> parallelMerge(int* R, int* S, uint sizeR, uint sizeS);

    /*
     * return the final merge list
     */
    std::vector<std::string> assembleResults(std::vector<std::vector<std::string>> results);

}

#endif //SORTMERGEJOIN_PARALLELSORT_H
