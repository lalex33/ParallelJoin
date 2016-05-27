#ifndef SORTMERGEJOIN_PARTITIONEDARRAY_HPP
#define SORTMERGEJOIN_PARTITIONEDARRAY_HPP

#include <vector>

namespace SMJ {
    template<typename T>
    class PartitionedArray{
    public:
        PartitionedArray<T>(std::vector<T> array, int num_partitions);
        PartitionedArray<T>(T* array, size_t array_size, int num_partitions);
        ~PartitionedArray();

        inline T* GetPartition(int partition_number){
            return partitions_[partition_number];
        }

        inline size_t GetPartitionSize(int partition_number){
            return partitions_size_[partition_number];
        }

    private:
        std::vector<T*> partitions_;
        std::vector<size_t> partitions_size_;
    };

    template<typename T>
    PartitionedArray<T>::PartitionedArray(std::vector<T> array, int num_partitions):
        PartitionedArray<T>(&array[0], array.size(), num_partitions)
    {
    }

    template<typename T>
    PartitionedArray<T>::PartitionedArray(T *array, size_t array_size, int num_partitions) {
        int partition_size = array_size / num_partitions;

        for(int partition = 0; partition < num_partitions; ++partition){
            int start = partition * partition_size;
            int end = start + partition_size;
            if(partition == num_partitions-1){
                end += array_size % end;
            }

            T* datas = new T[end - start];
            std::copy(array + start, array + start + end, datas);

            partitions_.push_back(datas);
            partitions_size_.push_back(end - start);
        }
    }

    template<typename T>
    PartitionedArray<T>::~PartitionedArray() {
        for(T* partition : partitions_){
            delete[] partition;
        }
        partitions_.clear();
        partitions_size_.clear();
    }

}

#endif //SORTMERGEJOIN_PARTITIONEDARRAY_HPP
