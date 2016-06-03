#ifndef SORTMERGEJOIN_PARALLELRADIXSORT_HPP
#define SORTMERGEJOIN_PARALLELRADIXSORT_HPP

#include "ParallelSMJ.h"

namespace SMJ {

    const int kNumDigit = 10;

    inline int GetDigit(int v, int p) {
        return (v >> p * 8) & 0x000000ff;
    }

    void CreateHistogramWithMax(int *start, int *end, int digit, std::vector<int> &histogram, int &max);

    std::vector<std::vector<int>> MakeHistogramsWithMax(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit, int &max);

    void CreateHistogram(int *start, int *end, int digit, std::vector<int> &histogram);

    std::vector<std::vector<int>> MakeHistograms(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit);

    void MoveResults(int *table, const std::vector<Partition> &copy, ThreadPool &threadPool,
                     std::vector<std::vector<int>> &histograms, int num_thread, int digit);

    void MoveRoutine(int *table, const Partition &copy, std::vector<int> &histogram, int digit);

    void ParallelRadixSort(int *table, uint size, ThreadPool& threadPool, std::vector<Partition> &partitions,
                           int num_thread);
}

#endif //SORTMERGEJOIN_PARALLELRADIXSORT_HPP
