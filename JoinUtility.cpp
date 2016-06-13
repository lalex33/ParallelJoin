#include "JoinUtility.h"

using namespace std;

void fillTable(int *table, size_t size, uint maxValue) {
    for (int i = 0; i < size; i++) {
        table[i] = rand() % maxValue;
    }
}

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

bool checkMerge(int* R, int sizeR, int* S, int sizeS, vector<string> resultFound){
    // use nested loop merge
    vector<string> result;
    int *tupleR = R, *endR = R + sizeR, *endS = S + sizeS, rowR = 0;

    while(tupleR != endR){
        int* tupleS = S, rowS = 0;
        while(tupleS != endS){
            if(*tupleR == *tupleS){
                result.push_back(getTuple(rowR, *tupleR, rowS, *tupleS));
            }
            tupleS++;
            rowS++;
        }
        tupleR++;
        rowR++;
    }

    return resultFound.size() == result.size();
}

void printTable(int *table, size_t size) {
    int *p = table, *end = p + size, row = 0;
    while (p != end) {
        cout << row++ << " : " << *(p++) << endl;
    }
}

void printSortMerge(int *r, int *s, int sizeR, int sizeS, const vector<string> &result) {
    cout << "Table R sorted" << endl;
    printTable(r, sizeR);
    cout << endl << endl;

    cout << "Table S sorted" << endl;
    printTable(s, sizeS);
    cout << endl << endl;

    cout << "----- Results -----" << endl << "R rows  |  S rows" << endl;
    for (auto match = result.begin(); match != result.end(); match++) {
        cout << (*match) << endl;
    }
}

string getTuple(int rowR, int tupleR, int rowsS, int tupleS) {
    ostringstream oss;
    oss << "[" << rowR << "] : " << tupleR
          << "   |   [" << rowsS << "] : " << tupleS;
    return oss.str();
}

vector<string> assembleResults(vector<vector<string>> results){
    vector<string> result;
    for(auto match = results.begin(); match != results.end(); ++match){
        for(auto value = match->begin(); value != match->end(); ++value){
            result.push_back(*value);
        }
    }
    return result;
}
