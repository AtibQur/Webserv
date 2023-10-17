#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "main.hpp"

class ConfigParser : public ConfigParserUtils {
private:
    std::ifstream _conf_file;
    std::string _conf_file_path;
    std::vector<std::string> _lines;

public:
    // STRUCTORS
    ConfigParser() { _conf_file_path = ""; }
    ConfigParser(std::string file_path) { _conf_file_path = file_path; }
    ~ConfigParser() {};

    void readFile();
    int findServerBlock(int start, int &end);
};
#endif