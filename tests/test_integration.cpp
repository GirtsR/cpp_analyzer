//
// Created by Girts Rudziss on 08/05/2018.
//
#define BOOST_TEST_DYN_LINK                 //Link to Boost
#define BOOST_TEST_MODULE "Integration Tests" //Module name

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
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 385);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 8);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 13);
}

BOOST_AUTO_TEST_CASE(empty_dir) {
    DirectoryTree tree(fs::path("../tests/testdir/emptyfolder"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 100);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 2);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 3);
}

BOOST_AUTO_TEST_CASE(testdir) {
    DirectoryTree tree(fs::path("../tests/testdir"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 36130);
    BOOST_CHECK_EQUAL(tree.return_total_sloc(), 708);
    BOOST_CHECK_EQUAL(tree.return_total_cloc(), 717);
}
