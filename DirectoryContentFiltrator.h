#pragma once

#ifndef DIRECTORY_CONTENT_FILTRATOR_H
#define DIRECTORY_CONTENT_FILTRATOR_H
#include "BoostIncludes.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <set>

using namespace std;
using namespace po;
using namespace fs;

class DirectoryContentFiltrator {
private:
    std::set<std::string> GetDirContents(const set<fs::path>& Dirs, size_t ScanLvl);
public:
    DirectoryContentFiltrator() = default;
    std::set<std::string> FilterDirectories(set<fs::path> IncDirs, set<fs::path> ExcDirs, std::set<std::string> Masks, size_t ScanLvl, size_t MinSize);
    unordered_map<size_t, vector<string>> GetGroupsByFileSize(std::set<std::string>& files);

};

#endif //DIRECTORY_CONTENT_FILTRATOR_H


