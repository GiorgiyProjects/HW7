using namespace std;
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <iostream>
#include <boost/range.hpp>
#include <boost/program_options.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/crc.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>

#include <fstream>

#include <set>
using namespace std;

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using strings = std::vector<std::string>;

void DefineCommandLineArgs(po::options_description& desc)
{
    desc.add_options()
        ("help,h", "This screen")
        ("inc_dirs", po::value<strings>()->multitoken(), "list of include dirs")
        ("exc_dirs", po::value<strings>()->multitoken(), "list of dirs to exclude")
        ("scan_lvl", po::value<std::size_t>()->default_value(1), "1 - scan subchildren for files, 0 - dont" )
        ("min_fs", po::value<std::size_t>()->default_value(1), "minimal file size (default =1)" )
        ("masks",  po::value<strings>()->multitoken(), "list of masks to filer by")
        ("block_size", po::value<std::size_t>()->default_value(5), "size f block for file comparison" )
        ("hash_func", po::value<std::string>()->default_value("crc32"), "hashing function");
    return;
}


void GetDirContents(std::set<std::string>& files, const set<fs::path>& inc_dirs, size_t scan_lvl)
{
    for (const auto& dir : inc_dirs)
    {
        fs::recursive_directory_iterator it_rec(dir), end_rec;
        fs::directory_iterator it(dir), end;
        if (scan_lvl) {
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
    return;
}

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
    po::options_description desc{"Options"};
    DefineCommandLineArgs(desc);
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc, po::command_line_style::default_style), vm);
    po::notify(vm);

    if (vm.count("help"))
        std::cout << desc << '\n';

    set<fs::path> inc_dirs, exc_dirs;
    if (vm.count("inc_dirs"))
        for (const auto& a : vm["inc_dirs"].as<strings>() )
            inc_dirs.insert(a);

    if (vm.count("exc_dirs"))
        for (const auto& a : vm["exc_dirs"].as<strings>() )
            exc_dirs.insert(a);

    std::set<std::string> files, files_exc;
    /* remove files from exclude directories 8*/
    GetDirContents(files, inc_dirs, vm["scan_lvl"].as<size_t>());
    GetDirContents(files_exc, exc_dirs, true);
    for (const auto& f : files_exc)
    {
        files.erase(f);
    }

    /* remove files that don't match the mask */
    std::set<string> masks;
    std::set<std::string> no_match;
    if (vm.count("masks"))
        for (const auto& a : vm["masks"].as<strings>() )
            masks.insert(a);

    for (const auto& f : files) {
        boost::smatch what;
        for (const auto& mask : masks)
        {
            const boost::regex re(mask);
            if (!boost::regex_match(f, what, re))
            {
                no_match.insert(f);
            }
        }
    }
    for (const auto& f : no_match)
    {
        files.erase(f);
    }

    /* remove files that are too small */
    std::set<std::string> too_small;
    unordered_map<size_t, vector<string>> file_groups; // groups are distinguished by size. if files dont have the same size, they cannot be similar
    for (const auto& f : files)
    {
        struct stat fs;
        stat(f.c_str(), &fs);
        if (fs.st_size < vm["min_fs"].as<size_t>()) {
            too_small.insert(f);
        } else {
            file_groups[fs.st_size].push_back(f);
        }
    }
    for (const auto& f : too_small)
    {
        files.erase(f);
    }

    cout << "List of files to be compared" << endl;
    for (const auto& f : files)
    {
        cout << f << endl;
    }

    cout << "Groups of similar files" << endl;
    for (const auto& it : file_groups)
    {
        if (it.second.size() <= 1) continue; // if group consists of a single file skip it
        OutputSimilarFileGroups(it.second, vm["block_size"].as<size_t>(), vm["hash_func"].as<string>()); // function that compares files
    }

    return 0;
}