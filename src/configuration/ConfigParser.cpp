#include "ConfigParser.hpp"

void ConfigParser::read_file() {
    _conf_file.open(_conf_file_path, std::ios::in); // open file readonly
    
    // check if file is opened correctly, store lines in vector
    if (_conf_file.is_open()) {
        std::string line;
        while (getline(_conf_file, line))
            _lines.push_back(line);
    } else {
        std::cerr << "Error: could not open file" << std::endl;
        std::quick_exit(EXIT_FAILURE);
    }
    _conf_file.close();
}