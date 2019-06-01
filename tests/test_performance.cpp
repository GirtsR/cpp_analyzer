//
// Created by Girts Rudziss on 2019-06-01.
//
#define BOOST_TEST_DYN_LINK                 //Link to Boost
#define BOOST_TEST_MODULE "FileStatsTests" //Module name

#include <boost/test/unit_test.hpp>
#include "../file_stats.h"
#include "../directory_tree.h"
#include <chrono>
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(performance_100_files) {
    auto start = std::chrono::system_clock::now();
    for (int i = 0; i < 100; i++) {
      FileStats file(fs::path("../tests/100_lines.cpp"));
      file.get_size();
      file.check_file();

      BOOST_CHECK_EQUAL(file.return_filename(), "100_lines.cpp");
      BOOST_CHECK_EQUAL(file.return_size(), 2260);
      BOOST_CHECK_EQUAL(file.return_sloc(), 43);
      BOOST_CHECK_EQUAL(file.return_cloc(), 44);
    }
    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Duration: " << duration << " milliseconds" << std::endl;
    BOOST_CHECK_LT(duration, 500);
}

BOOST_AUTO_TEST_CASE(performance_1000_lines) {
    auto start = std::chrono::system_clock::now();

    FileStats file(fs::path("../tests/1000_lines.cpp"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "1000_lines.cpp");
    BOOST_CHECK_EQUAL(file.return_size(), 22212);
    BOOST_CHECK_EQUAL(file.return_sloc(), 435);
    BOOST_CHECK_EQUAL(file.return_cloc(), 439);

    auto end = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Duration: " << duration << " milliseconds" << std::endl;
    BOOST_CHECK_LT(duration, 200);
}

BOOST_AUTO_TEST_CASE(performance_50_subdirs) {
  auto start = std::chrono::system_clock::now();
  for (int i = 0; i < 50; i++) {
    DirectoryTree tree(fs::path("../tests/testdir/aaa/ccc"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 18);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 0);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 6);
  }
  auto end = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
  std::cout << "Duration: " << duration << " milliseconds" << std::endl;
  BOOST_CHECK_LT(duration, 300);
}
