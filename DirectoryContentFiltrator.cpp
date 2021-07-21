#include "DirectoryContentFiltrator.h"
#include <iostream>

std::set<std::string> DirectoryContentFiltrator::FilterDirectories(set<fs::path> IncDirs, set<fs::path> ExcDirs, set<string> Masks, size_t ScanLvl, size_t MinSize)
{
    /* remove files from exclude directories */
    set<string> files = GetDirContents(IncDirs, ScanLvl);
    set<string> files_exc = GetDirContents(ExcDirs, 1);
    for (const auto& f : files_exc)
    {
        files.erase(f);
    }

    /* remove files that don't match the mask */
    set<string> no_match;
    for (const auto& f : files) {
        boost::smatch what;
        for (const auto& mask : Masks)
        {
            const boost::regex re(mask);
            if (!boost::regex_match(f, what, re))
            {
                no_match.insert(f);
            }
        }
    }
    for (const auto& f : Masks)
    {
        files.erase(f);
    }

    /* remove files that are too small */
    std::set<std::string> too_small;
    for (const auto& f : files)
    {
        struct stat fss;
        stat(f.c_str(), &fss);
        if (fss.st_size < MinSize) {
            too_small.insert(f);
        }
    }
    for (const auto& f : too_small)
    {
        files.erase(f);
    }
    return files;
}

unordered_map<size_t, vector<string>> DirectoryContentFiltrator::GetGroupsByFileSize(std::set<std::string>& files)
{
    unordered_map<size_t, vector<string>> file_groups; // groups are distinguished by size. if files dont have the same size, they cannot be similar
    for (const auto& f : files) {
        struct stat fss;
        stat(f.c_str(), &fss);
        file_groups[fss.st_size].push_back(f);
    }
    return file_groups;
}

std::set<std::string> DirectoryContentFiltrator::GetDirContents(const set<fs::path>& Dirs, size_t ScanLvl)
{
    std::set<std::string> files;
    for (const auto& dir : Dirs)
    {
        fs::recursive_directory_iterator it_rec(dir), end_rec;
        fs::directory_iterator it(dir), end;
        if (ScanLvl) {
            for (auto &entry : boost::make_iterator_range(it_rec, end_rec))
                if (is_regular(entry))
                    files.insert(entry.path().native());
        } else
        {
            for (auto &entry : boost::make_iterator_range(it, end))
                if (is_regular(entry))
                    files.insert(entry.path().native());
        }
    }
    return files;
}

