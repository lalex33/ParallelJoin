#include "ParallelRadixSort.hpp"

namespace SMJ {

    void CreateHistogramWithMax(int *start, int *end, int digit, std::vector<int> &histogram, int &max){
        //double s = sec();
        max = *start;
        while (start != end) {
            if(*start > max) max = *start;
            ++histogram[getDigit(*(start++), digit)];
        }
        //std::cout << "histogram routine : " << (sec() - s) << std::endl;
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
        //double s = sec();
        while (start != end) {
            ++histogram[getDigit(*(start++), digit)];
        }
        //std::cout << "histogram routine : " << (sec() - s) << std::endl;
    }

    std::vector<std::vector<int>> MakeHistograms(ThreadPool &threadPool, const std::vector<Partition> &partitions,
                                                 int num_threads, int digit) {
        std::vector<std::vector<int>> histograms(num_threads);

        //double s = sec();
        for (int thread = 0; thread < num_threads; ++thread) {
            std::vector<int> histogram(kNumDigit,0);
            histograms[thread] = histogram;
            threadPool.Enqueue( std::bind(CreateHistogram, partitions[thread].start, partitions[thread].end,
                                     digit, std::ref(histograms[thread])) );
        }
        //std::cout << "launch histo : " << (sec() - s) << std::endl;
        //s = sec();
        threadPool.WaitEndOfWork();
        //std::cout << "wait histo : " << (sec() - s) << std::endl;

        ComputePositions(histograms, num_threads);

        return histograms;
    }

    void MoveResults(int *table, const std::vector<Partition> &copy, ThreadPool &threadPool,
                     std::vector<std::vector<int>> &histograms, int num_thread, int digit) {
        //double s = sec();
        for (int thread = 0; thread < num_thread; ++thread) {
            threadPool.Enqueue( std::bind(MoveRoutine, table, std::ref(copy[thread]),
                                          std::ref(histograms[thread]), digit) );
        }
        //std::cout << "launch move routine : " << (sec() - s) << std::endl;
        //s = sec();
        threadPool.WaitEndOfWork();
        //std::cout << "wait end move routine : " << (sec() - s) << std::endl;
    }

    void MoveRoutine(int *table, const Partition &copy, std::vector<int> &histogram, int digit) {
        //double s = sec();
        int *start = copy.start, *end = copy.end;
        while (start != end){
            *(table + histogram[getDigit(*start, digit)]++) = *start;
            ++start;
        }
        //std::cout << "move routine : " << (sec() - s) << std::endl;
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

        //double start = sec();
        auto histograms = MakeHistogramsWithMax(threadPool, *table_partition, num_thread, 0, max);
        //std::cout << "histograms with max : " << (sec() - start) << std::endl;
        //start = sec();
        MoveResults(p_buffer, *table_partition, threadPool, histograms, num_thread, 0);
        //std::cout << "move results : " << (sec() - start) << std::endl << std::endl;

        std::ostringstream oss;
        oss << max;
        ulong digitLength = oss.str().size();

        for(uint digit = 1; digit < digitLength ; ++digit){
            //start = sec();
            std::swap(p_table, p_buffer);
            std::swap(table_partition, buffer_partition);
            histograms = MakeHistograms(threadPool, *table_partition, num_thread, digit);
            //std::cout << "histograms : " << (sec() - start) << std::endl;
            //start = sec();
            MoveResults(p_buffer, *table_partition, threadPool, histograms, num_thread, digit);
            //std::cout << "move results : " << (sec() - start) << std::endl << std::endl;
        }

        table = p_buffer;
        partitions = *buffer_partition;

        delete[] p_table;
    }

}

