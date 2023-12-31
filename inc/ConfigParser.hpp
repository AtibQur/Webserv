#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "main.hpp"

class ConfigParser {
private:
    std::ifstream _conf_file;
    std::string _conf_file_path;
    std::vector<std::string> _lines;

public:
    // STRUCTORS
    ConfigParser() { _conf_file_path = ""; }
    ConfigParser(std::string file_path) { _conf_file_path = file_path; }
    ConfigParser(std::vector<std::string> lines, int start, int end); 
    ~ConfigParser() {};

    void readFile();
    int findServerBlock(int start, int &end);
    std::vector<std::string> getLines() { return _lines; }
};
#endif