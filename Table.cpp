#include <stdlib.h>
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
            //row.values.push_back(random_string(FIELD_MAX_LENGTH));
        }

        // add the row generated
        addRow(row);
    }
}

/*
 * NOT IMPLEMENTED YET
 * sort the table on one or more columns
 */
void Table::sort(uint columnsToSort[]) {

}

/*
 * NOT IMPLEMENTED YET
 * merge two tables on one or more columns
 */
Table Table::merge(const Table &table, uint columnsToMergeR1[], uint columnsTomergeR2[]) {
    return Table(5);
}

/*
 * generate a random string
 */
string random_string(size_t length) {
    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    string str(length, 0);
    generate_n(str.begin(), length, randchar);
    return str;
}

void testTableSortMergeJoin() {
    Table R(3);
    Table S(3);

    R.addRandomRows(10);
    S.addRandomRows(10);

    uint columnR[2] = {0,2};
    R.sort(columnR);

    uint columnS[2] = {2,0};
    S.sort(columnS);

    Table result = R.merge(S, columnR, columnS);
}
