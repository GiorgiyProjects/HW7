
using namespace std;
#include <iostream>
#include <boost/crc.hpp>
#include "CommandArgParser.h"
#include "DirectoryContentFiltrator.h"

#include <fstream>


using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;



int GetCrc32(const string& my_string) {
    boost::crc_32_type result;
    result.process_bytes(my_string.data(), my_string.length());
    return result.checksum();
}

// todo: add other hashing functions
void RemoveNonDupKeys(unordered_map<int, vector<ifstream*>>& hash)
{
    vector<int> to_remove;
    for (const auto& obj:hash) {
        if (obj.second.size() <= 1)
            to_remove.push_back(obj.first);
    }
    for (const auto& key:to_remove)
    {
        hash.erase(key);
    }
    return;
}

void OutputSimilarFileGroups(vector<string> files, size_t block_size, string hash_func)
{
    unordered_map<int, vector<ifstream*>> hash; // this has will store the previous generation of groups of similar files
    // open all files in a group for reading
    unordered_map<ifstream*, string> descr_to_name;
    for (const auto& file:files) {
        std::ifstream* f = new std::ifstream(file, std::ios::in);
        descr_to_name[f] = file;
        hash[0].push_back(f);  // initially we assume they are all in the same group
    }

    while(!((*descr_to_name.begin()).first)->eof()) {
        RemoveNonDupKeys(hash);
        if (hash.empty()) return;

        unordered_map<int, vector<ifstream*>> hash_new_gen; // we do not store previous keys, only store a new one
        for (const auto& obj : hash)
        {
            for (const auto &f:obj.second) {
                char c[block_size];
                f->read(c, block_size);
                int key = GetCrc32(c);
                hash_new_gen[key].push_back(f);
            }
        }
        hash = hash_new_gen;
    }

    RemoveNonDupKeys(hash);
    if (hash.empty()) return;
    // now output all the similar file names
    for (const auto& obj : hash)
    {
        for (const auto &f:obj.second) {
            cout << descr_to_name[f] << endl;
        }
        cout << endl;
    }
}

int main(int argc, const char *argv[]) {
    CommandArgParser parser(argc, argv);

    DirectoryContentFiltrator filtrator;
    std::set<std::string> files = filtrator.FilterDirectories(parser.GetIncludeDirectories(), parser.GetExcludeDirectories(), parser.GetMasks(), parser.GetScanLvl(), parser.GetMinFileSize());

    cout << "List of files to be compared" << endl;
    for (const auto& f : files)
    {
        cout << f << endl;
    }

    unordered_map<size_t, vector<string>> file_groups = filtrator.GetGroupsBySize(files);
    cout << "Groups of similar files" << endl;
    for (const auto& it : file_groups)
    {
        if (it.second.size() <= 1) continue; // if group consists of a single file skip it
        OutputSimilarFileGroups(it.second, parser.GetBlockSize(), parser.GetHashFunction()); // function that compares files
    }

    return 0;
}