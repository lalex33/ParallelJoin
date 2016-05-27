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
            return partitions_[partition_number];
        }

        inline size_t GetPartitionSize(int partition_number){
            return partitions_size_[partition_number];
        }

    private:
        std::vector<T*> partitions_;
        std::vector<size_t> partitions_size_;
        int num_partitions_;
    };

    template<typename T>
    PartitionedArray<T>::PartitionedArray(std::vector<T> array, int num_partition):
        PartitionedArray<T>(&array[0], array.size(), num_partition)
    {
        // error with vector (freed object...)
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

            int size = end - start;
            T* data = new T[size];

            partitions_.push_back(data);
            partitions_size_.push_back( (size_t) (end - start) );

            for(int* p = array + start; p != array + start + end; ++p){
                *(data++) = *p;
            }
        }
    }

    template<typename T>
    PartitionedArray<T>::~PartitionedArray() {
        for(int partition = 0; partition < num_partitions_; ++partition){
            delete[] partitions_[partition];
        }
        partitions_.clear();
        partitions_size_.clear();
    }

}

#endif //SORTMERGEJOIN_PARTITIONEDARRAY_HPP
