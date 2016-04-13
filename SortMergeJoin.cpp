//
// Created by Alexis bayle on 13/04/2016.
//

#include "SortMergeJoin.h"

// launch a join and display result
void testSortMergeJoin() {
    srand(time(NULL)); // generate a seed

    vector<int> R;
    vector<int> S;

    // initialize relations with random data
    initRandomData(R, MAX_VALUE, R_SIZE);
    initRandomData(S, MAX_VALUE, S_SIZE);

    // sort the two relations
    sortRelation(R);
    sortRelation(S);

    // merge the two relations
    auto result = mergeRelations(R, S);

    // print datas
    printSortMerge(R, S, result);
}

/*
 * print rows of a table
 */
void printRelation(const vector<int> &relation){
    cout << "Row    | Value" << endl;
    int row=0;
    for(auto value : relation){
        cout << row++ << "      |  " << value << endl;
    }
}

/*
 * print sorted relations then the result of sort-merge join
 */
void printSortMerge(const vector<int> &r, const vector<int> &s, const vector<string> &result){
    cout << "Table R sorted" << endl;
    printRelation(r);
    cout << endl << endl;

    cout << "Table S sorted" << endl;
    printRelation(s);
    cout << endl << endl;

    cout << "-- Results --" << endl << "R rows  |  S rows" << endl;
    for(auto match : result){
        cout << match << endl;
    }
}

/*
 * fill a table of <nbRows> rows with random integer between 0 and <maxValue>
 */
void initRandomData(vector<int> &relation, int maxValue, int nbRows){
    int i=0;
    while(i < nbRows){
        int value = rand() % maxValue;
        relation.push_back(value);
        i++;
    }
}

/*
 * sort a table with STL function
 */
void sortRelation(vector<int> &relation){
    sort(relation.begin(), relation.end());
}

// implementation of sort-merge-algorithm : http://www.dcs.ed.ac.uk/home/tz/phd/thesis/node20.htm
vector<string> mergeRelations(const vector<int> &r, const vector<int> &s){
    // init vars
    vector<string> result;
    string match;

    auto tupleR = r.begin();
    auto tupleS = s.begin();
    auto endR = r.end();
    auto endS = s.end();

    int rowR=0, rowS=0;

    // loop until there is no tuples to read in both relation
    while(tupleR != endR && tupleS != endS){
        if(*tupleR > *tupleS){
            // move to a greater tuple in S
            tupleS++;
            rowS++;
        } else if(*tupleR < *tupleS){
            // move to a greater tuple in R
            tupleR++;
            rowR++;
        } else{
            // two equal tuples found -> record the rows
            match = to_string(rowR) + "       |    " + to_string(rowS);
            result.push_back(match);

            // loop on s to find other equal tuples after
            auto tupleS2 = tupleS + 1;
            auto rowS2 = rowS + 1;

            while(tupleS2 != endS && *tupleS2 == *tupleR){
                match = to_string(rowR) + "       |    " + to_string(rowS2);
                result.push_back(match);
                tupleS2++;
            }

            // loop on r to find other equal tuples after
            auto tupleR2 = tupleR + 1;
            auto rowR2 = rowR + 1;

            while(tupleR2 != endR && *tupleR2 == *tupleS){
                match = to_string(rowR2) + "       |    " + to_string(rowS);
                result.push_back(match);
                tupleR2++;
            }

            // go to higher tuples
            tupleR++;
            rowR++;

            tupleS++;
            rowS++;
        }
    }

    return result;
}