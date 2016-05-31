#ifndef SORTMERGEJOIN_PARALLELHASHJOIN_HPP
#define SORTMERGEJOIN_PARALLELHASHJOIN_HPP

#include <map>
#include "SMJUtility.h"
#include "ThreadPool.h"

class ParallelHashJoin {

public:
    ParallelHashJoin(uint num_rows_r, uint num_rows_s, uint num_buckets = kNUM_BUCKETS, uint num_threads = kNUM_THREADS);

    ~ParallelHashJoin();

    void PrintResult(bool verbose = false);

    inline double GetProcessingTime(){
        return time_hash_join_;
    }

    inline std::vector<std::string> GetResults(){
        return results_;
    }

    inline void HashJoinOnR(uint max_values = kMAX_INT){
        InitTables(max_values);
        ComputeHashJoin(R, size_r_, S, size_s_);
    }

    inline void HashJoinOnS(uint max_values = kMAX_INT){
        InitTables(max_values);
        ComputeHashJoin(S, size_s_, R, size_r_);
    }

    inline void ParallelHashJoinOnR(uint max_values = kMAX_INT){
        InitTables(max_values);
        ComputeParallelHashJoin(R, size_r_, S, size_s_);
    }

    inline void ParallelHashJoinOnS(uint max_values = kMAX_INT){
        InitTables(max_values);
        ComputeParallelHashJoin(S, size_s_, R, size_r_);
    }

private:
    static const int kMAX_INT = INT_MAX;
    static const uint kNUM_BUCKETS = 100;
    static const uint kNUM_THREADS = 4;

    int *R, *S;
    size_t size_r_, size_s_;

    uint num_buckets_;
    std::hash<int> hash_fn_;
    std::map<int, std::vector<int>> hash_buckets_;

    std::vector<std::string> results_;

    double time_hash_join_;

    uint num_threads_;
    ThreadPool threadpool_;

    std::mutex hash_lock;

private:
    inline void InitTables(uint max_values){
        R = new int[size_r_];
        S = new int[size_s_];
        fillTable(R, size_r_, max_values);
        fillTable(S, size_r_, max_values);
    }

    inline int Hash(int* value){
        //return ( hash_fn_(*value) % num_buckets_); // useful with template
        return (*value) % num_buckets_;
    }

    void ComputeHashJoin(int* r, size_t size_R, int* s, size_t size_S);

    void ComputeParallelHashJoin(int* r, size_t size_R, int* s, size_t size_S);

    void HashTable(int* table, size_t size);

    void HashJoin(int* table, size_t size, std::vector<std::string> &results);

    void ParallelHashTable(int* table, size_t size);

    std::vector<std::vector<std::string>> ParallelHashjoin(int* table, size_t size);

};


#endif //SORTMERGEJOIN_PARALLELHASHJOIN_HPP
