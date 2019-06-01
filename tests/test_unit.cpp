//
// Created by Girts Rudziss on 08/05/2018.
//
#define BOOST_TEST_DYN_LINK                 //Link to Boost
#define BOOST_TEST_MODULE "FileStatsTests" //Module name

#include <boost/test/unit_test.hpp>
#include "../file_stats.h"
#include <chrono>
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(empty_file) {
    FileStats file(fs::path("../tests/testdir/empty.cpp"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "empty.cpp");
    BOOST_CHECK_EQUAL(file.return_size(), 0);
    BOOST_CHECK_EQUAL(file.return_sloc(), 0);
    BOOST_CHECK_EQUAL(file.return_cloc(), 0);
}

BOOST_AUTO_TEST_CASE(mixed_file) {
    FileStats file(fs::path("../tests/testdir/aaa/mixed_file.cpp"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "mixed_file.cpp");
    BOOST_CHECK_EQUAL(file.return_size(), 354);
    BOOST_CHECK_EQUAL(file.return_sloc(), 7);
    BOOST_CHECK_EQUAL(file.return_cloc(), 7);
}

BOOST_AUTO_TEST_CASE(source_only) {
    FileStats file(fs::path("../tests/testdir/aaa/bbb/source_only.cpp"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "source_only.cpp");
    BOOST_CHECK_EQUAL(file.return_size(), 12);
    BOOST_CHECK_EQUAL(file.return_sloc(), 1);
    BOOST_CHECK_EQUAL(file.return_cloc(), 0);
}

BOOST_AUTO_TEST_CASE(comment_only) {
    FileStats file(fs::path("../tests/testdir/aaa/ccc/comment_only.cpp"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "comment_only.cpp");
    BOOST_CHECK_EQUAL(file.return_size(), 18);
    BOOST_CHECK_EQUAL(file.return_sloc(), 0);
    BOOST_CHECK_EQUAL(file.return_cloc(), 6);
}

BOOST_AUTO_TEST_CASE(large_file) {
    FileStats file(fs::path("../tests/testdir/large_file.cpp"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "large_file.cpp");
    BOOST_CHECK_EQUAL(file.return_size(), 35697);
    BOOST_CHECK_EQUAL(file.return_sloc(), 700);
    BOOST_CHECK_EQUAL(file.return_cloc(), 701);
}

BOOST_AUTO_TEST_CASE(filename_special_chars) {
    FileStats file(fs::path("../tests/testdir/test-#!@£$%^&*()_+\":;'.h"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "test-#!@£$%^&*()_+\":;'.h");
    BOOST_CHECK_EQUAL(file.return_size(), 49);
    BOOST_CHECK_EQUAL(file.return_sloc(), 0);
    BOOST_CHECK_EQUAL(file.return_cloc(), 3);
}

BOOST_AUTO_TEST_CASE(could_not_open_exception) {
    BOOST_CHECK_THROW(FileStats file(fs::path("")), std::runtime_error);        //Checks if runtime error is thrown in the constructor
}

BOOST_AUTO_TEST_CASE(one_line) {
    FileStats file(fs::path("../tests/one_line.h"));
    file.get_size();
    file.check_file();

    BOOST_CHECK_EQUAL(file.return_filename(), "one_line.h");
    BOOST_CHECK_EQUAL(file.return_size(), 506);
    BOOST_CHECK_EQUAL(file.return_sloc(), 1);
    BOOST_CHECK_EQUAL(file.return_cloc(), 1);
}
