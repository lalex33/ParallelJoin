#include "ParallelRadixSort.hpp"

namespace SMJ {

    void CreateHistogram(int *start, int *end, int digit, std::vector<int> &histogram) {
        while (start != end) {
            histogram[getDigit(*(start++), digit)]++;
        }
    }

    std::vector<std::vector<int>> MakeHistograms(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit) {
        std::vector<std::vector<int>> histograms(num_threads);

        for (int thread = 0; thread < num_threads; ++thread) {
            std::vector<int> histogram(10, 0);
            histograms.push_back(histogram);
            threadPool.Enqueue( std::bind(CreateHistogram, partitions[thread].start, partitions[thread].end,
                                     digit, std::ref(histograms[thread])) );
        }
        threadPool.WaitEndOfWork();

        for (int thread = 0; thread < num_threads; ++thread) {
            for(int i=0; i < 10; ++i){
                for(int th=0; th < num_threads; ++th){
                    for(int j=0; j < i; ++j){
                        histograms[thread][i] += histograms[th][j];
                    }
                }
            }
        }

        return histograms;
    }

    void MoveResults(std::vector<Partition> &table, const std::vector<Partition> &copy, ThreadPool &threadPool,
                     std::vector<std::vector<int>> &histograms, int num_thread, int digit) {
        for (int thread = 0; thread < num_thread; ++thread) {
            threadPool.Enqueue( std::bind(MoveRoutine, std::ref(table[thread]), std::ref(copy[thread]),
                                          std::ref(histograms[thread]), digit) );
        }
        threadPool.WaitEndOfWork();
    }

    void MoveRoutine(Partition &table, const Partition &copy, std::vector<int> &histogram, int digit) {
        int *start = copy.start, *end = copy.end;
        int *table_start = table.start, pos;
        while (start != end){
            pos = getDigit(*start, digit);
            *(table_start + (histogram[pos]++) - 1) = *(start++);
        }
    }

    void ParallelRadixSort(int *table, uint size, ThreadPool &threadPool, std::vector<Partition> &partitions,
                           int num_thread) {

        std::ostringstream oss;
        oss << parallelMax(table, size, threadPool, partitions);
        ulong digitLength = oss.str().size();

        int* copy = new int[size];
        auto copyPartition = partitionArray(copy, size, num_thread);

        for(uint digit = 0 ; digit < digitLength ; ++digit){
            // parallel copy?
            std::copy(table, table + size, copy);

            auto histograms = MakeHistograms(threadPool, partitions, num_thread, digit);

            MoveResults(partitions, copyPartition, threadPool, histograms, num_thread, digit);
        }

        delete[] copy;
    }

}

