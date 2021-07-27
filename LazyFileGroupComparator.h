#include "BoostIncludes.h"

#ifndef BAYAN_LAZYFILEGROUPCOMPARATOR_H
#define BAYAN_LAZYFILEGROUPCOMPARATOR_H

using namespace std;
using namespace po;
using namespace fs;

/*Can only compare files of the same sizes*/
class LazyFileGroupComparator {
private:
    map<string, function<size_t(string)>> mHashLib;
    void RemoveNonDupKeys(unordered_map<size_t, vector<std::ifstream*>>& hash);
public:
    LazyFileGroupComparator();
    void OutputSimilarInGroups(vector<string> files, size_t block_size, string hash_func);
};


#endif //BAYAN_LAZYFILEGROUPCOMPARATOR_H
