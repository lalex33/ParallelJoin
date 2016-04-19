#ifndef SORTMERGEJOIN_SORTMERGEJOIN_H
#define SORTMERGEJOIN_SORTMERGEJOIN_H

#include "SMJUtility.h"

namespace SMJ{

    /*
     * sort a table with STL function
     */
    void sortRelation(int *start, int *end);

    /*
     * implementation of sort-merge-algorithm based on http://www.dcs.ed.ac.uk/home/tz/phd/thesis/node20.htm
     */
    void mergeRelations(int *startR, int *endR, int *startS, int *endS,
                        std::vector<std::string> &results, uint rowR, uint rowS);

}

#endif //SORTMERGEJOIN_SORTMERGEJOIN_H
