#ifndef SORTMERGEJOIN_PARALLELHASHJOIN_HPP
#define SORTMERGEJOIN_PARALLELHASHJOIN_HPP

#include <map>
#include "SMJUtility.h"

class ParallelHashJoin {

public:
    ParallelHashJoin(uint num_rows_r, uint num_rows_s);

    ~ParallelHashJoin(){
        delete[] R;
        delete[] S;
    }

    inline void InitTables(uint max_values){
        fillTable(R, size_r_, max_values);
        fillTable(S, size_r_, max_values);
    }

    inline void HashJoinOnR(uint max_values = kMAX_INT){
        InitTables(max_values);
        HashTable(R);
        HashJoin(S);
    }

    void HashJoinOnS(uint max_values = kMAX_INT){
        InitTables(max_values);
        HashTable(S);
        HashJoin(R);
    }

private:
    const int kMAX_INT = INT_MAX;
    const int kNUM_BUCKETS = 100;

    int *R, *S;
    size_t size_r_, size_s_;

    std::hash<int> hash_fn_;
    std::map<int, std::vector<int>> hash_buckets_;

    double time_hash_join_;

private:
    inline size_t Hash(int* value){
        return ( hash_fn_(*value) % kNUM_BUCKETS);
    }

    void HashTable(int* table);

    void HashJoin(int* table);

};


#endif //SORTMERGEJOIN_PARALLELHASHJOIN_HPP
