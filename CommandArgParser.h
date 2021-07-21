#pragma once

#ifndef COMMAND_ARG_PARSER_H
#define COMMAND_ARG_PARSER_H
#include "BoostIncludes.h"
#include <set>

using namespace std;
using namespace po;
using namespace fs;
using strings = std::vector<std::string>;

class CommandArgParser {
private:
    po::options_description mDesc{"Options"};
    po::variables_map mVarMap;

    set<fs::path> mIncDirs;
    set<fs::path> mExcDirs;
    std::set<string> mMasks;

public:
    CommandArgParser(int argc, char *argv[]);
    set<fs::path> GetIncludeDirectories();
    set<fs::path> GetExcludeDirectories();
    std::set<string> GetMasks();
    size_t GetScanLvl();
    size_t GetMinFileSize();
    size_t GetBlockSize();
    string GetHashFunction();
};

#endif

