#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include "ThreadPool.h"

namespace SMJ{

    typedef std::vector<std::vector<int>> digits_bucket;
    const uint MAX_DIGIT = 10;
    extern uint NB_THREAD;

    struct Partition{
        int *start, *end;
    };

    std::vector<Partition> partitionArray(int *table, uint size, uint num_threads);

    /*
     * start a radix sort using threads
     */
    void parallelSort(int *table, uint size, ThreadPool& threadPool, const std::vector<Partition> &partitions);

    /*
     * sort values in buckets by a digit
     */
    void radixSort(int *start, int *end, digits_bucket &buckets, uint posDigit);

    /*
     * find max value of array using thread
     */
    int parallelMax(int *table, uint size, ThreadPool& threadPool, const std::vector<Partition> &partitions);


    static int pow10[10] = {
            1, 10, 100, 1000, 10000,
            100000, 1000000, 10000000, 100000000, 1000000000
    };

    /*
     * return the digit at the given position
     * pos of least digit = 0
     */
    inline int getDigit(int number, int pos){
        return ( (number / pow10[pos]) % 10);
    }

    /*
     * parallel merge using ThreadPool, partitions of R
     */
    std::vector<std::vector<std::string>> parallelMerge(ThreadPool &threadPool, int *R, int *S,
                                                        uint sizeR, uint sizeS,
                                                        const std::vector<Partition> &partitions);
}

#endif //SORTMERGEJOIN_PARALLELSORT_H
