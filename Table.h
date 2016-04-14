#ifndef SORTMERGEJOIN_TABLE_H
#define SORTMERGEJOIN_TABLE_H

#include <vector>
#include <string>

typedef unsigned int uint;
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
    Table(size_t nbColumns);

    void addRow(const Row &row);

    void addRandomRows(size_t nbRows);

    void sort(uint columnsToSort[]);

    Table merge(const Table &table, uint columnsToMergeR1[], uint columnsTomergeR2[]);

    // http://stackoverflow.com/a/12468109
    friend std::string random_string( size_t length );

    friend void testTableSortMergeJoin();
};


#endif //SORTMERGEJOIN_TABLE_H
