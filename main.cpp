using namespace std;
#include <iostream>
#include "CommandArgParser.h"
#include "DirectoryContentFiltrator.h"
#include "LazyFileGroupComparator.h"
using namespace std;
using namespace po;
using namespace fs;

int main(int argc, char *argv[]) {
    CommandArgParser parser(argc, argv);
    DirectoryContentFiltrator filtrator;
    std::set<std::string> files = filtrator.FilterDirectories(parser.GetIncludeDirectories(), parser.GetExcludeDirectories(), parser.GetMasks(), parser.GetScanLvl(), parser.GetMinFileSize());

    cout << "List of files to be compared" << endl;
    for (const auto& f : files)
    {
        cout << f << endl;
    }

    unordered_map<size_t, vector<string>> file_groups = filtrator.GetGroupsByFileSize(files);
    cout << "Groups of similar files" << endl;

    LazyFileGroupComparator L;
    for (const auto& it : file_groups)
    {
        L.OutputSimilarInGroups(it.second, parser.GetBlockSize(), parser.GetHashFunction()); // function that compares files
    }

    return 0;
}