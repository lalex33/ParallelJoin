#ifndef SORTMERGEJOIN_TABLE_H
#define SORTMERGEJOIN_TABLE_H

#include "SMJUtility.h"

namespace SMJ{
    const size_t FIELD_MAX_LENGTH = 10;

    struct Row {
        std::size_t rowId;
        std::vector<std::string> values;
    };

    class Table {

    private:
        std::vector<Row> rows;
        size_t nbColumns;

    public:
        /*
         * constructor which defines table's number of columns
         */
        Table(size_t nbColumns);

        /*
         * add a row to the table
         */
        void addRow(const Row &row);

        /*
         * generate <nbRows> rows
         */
        void addRandomRows(size_t nbRows);

        /*
         * NOT IMPLEMENTED YET
         * sort the table on one or more columns
         */
        void sort(uint columnsToSort[]);

        /*
         * NOT IMPLEMENTED YET
         * merge two tables on one or more columns
         */
        Table merge(const Table &table, uint columnsToMergeR1[], uint columnsTomergeR2[]);

        /*
         * generate a random string
         * http://stackoverflow.com/a/12468109
         */
        friend std::string random_string( size_t length );

        friend void testTableSortMergeJoin();
    };
}

#endif //SORTMERGEJOIN_TABLE_H
