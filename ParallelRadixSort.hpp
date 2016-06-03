#ifndef SORTMERGEJOIN_PARALLELRADIXSORT_HPP
#define SORTMERGEJOIN_PARALLELRADIXSORT_HPP

#include "ParallelSMJ.h"

namespace SMJ {

    inline int GetDigit(int v, int p) {
        return (v >> p * 8) & 0x000000ff;
    }

    void CreateHistogram(int *start, int *end, int digit, std::vector<int> &histogram);

    std::vector<std::vector<int>> MakeHistograms(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit);

    void MoveResults(std::vector<Partition> &table, const std::vector<Partition> &copy, ThreadPool &threadPool,
                     std::vector<std::vector<int>> &histograms, int num_thread, int digit);

    void MoveRoutine(Partition &table, const Partition &copy, std::vector<int> &histogram, int digit);

    void ParallelRadixSort(int *table, uint size, ThreadPool& threadPool, std::vector<Partition> &partitions,
                           int num_thread);
}

#endif //SORTMERGEJOIN_PARALLELRADIXSORT_HPP
