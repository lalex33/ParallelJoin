#include "Statistic.h"

#ifdef __linux__
#include "/home/bayle/papi/include/papi.h"
#endif

using namespace SMJ;

int main(){

    #ifdef __linux__
        const int NB_EVENTS = 1;
        int events[NB_EVENTS] = {PAPI_L1_DCM};
        int ret;
        long_long values[NB_EVENTS];

        if (PAPI_num_counters() < NB_EVENTS) {
            fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
            exit(1);
        }
        if ((ret = PAPI_start_counters(events, NB_EVENTS)) != PAPI_OK) {
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
        using namespace std;
        if ((ret = PAPI_read_counters(values, NB_EVENTS)) != PAPI_OK) {
            fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
            exit(1);
        }
        for(int i=0; i<NB_EVENTS; ++i){
            cout << "values[" << i << "] = " << values[i] << endl;
        }
    #endif

    return 0;
}