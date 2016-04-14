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
void Table::addRow(const Row &row) {
    rows.push_back(row);
}

/*
 * generate <nbRows> rows
 */
void Table::addRandomRows(size_t nbRows) {
    for(size_t nbRow = rows.size(); nbRow < nbRows + rows.size(); nbRow++){
        Row row;

        // add data
        row.rowId = nbRow;
        for(int column=0; column<nbColumns; column++){
            row.values.push_back("random string here");
        }

        // add the row generated
        addRow(row);
    }
}

/*
 * sort the table on one or more columns
 */
void Table::sort(unsigned int columnsToSort[]) {

}

/*
 * merge two tables on one or more columns
 */
Table Table::merge(const Table &table, unsigned int columnsToMerge[]) {
    return nullptr;
}
