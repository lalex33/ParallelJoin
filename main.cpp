#include "Statistic.h"

#ifdef __linux__
#include "/home/bayle/papi/include/papi.h"
#endif

using namespace SMJ;

int main(){

    #ifdef __linux__
        int events[1] = {PAPI_L1_DCM}, ret;
        long_long values[1];

        if (PAPI_num_counters() < 2) {
            fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
            exit(1);
        }
        if ((ret = PAPI_start_counters(events, 1)) != PAPI_OK) {
            fprintf(stderr, "PAPI failed to start counters: %s\n", PAPI_strerror(ret));
            exit(1);
        }
    #endif

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merge between two tables
    //benchmarkSMJ();

    // set number of thread
    // NB_THREAD < NB_ROWS
    NB_THREAD = 4;

    // shows example of parallel sort-merge join
    //testParallelSort(false);
    //testParallelJoin();

    //testLotOfData();

    //benchmarkData();
    //benchmarkData2();
    //benchmarkMerge();

    // launch test of parallel sort-merge
    //benchmarkParallelSMJ();
    benchmarkThreadPSMJ();

    #ifdef __linux__
        if ((ret = PAPI_read_counters(values, 1)) != PAPI_OK) {
            fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
            exit(1);
        }

        printf("L2 data cache misses is %lld\n", values[0]);
    #endif

    return 0;
}