#include "CommandArgParser.h"
#include<iostream>

CommandArgParser::CommandArgParser(int argc, char *argv[]) {
    mDesc.add_options()
            ("help,h", "This screen")
            ("inc_dirs", po::value<strings>()->multitoken(), "list of include dirs")
            ("exc_dirs", po::value<strings>()->multitoken(), "list of dirs to exclude")
            ("scan_lvl", po::value<std::size_t>()->default_value(1), "1 - scan subchildren for files, 0 - dont" )
            ("min_fs", po::value<std::size_t>()->default_value(1), "minimal file size (default =1)" )
            ("masks",  po::value<strings>()->multitoken(), "list of masks to filer by")
            ("block_size", po::value<std::size_t>()->default_value(5), "size f block for file comparison" )
            ("hash_func", po::value<std::string>()->default_value("crc32"), "hashing function");
    po::store(po::parse_command_line(argc, argv, mDesc, po::command_line_style::default_style), mVarMap);
    po::notify(mVarMap);
    return;
}

set<fs::path> CommandArgParser::GetIncludeDirectories() {
    if (mIncDirs.empty()) {
        if (mVarMap.count("inc_dirs")) {
            for (const auto &a : mVarMap["inc_dirs"].as<strings>())
                mIncDirs.insert(a);
        } else {
            mIncDirs.insert("../inputs/");
        }
    }
    return mIncDirs;
}

set<fs::path> CommandArgParser::GetExcludeDirectories() {
    if (mExcDirs.empty()) {
        if (mVarMap.count("exc_dirs")) {
            for (const auto &a : mVarMap["exc_dirs"].as<strings>())
                mExcDirs.insert(a);
        }
    }
    return mExcDirs;
}

std::set<string> CommandArgParser::GetMasks() {
    if (mMasks.empty()) {
        if (mVarMap.count("masks")) {
            for (const auto &a : mVarMap["masks"].as<strings>())
                mMasks.insert(a);
        }
    }
    return mMasks;
}

size_t CommandArgParser::GetScanLvl() {
    if (mVarMap.count("scan_lvl"))
        return mVarMap["scan_lvl"].as<size_t>();
    return 1;
}

size_t CommandArgParser::GetMinFileSize() {
    if (mVarMap.count("min_fs"))
        return mVarMap["min_fs"].as<size_t>();
    return 1;
}

size_t CommandArgParser::GetBlockSize() {
    if (mVarMap.count("block_size"))
        return mVarMap["block_size"].as<size_t>();
    return 5;
}

string CommandArgParser::GetHashFunction() {
    if (mVarMap.count("block_size"))
        return mVarMap["hash_func"].as<string>();
    return("crc32");
}
