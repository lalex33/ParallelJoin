#ifndef SORTMERGEJOIN_PARALLELHASHJOIN_HPP
#define SORTMERGEJOIN_PARALLELHASHJOIN_HPP

#include <map>
#include "SMJUtility.h"

class ParallelHashJoin {

public:
    ParallelHashJoin(uint num_rows_r, uint num_rows_s, uint num_buckets = kNUM_BUCKETS);

    ~ParallelHashJoin();

    void HashJoinOnR(uint max_values = kMAX_INT);

    void HashJoinOnS(uint max_values = kMAX_INT);

    void ParallelHashJoinOnR(uint max_values = kMAX_INT);

    void ParallelHashJoinOnS(uint max_values = kMAX_INT);

    void PrintResult(bool verbose = false);

    inline double GetProcessingTime(){
        return time_hash_join_;
    }

    inline std::vector<std::string> GetResults(){
        return results_;
    }

private:
    static const int kMAX_INT = INT_MAX;
    static const int kNUM_BUCKETS = 100;

    int *R, *S;
    size_t size_r_, size_s_;

    uint num_buckets_;
    std::hash<int> hash_fn_;
    std::map<int, std::vector<int>> hash_buckets_;

    std::vector<std::string> results_;

    double time_hash_join_;

private:
    inline void InitTables(uint max_values){
        R = new int[size_r_];
        S = new int[size_s_];
        fillTable(R, size_r_, max_values);
        fillTable(S, size_r_, max_values);
    }

    inline size_t Hash(int* value){
        //return ( hash_fn_(*value) % num_buckets_);
        return (*value) % num_buckets_;
    }

    void HashTable(int* table, size_t size);

    void HashJoin(int* table, size_t size);

    void ParallelHashTable(int* table, size_t size);

    void ParallelHashjoin(int* table, size_t size);

};


#endif //SORTMERGEJOIN_PARALLELHASHJOIN_HPP
