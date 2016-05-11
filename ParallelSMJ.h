#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include "ThreadWork.h"

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
     * start other merge using threads
     */
    std::vector<std::vector<std::string>> parallelMerge2(int* R, int* S, uint sizeR, uint sizeS);

    /*
     * parallel merge using ThreadPool
     */
    std::vector<std::vector<std::string>> parallelMerge3(ThreadPool& threadPool, int* R, int* S, uint sizeR, uint sizeS);

    /*
     * parallel merge using threads without mutex
     */
    std::vector<std::vector<std::string>> parallelMerge4(ThreadWork& threadWork, int* R, int* S, uint sizeR, uint sizeS);

    /*
     * parallel merge using threads without mutex
     */
    std::vector<std::vector<std::string>> parallelMerge5(ThreadWork *threadWorks, int* R, int* S, uint sizeR, uint sizeS);

}

#endif //SORTMERGEJOIN_PARALLELSORT_H
