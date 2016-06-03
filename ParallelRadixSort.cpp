#include "ParallelRadixSort.hpp"

namespace SMJ {

    void CreateHistogramWithMax(int *start, int *end, int digit, std::vector<int> &histogram, int &max){
        max = *start;
        while (start != end) {
            if(*start > max) max = *start;
            histogram[getDigit(*(start++), digit)]++;
        }
    }

    std::vector<std::vector<int>> MakeHistogramsWithMax(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit, int &max){
        std::vector<std::vector<int>> histograms(num_threads);
        int maxArray[num_threads];

        for (int thread = 0; thread < num_threads; ++thread) {
            std::vector<int> histogram(kNumDigit,0);
            histograms[thread] = histogram;
            threadPool.Enqueue( std::bind(CreateHistogramWithMax, partitions[thread].start, partitions[thread].end,
                                          digit, std::ref(histograms[thread]), std::ref(maxArray[thread])) );
        }
        threadPool.WaitEndOfWork();

        max = *std::max_element(maxArray, maxArray + num_threads);

        auto histogramCopy = histograms;
        for (int thread = 0; thread < num_threads; ++thread) {
            for(int i = 0; i < kNumDigit; ++i){
                histograms[thread][i] = 0;
                for(int th= 0; th < num_threads; ++th){
                    for(int j = 0; j < i; ++j){
                        histograms[thread][i] += histogramCopy[th][j];
                    }
                    if(th < thread){
                        histograms[thread][i] += histogramCopy[th][i];
                    }
                }
            }
        }

        return histograms;
    }

    void CreateHistogram(int *start, int *end, int digit, std::vector<int> &histogram) {
        while (start != end) {
            histogram[getDigit(*(start++), digit)]++;
        }
    }

    std::vector<std::vector<int>> MakeHistograms(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit) {
        std::vector<std::vector<int>> histograms(num_threads);

        for (int thread = 0; thread < num_threads; ++thread) {
            std::vector<int> histogram(kNumDigit,0);
            histograms[thread] = histogram;
            threadPool.Enqueue( std::bind(CreateHistogram, partitions[thread].start, partitions[thread].end,
                                     digit, std::ref(histograms[thread])) );
        }
        threadPool.WaitEndOfWork();

        auto histogramCopy = histograms;
        for (int thread = 0; thread < num_threads; ++thread) {
            for(int i = 0; i < kNumDigit; ++i){
                histograms[thread][i] = 0;
                for(int th= 0; th < num_threads; ++th){
                    for(int j = 0; j < i; ++j){
                        histograms[thread][i] += histogramCopy[th][j];
                    }
                    if(th < thread){
                        histograms[thread][i] += histogramCopy[th][i];
                    }
                }
            }
        }

        return histograms;
    }

    void MoveResults(int *table, const std::vector<Partition> &copy, ThreadPool &threadPool,
                     std::vector<std::vector<int>> &histograms, int num_thread, int digit) {
        for (int thread = 0; thread < num_thread; ++thread) {
            threadPool.Enqueue( std::bind(MoveRoutine, table, std::ref(copy[thread]),
                                          std::ref(histograms[thread]), digit) );
        }
        threadPool.WaitEndOfWork();
    }

    void MoveRoutine(int *table, const Partition &copy, std::vector<int> &histogram, int digit) {
        int *start = copy.start, *end = copy.end, pos;
        while (start != end){
            pos = getDigit(*start, digit);
            *(table + histogram[pos]++) = *(start++);
        }
    }

    void ParallelRadixSort(int *table, uint size, ThreadPool &threadPool, std::vector<Partition> &partitions,
                           int num_thread) {
        int* copy = new int[size];
        auto copyPartition = partitionArray(copy, size, num_thread);
        int max = 0;

        std::copy(table, table + size, copy);
        auto histograms = MakeHistogramsWithMax(threadPool, partitions, num_thread, 0, max);
        MoveResults(table, copyPartition, threadPool, histograms, num_thread, 0);

        std::ostringstream oss;
        oss << max;
        ulong digitLength = oss.str().size();

        for(uint digit = 1; digit < digitLength ; ++digit){
            // parallel copy?
            std::copy(table, table + size, copy);

            histograms = MakeHistograms(threadPool, partitions, num_thread, digit);

            MoveResults(table, copyPartition, threadPool, histograms, num_thread, digit);
        }

        delete[] copy;
    }

}

