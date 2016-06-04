#ifndef SORTMERGEJOIN_PARALLELSORT_H
#define SORTMERGEJOIN_PARALLELSORT_H

#include "ThreadWork.h"
#include "PartitionedArray.hpp"

namespace SMJ{

    typedef std::vector<std::vector<int>> digits_bucket;
    const uint MAX_DIGIT = 10;
    extern uint NB_THREAD;

    static int pow10[10] = {
        1, 10, 100, 1000, 10000,
        100000, 1000000, 10000000, 100000000, 1000000000
    };

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

    /*
     * return the digit at the given position
     * pos of least digit = 0
     */
    inline int getDigit(int number, int pos){
        //return (pos == 0) ? number % 10 : getDigit (number/10, --pos);
        if(pos == 0) return number%10;
        return ( (number / pow10[pos]) % 10);
    }

    /*
     * start a merge using threads, partitions of R
     */
    std::vector<std::vector<std::string>> parallelMerge(int* R, int* S, uint sizeR, uint sizeS);

    /*
     * start a merge using threads, partitions of R and S
     */
    std::vector<std::vector<std::string>> parallelMerge2(int* R, int* S, uint sizeR, uint sizeS);

    /*
     * parallel merge using ThreadPool, partitions of R
     */
    std::vector<std::vector<std::string>> parallelMerge3(ThreadPool& threadPool, int* R, int* S,
                                                         uint sizeR, uint sizeS, const std::vector<Partition> &partitions);

    /*
     * parallel merge using WorkThread, partitions of R
     */
    std::vector<std::vector<std::string>> parallelMerge4(ThreadWork& threadWork, int* R, int* S, uint sizeR, uint sizeS);

    /*
     * parallel merge using Workthread, partitions of R and S
     */
    std::vector<std::vector<std::string>> parallelMerge5(int* R, int* S, uint sizeR, uint sizeS);

    /*
     * parallelMerge5 with ThreadPool and PartitionedArray
     */
    std::vector<std::vector<std::string>> parallelMerge6(ThreadPool& threadPool, PartitionedArray<int>& R, PartitionedArray<int>& S);

}

#endif //SORTMERGEJOIN_PARALLELSORT_H
