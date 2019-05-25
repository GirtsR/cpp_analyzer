//
// Created by Girts Rudziss on 08/05/2018.
//
#define BOOST_TEST_DYN_LINK                 //Link to Boost
#define BOOST_TEST_MODULE "DirectoryTreeTests" //Module name

#include <boost/test/unit_test.hpp>
#include "../directory_tree.h"
#include <chrono>
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(no_such_dir_exception) {
    BOOST_CHECK_THROW(DirectoryTree tree(fs::path("")), boost::filesystem::filesystem_error);
}

BOOST_AUTO_TEST_CASE(ccc_dir) {
    DirectoryTree tree(fs::path("../tests/testdir/aaa/ccc"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 18);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 0);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 6);
}

BOOST_AUTO_TEST_CASE(bbb_dir) {
    DirectoryTree tree(fs::path("../tests/testdir/aaa/bbb"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 12);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 1);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 0);
}

BOOST_AUTO_TEST_CASE(aaa_dir) {
    DirectoryTree tree(fs::path("../tests/testdir/aaa"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 384);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 8);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 13);
}

BOOST_AUTO_TEST_CASE(empty_dir) {
    DirectoryTree tree(fs::path("../tests/testdir/emptyfolder"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 0);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 0);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 0);
}

BOOST_AUTO_TEST_CASE(testdir) {
    DirectoryTree tree(fs::path("../tests/testdir"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 36130);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 708);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 717);
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
