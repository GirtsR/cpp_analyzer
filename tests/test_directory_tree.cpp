//
// Created by Girts Rudziss on 08/05/2018.
//
#define BOOST_TEST_DYN_LINK                 //Link to Boost
#define BOOST_TEST_MODULE "DirectoryTreeTests" //Module name

#include <boost/test/unit_test.hpp>
#include "../directory_tree.h"
namespace fs = boost::filesystem;

BOOST_AUTO_TEST_CASE(no_such_dir_exception)
{
    BOOST_CHECK_THROW(DirectoryTree tree(fs::path("")), boost::filesystem::filesystem_error);
}

BOOST_AUTO_TEST_CASE(testdir_size)
{
    DirectoryTree tree(fs::path("../tests/testdir"));
    BOOST_CHECK_EQUAL(tree.return_dirsize(), 48426);
}