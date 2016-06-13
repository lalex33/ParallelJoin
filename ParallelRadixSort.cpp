#include "ParallelRadixSort.hpp"

namespace SMJ {

    void CreateHistogramWithMax(int *start, int *end, int digit, std::vector<int> &histogram, int &max){
        max = *start;
        while (start != end) {
            if(*start > max) max = *start;
            ++histogram[getDigit(*(start++), digit)];
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

        ComputePositions(histograms, num_threads);

        return histograms;
    }

    void CreateHistogram(int *start, int *end, int digit, std::vector<int> &histogram) {
        while (start != end) {
            ++histogram[getDigit(*(start++), digit)];
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

        ComputePositions(histograms, num_threads);

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
        int *start = copy.start, *end = copy.end;
        while (start != end){
            *(table + histogram[getDigit(*start, digit)]++) = *start;
            ++start;
        }
    }

    void ParallelRadixSort(int* &table, uint size, ThreadPool &threadPool, std::vector<Partition> &partitions,
                           int num_thread) {
        int* buffer = new int[size];
        auto bufferPartition = partitionArray(buffer, size, num_thread);
        int max = 0;

        int* p_table = table;
        int* p_buffer = buffer;
        auto buffer_partition = &bufferPartition;
        auto table_partition = &partitions;

        auto histograms = MakeHistogramsWithMax(threadPool, *table_partition, num_thread, 0, max);
        MoveResults(p_buffer, *table_partition, threadPool, histograms, num_thread, 0);

        std::ostringstream oss;
        oss << max;
        ulong digitLength = oss.str().size();

        for(uint digit = 1; digit < digitLength ; ++digit){
            std::swap(p_table, p_buffer);
            std::swap(table_partition, buffer_partition);
            histograms = MakeHistograms(threadPool, *table_partition, num_thread, digit);
            MoveResults(p_buffer, *table_partition, threadPool, histograms, num_thread, digit);
        }

        table = p_buffer;
        partitions = *buffer_partition;

        delete[] p_table;
    }

}

