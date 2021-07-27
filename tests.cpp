#define BOOST_TEST_MODULE test
#include <boost/test/included/unit_test.hpp>
#include "MainManager.h"
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
                int argc = 3;
                string argv0,argv1,argv2;
                argv0 = "/home/bbrave/Desktop/OTUS/homeworks/hw7/HW7/cmake-build-debug/test";
                argv1 = "--inc_dirs=../tests/test_include_exclude/";
                argv2 = "--scan_lvl=1";
                argv[0] = const_cast<char*>(argv0.c_str());
                argv[1] = const_cast<char*>(argv1.c_str());
                argv[2] = const_cast<char*>(argv2.c_str());
                MainManager(argc, argv);
            }
            BOOST_CHECK(output.is_equal("../tests/test_include_exclude/1.txt\n"
                                           "../tests/test_include_exclude/1/1.txt\n"
                                           "../tests/test_include_exclude/2/1.txt\n"
                                           "\n"));
        }
    }

    BOOST_FIXTURE_TEST_CASE ( test2, ArgsFixture ) {
        {
            boost::test_tools::output_test_stream output;
            {
                cout_redirect guard(output.rdbuf());
                int argc = 4;
                string argv0,argv1,argv2,argv3;
                argv0 = "/home/bbrave/Desktop/OTUS/homeworks/hw7/HW7/cmake-build-debug/test";
                argv1 = "--inc_dirs=../tests/test_include_exclude/";
                argv2 = "--exc_dirs=../tests/test_include_exclude/2/";
                argv3 = "--scan_lvl=1";

                argv[0] = const_cast<char*>(argv0.c_str());
                argv[1] = const_cast<char*>(argv1.c_str());
                argv[2] = const_cast<char*>(argv2.c_str());
                argv[3] = const_cast<char*>(argv3.c_str());

                MainManager(argc, argv);
            }
            BOOST_CHECK(output.is_equal("../tests/test_include_exclude/1.txt\n"
                                        "../tests/test_include_exclude/1/1.txt\n"
                                        "\n"));
        }
    }
    // to check if scan lvl flag = 0 works
    BOOST_FIXTURE_TEST_CASE ( test3, ArgsFixture ) {
        {
            boost::test_tools::output_test_stream output;
            {
                cout_redirect guard(output.rdbuf());
                int argc = 3;
                string argv0, argv1, argv2;
                argv0 = "/home/bbrave/Desktop/OTUS/homeworks/hw7/HW7/cmake-build-debug/test";
                argv1 = "--inc_dirs=../tests/test_scan_lvl/";
                argv2 = "--scan_lvl=0";

                argv[0] = const_cast<char*>(argv0.c_str());
                argv[1] = const_cast<char*>(argv1.c_str());
                argv[2] = const_cast<char*>(argv2.c_str());

                MainManager(argc, argv);
            }
            BOOST_CHECK(output.is_equal("../tests/test_scan_lvl/1_1.txt\n"
                                           "../tests/test_scan_lvl/1_2.txt\n"
                                           "\n"));
        }
    }
    // check if mask works correctly
    BOOST_FIXTURE_TEST_CASE ( test4, ArgsFixture ) {
        {
            boost::test_tools::output_test_stream output;
            {
                cout_redirect guard(output.rdbuf());
                int argc = 3;
                string argv0, argv1, argv2;
                argv0 = "/home/bbrave/Desktop/OTUS/homeworks/hw7/HW7/cmake-build-debug/test";
                argv1 = "--inc_dirs=../tests/test_mask/";
                argv2 = "--mask=^[0-9]+$";

                argv[0] = const_cast<char*>(argv0.c_str());
                argv[1] = const_cast<char*>(argv1.c_str());
                argv[2] = const_cast<char*>(argv2.c_str());

                MainManager(argc, argv);
            }
            BOOST_CHECK(output.is_equal("../tests/test_mask/1\n"
                                        "../tests/test_mask/12\n"
                                        "\n"));
        }
    }
    // big test
    BOOST_FIXTURE_TEST_CASE ( test5, ArgsFixture ) {
        {
            boost::test_tools::output_test_stream output;
            {
                cout_redirect guard(output.rdbuf());
                int argc = 2;
                string argv0, argv1, argv2;
                argv0 = "/home/bbrave/Desktop/OTUS/homeworks/hw7/HW7/cmake-build-debug/test";
                argv1 = "--inc_dirs=../tests/test_big/";
                argv2 = "--exc_dirs=../tests/test_big/2/";

                argv[0] = const_cast<char*>(argv0.c_str());
                argv[1] = const_cast<char*>(argv1.c_str());
                argv[2] = const_cast<char*>(argv2.c_str());

                MainManager(argc, argv);
            }
            BOOST_CHECK(output.is_equal(
                    "../tests/test_big/1.1\n"
                    "../tests/test_big/1/1.1/1.2\n"
                    "../tests/test_big/3/3.1/1.3\n"
                    "\n"
                    "../tests/test_big/1/1.1/2.2\n"
                    "../tests/test_big/1/2.1\n"
                    "../tests/test_big/2/2.3\n"
                    "../tests/test_big/2/2.4\n"
                    "../tests/test_big/3/2.5\n"
                    "\n"
                    ));
        }
    }
BOOST_AUTO_TEST_SUITE_END()
