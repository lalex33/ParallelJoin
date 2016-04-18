#ifndef SORTMERGEJOIN_SMJUTILITY_H
#define SORTMERGEJOIN_SMJUTILITY_H

#include <iostream>
#include <vector>

namespace SMJ {

    /*
     * fill a table of rows with random integer between 0 and <maxValue>
     */
    void fillTable(int *table, size_t size, uint maxValue);

    /*
     * return true if an array is sorted
     */
    bool checkSorted(int *table, size_t size);

    /*
     * print rows of a table
     */
    void printTable(int *table, size_t size);

    /*
     * print sorted relations then the result of sort-merge join
     */
    void printSortMerge(int *r, int *s, int sizeR, int sizeS, const std::vector<std::string> &result);

    /*
     * print a match
     */
    std::string getTuple(int rowR, int *tupleR, int rowsS, int *tupleS);
}

#endif //SORTMERGEJOIN_SMJUTILITY_H
