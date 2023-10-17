#include "ConfigParser.hpp"

void ConfigParser::readFile() {
    _conf_file.open(_conf_file_path, std::ios::in); // open file readonly
    
    // check if file is opened correctly, if so store lines in vector
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

int ConfigParser::findServerBlock(int start, int &end) {
    int i = start;
    while (i < _lines.size()) {
        if (_lines[i].find("server ") != std::string::npos) {
            start = i;
            break;
        }
        i++;
    }
    while (i < _lines.size()) {
        if (_lines[i].find("}") != std::string::npos) {
            end = i;
            break;
        }
        i++;
    }
    if (i == _lines.size())
        return -1;
    return start;
}