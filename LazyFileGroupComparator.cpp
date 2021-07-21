#include <iostream>
#include "LazyFileGroupComparator.h"

// todo: add other hashing functions
void LazyFileGroupComparator::RemoveNonDupKeys(unordered_map<string, vector<std::ifstream*>>& hash)
{
    vector<string> to_remove;
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

string ConvertCrc32(string s) {
    boost::crc_32_type result;
    result.process_bytes(s.data(), s.length());
    return s;
    //return (result.checksum());
}

LazyFileGroupComparator::LazyFileGroupComparator() {
    // to do: add other hash functions to the lib
    mHashLib["crc32"] = ConvertCrc32;
}

void LazyFileGroupComparator::OutputSimilarInGroups(vector<string> files, size_t block_size, string hash_func)
{
    if (files.size() <= 1) return; // if group consists of a single file skip it
    unordered_map<string, vector<std::ifstream*>> hash; // this has will store the previous generation of groups of similar files
    // open all files in a group for reading
    unordered_map<std::ifstream*, string> descr_to_name;
    for (const auto& file:files) {
        std::ifstream* f = new std::ifstream(file, std::ios::in);
        descr_to_name[f] = file;
        hash[0].push_back(f);  // initially we assume they are all in the same group
    }

    while(!((*descr_to_name.begin()).first)->eof()) {
        RemoveNonDupKeys(hash);
        if (hash.empty()) return;

        unordered_map<string, vector<std::ifstream*>> hash_new_gen; // we do not store previous keys, only store a new one
        for (const auto& obj : hash)
        {
            for (const auto &f:obj.second) {
                char c[block_size];
                f->read(c, block_size);
                string key = mHashLib[hash_func](c);
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