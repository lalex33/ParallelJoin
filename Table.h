//
// Created by Alexis bayle on 14/04/2016.
//

#ifndef SORTMERGEJOIN_TABLE_H
#define SORTMERGEJOIN_TABLE_H

#include <vector>
#include <string>

struct Row {
    size_t rowId;
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

    void sort(unsigned int columnsToSort[]);

    Table merge(const Table &table, unsigned int columnsToMerge[]);

};


#endif //SORTMERGEJOIN_TABLE_H
