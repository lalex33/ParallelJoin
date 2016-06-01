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
    HashTable(r, size_R, hash_buckets_);
    HashJoin(s, size_S, results_);
    time_hash_join_ = sec() - start;
}

void ParallelHashJoin::ComputeParallelHashJoin(int *r, size_t size_R, int *s, size_t size_S) {
    double start = sec();
    ParallelHashTable(r, size_R);
    auto results = ParallelHashjoin(s, size_S);
    time_hash_join_ = sec() - start;

    results_ = assembleResults(results);
}

/*
 * Simple Hash Join
 */

void ParallelHashJoin::HashTable(int *table, size_t size, std::map<int, std::vector<int>> &hash_buckets) {
    int* end = table + size;

    while (table != end){
        hash_buckets[ Hash(table) ].push_back( *table );
        ++table;
    }
}

void ParallelHashJoin::HashJoin(int *table, size_t size, std::vector<std::string> &results) {
    int* end = table + size, value;

    while(table != end){
        auto bucket = hash_buckets_[ Hash(table) ];
        value = *table;

        for(auto tuple : bucket){
            if(tuple == value){
                results.push_back(" [" + std::to_string(tuple) + " == " + std::to_string(value) + "]");
            }
        }

        ++table;
    }
}

/*
 * Parallel Hash Join
 */

void ParallelHashJoin::ParallelHashTable(int *table, size_t size) {
    size_t num_rows = size / num_threads_;

    std::vector< std::map< int, std::vector<int> > > hash_tables(num_threads_);

    for(int thread = 0; thread < num_threads_; ++thread){
        int* start = table + thread * num_rows;
        size_t end_size = num_rows;

        if(thread == num_threads_ - 1){
            end_size += size % end_size;
        }

        threadpool_.Enqueue( std::bind(&ParallelHashJoin::HashTable, this, start, end_size,
                                       std::ref(hash_tables[thread]) ) );
    }

    threadpool_.WaitEndOfWork();

    for(auto const &hash_table : hash_tables){
        for(auto const &map : hash_table){
            for(auto const &tuple : map.second) {
                hash_buckets_[map.first].push_back( tuple );
            }
        }
    }
}

std::vector<std::vector<std::string>> ParallelHashJoin::ParallelHashjoin(int *table, size_t size) {
    size_t num_rows = size / num_threads_;

    std::vector<std::vector<std::string>> results(num_threads_);

    for(int thread = 0; thread < num_threads_; ++thread){
        int* start = table + thread * num_rows;
        size_t end_size = num_rows;

        if(thread == num_threads_ - 1){
            end_size += size % end_size;
        }

        threadpool_.Enqueue( std::bind(&ParallelHashJoin::HashJoin, this, start, end_size,
                                       std::ref(results[thread]) ) );
    }

    threadpool_.WaitEndOfWork();

    return results;
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
