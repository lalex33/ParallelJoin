#include "ParallelHashJoin.hpp"

ParallelHashJoin::ParallelHashJoin(uint num_rows_r, uint num_rows_s, uint num_buckets, uint num_threads):
    size_r_(num_rows_r), size_s_(num_rows_s), num_buckets_(num_buckets), num_threads_(num_threads),
    threadpool_(num_threads), hash_buckets_(num_buckets)
{ }

ParallelHashJoin::~ParallelHashJoin() {
    delete[] R;
    delete[] S;
}

void ParallelHashJoin::ComputeHashJoin(int *r, size_t size_R, int *s, size_t size_S) {
    double start = sec();
    HashTable(r, size_R, hash_buckets_);
    time_hash_ = sec() - start;

    start = sec();
    HashJoin(s, size_S, results_);
    time_join_ = sec() - start;

    time_hash_join_ = time_hash_ + time_join_;
}

void ParallelHashJoin::ComputeParallelHashJoin(int *r, size_t size_R, int *s, size_t size_S) {
    double start = sec();
    ParallelHashTable(r, size_R);
    time_hash_ = sec() - start;

    start = sec();
    auto results = ParallelHashjoin(s, size_S);
    time_join_ = sec() - start;

    time_hash_join_ = time_hash_ + time_join_;
    results_ = assembleResults(results);
}

/*
 * Simple Hash Join
 */
void ParallelHashJoin::HashTable(int *table, size_t size, std::vector<Bucket> &hash_buckets) {
    int* end = table + size;
    double start = sec();

    while (table != end){
        auto bucket = &hash_buckets[ Hash(table) ];
        (*bucket).bucket_lock.lock();
        (*bucket).values.push_back(*table);
        (*bucket).bucket_lock.unlock();
        ++table;
    }

    std::cout << " hash table : " << (sec() - start) << std::endl;
}

void ParallelHashJoin::HashJoin(int *table, size_t size, std::vector<std::string> &results) {
    int* end = table + size, value;
    double start = sec();

    while(table != end){
        auto bucket = &hash_buckets_[ Hash(table) ].values;
        value = *table;

        for(auto tuple : (*bucket)){
            if(tuple == value){
                results.push_back(" [" + std::to_string(tuple) + " == " + std::to_string(value) + "]");
            }
        }

        ++table;
    }

    std::cout << " hash join : " << (sec() - start) << std::endl;
}


/*
 * Parallel Hash Join
 */
void ParallelHashJoin::ParallelHashTable(int *table, size_t size) {
    size_t num_rows = size / num_threads_;

    for(int thread = 0; thread < num_threads_; ++thread){
        int* start = table + thread * num_rows;
        size_t end_size = num_rows;

        if(thread == num_threads_ - 1){
            end_size += size % end_size;
        }

        threadpool_.Enqueue( std::bind(&ParallelHashJoin::HashTable, this, start, end_size,
                                       std::ref(hash_buckets_) ) );
    }

    threadpool_.WaitEndOfWork();
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

    if(verbose) {
        cout << "Good merge? " << checkMerge(R, size_r_, S, size_s_, results_) << endl;
        for (auto match : results_) {
            cout << match << endl;
        }
    }
}
