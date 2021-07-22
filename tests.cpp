#define BOOST_TEST_MODULE test
#include "CommandArgParser.h"
#include "DirectoryContentFiltrator.h"
#include "LazyFileGroupComparator.h"
#include <boost/test/included/unit_test.hpp>
using namespace std;
using namespace po;
using namespace fs;

struct ArgsFixture {
    ArgsFixture(): argc(boost::unit_test::framework::master_test_suite().argc),
                   argv(boost::unit_test::framework::master_test_suite().argv){}
    int argc;
    char **argv;
};

BOOST_AUTO_TEST_SUITE( TestSuite )

    struct cout_redirect {
        cout_redirect( std::streambuf * new_buffer )
                : old( std::cout.rdbuf( new_buffer ) )
        { }

        ~cout_redirect( ) {
            std::cout.rdbuf( old );
        }

    private:
        std::streambuf * old;
    };

    BOOST_FIXTURE_TEST_CASE ( test1, ArgsFixture ) {
        {
            boost::test_tools::output_test_stream output;
            {
                cout_redirect guard(output.rdbuf());

                int argc = 4;

                argv[0] = "/home/bbrave/Desktop/OTUS/homeworks/hw7/HW7/cmake-build-debug/test";
                argv[1] = "--inc_dirs=../tests/test_include_exclude/";
                argv[2] = "--exc_dirs=../tests/test_include_exclude/2/";
                argv[3] = "--scan_lvl=1";

                CommandArgParser parser(argc, argv);
                DirectoryContentFiltrator filtrator;
                std::set<std::string> files = filtrator.FilterDirectories(parser.GetIncludeDirectories(),
                                                                          parser.GetExcludeDirectories(),
                                                                          parser.GetMasks(), parser.GetScanLvl(),
                                                                          parser.GetMinFileSize());

                unordered_map<size_t, vector<string>> file_groups = filtrator.GetGroupsByFileSize(files);

                LazyFileGroupComparator L;
                for (const auto &it : file_groups) {
                    L.OutputSimilarInGroups(it.second, parser.GetBlockSize(),
                                            parser.GetHashFunction()); // function that compares files
                }
            }
            BOOST_CHECK(output.is_equal("../tests/test_include_exclude/1.txt\n"
                                           "../tests/test_include_exclude/1/1.txt\n"
                                           "\n"));
        }
    }
BOOST_AUTO_TEST_SUITE_END()
