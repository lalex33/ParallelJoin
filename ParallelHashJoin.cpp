#include "ParallelHashJoin.hpp"

ParallelHashJoin::ParallelHashJoin(uint num_rows_r, uint num_rows_s, uint num_buckets, uint num_threads):
    size_r_(num_rows_r), size_s_(num_rows_s), num_buckets_(num_buckets), num_threads_(num_threads),
    threadpool_(num_threads)
{ }

ParallelHashJoin::~ParallelHashJoin() {
    delete[] R;
    delete[] S;
}

void ParallelHashJoin::ComputeHashJoin(int *r, size_t size_R, int *s, size_t size_S) {
    double start = sec();
    HashTable(r, size_R);
    HashJoin(s, size_S);
    time_hash_join_ = sec() - start;
}

void ParallelHashJoin::ComputeParallelHashJoin(int *r, size_t size_R, int *s, size_t size_S) {
    double start = sec();
    ParallelHashTable(r, size_R);
    ParallelHashjoin(s, size_S);
    time_hash_join_ = sec() - start;
}

/*
 * Simple Hash Join
 */

void ParallelHashJoin::HashTable(int *table, size_t size) {
    int* end = table + size;

    while (table != end){
        hash_buckets_[ Hash(table) ].push_back( *table );
        ++table;
    }
}

void ParallelHashJoin::HashJoin(int *table, size_t size) {
    int* end = table + size;

    while(table != end){
        auto bucket = hash_buckets_[ Hash(table) ];
        int value = *table;

        for(auto tuple : bucket){
            if(tuple == value){
                results_.push_back(" [" + std::to_string(tuple) + " == " + std::to_string(value) + "]");
            }
        }

        ++table;
    }
}

/*
 * Parallel Hash Join
 */

void ParallelHashJoin::ParallelHashTable(int *table, size_t size) {

}

void ParallelHashJoin::ParallelHashjoin(int *table, size_t size) {

}

/*
 * Print the result
 * verbose : display all results if true
 */
void ParallelHashJoin::PrintResult(bool verbose) {
    using std::cout;
    using std::endl;

    if(verbose) {
        cout << "Table R : " << endl;
        printTable(R, size_r_);
        cout << endl;

        cout << "Table S : " << endl;
        printTable(S, size_s_);
        cout << endl;
    }

    cout << "Execution time : " << GetProcessingTime() << endl;
    cout << "Good merge? " << checkMerge(R, size_r_, S, size_s_, results_) << endl;

    if(verbose) {
        for (auto match : results_) {
            cout << match << endl;
        }
    }
}
