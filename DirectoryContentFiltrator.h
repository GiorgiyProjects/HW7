#pragma once

#ifndef DIRECTORY_CONTENT_FILTRATOR_H
#define DIRECTORY_CONTENT_FILTRATOR_H

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <set>
namespace fs = boost::filesystem;
using namespace std;

class DirectoryContentFiltrator {
private:
    std::set<std::string> GetDirContents(const set<fs::path>& Dirs, size_t ScanLvl);
public:
    DirectoryContentFiltrator() = default;
    std::set<std::string> FilterDirectories(set<fs::path> IncDirs, set<fs::path> ExcDirs, std::set<std::string> Masks, size_t ScanLvl, size_t MinSize);
    unordered_map<size_t, vector<string>> GetGroupsBySize(std::set<std::string>& files);

};

#endif //DIRECTORY_CONTENT_FILTRATOR_H


