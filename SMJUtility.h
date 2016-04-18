#ifndef SORTMERGEJOIN_SMJUTILITY_H
#define SORTMERGEJOIN_SMJUTILITY_H

#include <iostream>
#include <vector>

using namespace std;

namespace SMJ {

    /*
     * fill a table of rows with random integer between 0 and <maxValue>
     */
    void fillTable(int *table, size_t size, uint maxValue) {
        for (int i = 0; i < size; i++) {
            table[i] = rand() % maxValue;
        }
    }

    /*
     * return true if an array is sorted
     */
    bool checkSorted(int *table, size_t size) {
        int *p = table, *end = p + size - 1;
        while (p != end) {
            // check that the previous is lower than the next
            if (*p > *(p + 1))
                return false;
            p++;
        }
        return true;
    }

    /*
     * print rows of a table
     */
    void printTable(int *table, size_t size) {
        int *p = table, *end = p + size, row = 0;
        while (p != end) {
            cout << row++ << " : " << *(p++) << endl;
        }
    }

    /*
     * print sorted relations then the result of sort-merge join
     */
    void printSortMerge(int *r, int *s, int sizeR, int sizeS, const vector<string> &result) {
        cout << "Table R sorted" << endl;
        printTable(r, sizeR);
        cout << endl << endl;

        cout << "Table S sorted" << endl;
        printTable(s, sizeS);
        cout << endl << endl;

        cout << "----- Results -----" << endl << "R rows  |  S rows" << endl;
        for (auto match : result) {
            cout << match << endl;
        }
    }

    /*
     * print a match
     */
    string getTuple(int rowR, int *tupleR, int rowsS, int *tupleS) {
        return "[" + to_string(rowR) + "] : " + to_string(*tupleR)
               + "   |   [" + to_string(rowsS) + "] : " + to_string(*tupleS);
    }
}

#endif //SORTMERGEJOIN_SMJUTILITY_H
