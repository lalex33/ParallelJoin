#ifndef SORTMERGEJOIN_PARALLELRADIXSORT_HPP
#define SORTMERGEJOIN_PARALLELRADIXSORT_HPP

#include "ThreadPool.h"
#include "ParallelSMJ.h"

#include <vector>
#include <functional>
#include <sstream>

namespace SMJ {

    const int kNumDigit = 10;

    inline void ComputePositions(std::vector<std::vector<int>> &histograms, int num_threads){
        auto histogramCopy = histograms;

        for (int thread = 0; thread < num_threads; ++thread) {
            for(int i = 0; i < kNumDigit; ++i){
                histograms[thread][i] = 0;
                for(int th = 0; th < num_threads; ++th){
                    for(int j = 0; j < i; ++j){
                        histograms[thread][i] += histogramCopy[th][j];
                    }
                    if(th < thread){
                        histograms[thread][i] += histogramCopy[th][i];
                    }
                }
            }
        }
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

    void ParallelRadixSort(int* &table, uint size, ThreadPool& threadPool, std::vector<Partition> &partitions,
                           int num_thread);
}

#endif //SORTMERGEJOIN_PARALLELRADIXSORT_HPP
