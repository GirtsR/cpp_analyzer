//
// Created by Girts Rudziss on 14/05/2018.
//
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fstream>
namespace pt=boost::property_tree;

#ifndef ANALYZER_HTML_GENERATOR_H
#define ANALYZER_HTML_GENERATOR_H

void generate_report(pt::ptree &root);

#endif //ANALYZER_HTML_GENERATOR_H
