#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "main.hpp"

class ConfigParser : public ConfigParserUtils {
private:
    std::string _conf_file;
    int _server_bracket = 0;
    int _location_bracket = 0;

public:
    // STRUCTORS
    ConfigParser() { _conf_file = ""; }
    ConfigParser(std::string file_path) { _conf_file = file_path; }
    ~ConfigParser() {};

    void read_file();
};
#endif