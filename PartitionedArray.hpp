#ifndef SORTMERGEJOIN_PARTITIONEDARRAY_HPP
#define SORTMERGEJOIN_PARTITIONEDARRAY_HPP

#include <vector>

namespace SMJ {
    template<typename T, int P>
    class PartitionedArray{
    public:
        PartitionedArray<T,P>(std::vector<T> array);
        PartitionedArray<T,P>(T* array, size_t array_size);
        ~PartitionedArray();

        inline T* GetPartition(int partition_number){
            return partitions_[partition_number];
        }

        inline size_t GetPartitionSize(int partition_number){
            return partitions_size_[partition_number];
        }

    private:
        int* partitions_[P];
        size_t partitions_size_[P];
    };

    template<typename T, int P>
    PartitionedArray<T,P>::PartitionedArray(std::vector<T> array):
        PartitionedArray<T,P>(&array[0], array.size())
    {
        // error with vector (memory error)
    }

    template<typename T, int P>
    PartitionedArray<T,P>::PartitionedArray(T *array, size_t array_size) {
        int partition_size = (int) (array_size / P);

        for(int partition = 0; partition < P; ++partition){
            int start = partition * partition_size;
            int end = start + partition_size;
            if(partition == P-1){
                end += array_size % end;
            }

            T* data = new T[end - start];

            partitions_[partition] = data;
            partitions_size_[partition] = (size_t) (end - start);

            for(int* p = array + start; p != array + start + end; ++p){
                *(data++) = *p;
            }
        }
    }

    template<typename T, int P>
    PartitionedArray<T,P>::~PartitionedArray() {
        for(int partition = 0; partition < P; ++partition){
            delete[] partitions_[partition];
        }
    }

}

#endif //SORTMERGEJOIN_PARTITIONEDARRAY_HPP
