cmake_minimum_required(VERSION 3.5)

project(bayan)

set(CMAKE_CXX_STANDARD 17)
# set(BOOST_SYSTEM "${BOOST_LATEST_BIN_DIR}/libboost_system.so")
# include_directories(${CMAKE_SOURCE_DIR} ${BOOST_SYSTEM})
# add_definitions(-DBOOST_ERROR_CODE_HEADER_ONLY)

find_package(Boost 1.65.0 REQUIRED COMPONENTS thread system filesystem program_options regex unit_test_framework)
add_executable(bayan main.cpp CommandArgParser.cpp CommandArgParser.h DirectoryContentFiltrator.cpp DirectoryContentFiltrator.h BoostIncludes.h LazyFileGroupComparator.cpp LazyFileGroupComparator.h MainManager.cpp MainManager.h)
add_executable(test tests.cpp CommandArgParser.cpp CommandArgParser.h DirectoryContentFiltrator.cpp DirectoryContentFiltrator.h BoostIncludes.h LazyFileGroupComparator.cpp LazyFileGroupComparator.h MainManager.cpp MainManager.h)

include_directories(${CMAKE_SOURCE_DIR} ${BOOST_LATEST_INCLUDE_DIR})
target_link_libraries(bayan PUBLIC Boost::filesystem Boost::thread Boost::system Boost::program_options Boost::regex)
target_link_libraries(test PUBLIC Boost::filesystem Boost::thread Boost::system Boost::program_options Boost::regex)



