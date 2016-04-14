#include "Statistique.h"
#include "Table.h"

int main(){

    // shows an example of sort-merge join
    //testSortMergeJoin();

    // launch test of sort-merg between two tables
    launchPerformanceTest();

    // possible use of Table when it will be fully implemented
    /*Table R(3);
    Table S(3);

    R.addRandomRows(10);
    S.addRandomRows(10);

    uint columnR[2] = {0,2};
    R.sort(columnR);

    uint columnS[2] = {2,0};
    S.sort(columnS);

    Table result = R.merge(S, columnR, columnS);*/

    return 0;
}