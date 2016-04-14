//
// Created by Alexis bayle on 14/04/2016.
//

#include "Table.h"

using namespace std;

/*
 * constructor which defines table's number of columns
 */
Table::Table(size_t nbColumns): nbColumns(nbColumns){ }

/*
 * add a row to the table
 */
void Table::addRow(const Tuple &tuple) {
    rows.push_back(tuple);
}

/*
 * generate <nbRows> rows
 */
void Table::addRandomRows(size_t nbRows) {
    for(size_t row = rows.size() - 1; row < nbRows; row++){
        Tuple tuple;

        // add data
        tuple.rowId = row;
        for(int column=0; column<nbColumns; column++){
            tuple.values.push_back("random string here");
        }

        // add the row generated
        addRow(tuple);
    }
}

/*
 * sort the table based on the column
 */
void Table::sort(unsigned int columnsToSort[]) {

}

/*
 * merge two tables on one or more columns
 */
Table Table::merge(const Table &table, unsigned int columnsToMerge[]) {
    return nullptr;
}
