#ifndef SORTMERGEJOIN_PARTITIONEDARRAY_HPP
#define SORTMERGEJOIN_PARTITIONEDARRAY_HPP

#include <vector>

namespace SMJ {
    template<typename T>
    class PartitionedArray{
    public:
        PartitionedArray<T>(std::vector<T> array, int num_partition);
        PartitionedArray<T>(T* array, size_t array_size, int num_partition);
        ~PartitionedArray();

        inline T* GetPartition(int partition_number){
            return &partitions_[partition_number][0];
        }

        inline int GetPartitionSize(int partition_number){
            return partitions_[partition_number].size();
        }

    private:
        std::vector<std::vector<T>> partitions_;
        int num_partitions_;
    };

    template<typename T>
    PartitionedArray<T>::PartitionedArray(std::vector<T> array, int num_partition):
        PartitionedArray<T>(&array[0], array.size(), num_partition)
    {
    }

    template<typename T>
    PartitionedArray<T>::PartitionedArray(T *array, size_t array_size, int num_partition):
        num_partitions_(num_partition)
    {
        int partition_size = (int) (array_size / num_partitions_);

        for(int partition = 0; partition < num_partitions_; ++partition){
            int start = partition * partition_size;
            int end = start + partition_size;
            if(partition == num_partitions_ - 1){
                end += array_size % end;
            }

            partitions_.push_back(std::vector<T>());

            for(int* p = array + start; p != array + start + end; ++p){
                partitions_[partition].push_back(*p);
            }
        }
    }

    template<typename T>
    PartitionedArray<T>::~PartitionedArray() {
        partitions_.clear();
    }

}

#endif //SORTMERGEJOIN_PARTITIONEDARRAY_HPP
