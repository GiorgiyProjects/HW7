//
// Created by bbrave on 7/22/21.
//

#include "MainManager.h"

MainManager::MainManager(int argc, char *argv[]) {
    CommandArgParser parser(argc, argv);
    DirectoryContentFiltrator filtrator;
    std::set<std::string> files = filtrator.FilterDirectories(parser.GetIncludeDirectories(),
                                                              parser.GetExcludeDirectories(),
                                                              parser.GetMasks(), parser.GetScanLvl(),
                                                              parser.GetMinFileSize());

    unordered_map<size_t, vector<string>> file_groups = filtrator.GetGroupsByFileSize(files);

    LazyFileGroupComparator L;
    for (const auto &it : file_groups) {
        L.OutputSimilarInGroups(it.second, parser.GetBlockSize(), parser.GetHashFunction());
    }
}
