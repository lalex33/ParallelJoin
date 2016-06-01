#ifndef SORTMERGEJOIN_PARALLELHASHJOIN_HPP
#define SORTMERGEJOIN_PARALLELHASHJOIN_HPP

#include <map>
#include "SMJUtility.h"
#include "ThreadPool.h"

class ParallelHashJoin {

public:
    ParallelHashJoin(uint num_rows_r, uint num_rows_s, uint num_buckets = kNUM_BUCKETS, uint num_threads = kNUM_THREADS);

    ~ParallelHashJoin();

    inline void InitTables(uint max_values){
        R = new int[size_r_];
        S = new int[size_s_];
        fillTable(R, size_r_, max_values);
        fillTable(S, size_r_, max_values);
    }

    void PrintResult(bool verbose = false);

    inline void HashJoinOnR(){
        ComputeHashJoin(R, size_r_, S, size_s_);
    }

    inline void HashJoinOnS(){
        ComputeHashJoin(S, size_s_, R, size_r_);
    }

    inline void ParallelHashJoinOnR(){
        ComputeParallelHashJoin(R, size_r_, S, size_s_);
    }

    inline void ParallelHashJoinOnS(){
        ComputeParallelHashJoin(S, size_s_, R, size_r_);
    }

    inline double GetProcessingTime(){ return time_hash_join_; }
    inline std::vector<std::string> GetResults(){ return results_; }
    inline int* GetR(){ return R; }
    inline int* GetS(){ return S; }
    inline size_t GetSizeR(){ return size_r_; }
    inline size_t GetSizeS(){ return size_s_; }

protected:
    struct Bucket{
        std::vector<int> values;
        std::mutex bucket_lock;
    };

private:
    static const int kMAX_INT = INT_MAX;
    static const uint kNUM_BUCKETS = 100;
    static const uint kNUM_THREADS = 4;

    int *R, *S;
    size_t size_r_, size_s_;

    uint num_buckets_;
    std::hash<int> hash_fn_;
    std::vector<Bucket> hash_buckets_;

    std::vector<std::string> results_;

    double time_hash_join_;

    uint num_threads_;
    ThreadPool threadpool_;

private:

    inline int Hash(int* value){
        //return ( hash_fn_(*value) % num_buckets_); // useful with template class
        return (*value) % num_buckets_;
    }

    void ComputeHashJoin(int* r, size_t size_R, int* s, size_t size_S);

    void ComputeParallelHashJoin(int* r, size_t size_R, int* s, size_t size_S);

    void HashTable(int* table, size_t size, std::vector<Bucket> &hash_buckets);

    void HashJoin(int* table, size_t size, std::vector<std::string> &results);

    void ParallelHashTable(int* table, size_t size);

    std::vector<std::vector<std::string>> ParallelHashjoin(int* table, size_t size);

};


#endif //SORTMERGEJOIN_PARALLELHASHJOIN_HPP
