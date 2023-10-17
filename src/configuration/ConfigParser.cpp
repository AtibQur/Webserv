#include "ConfigParser.hpp"
#include <algorithm>

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
    int bracket_count = 0;
    int line_count = 0;
    while (i < _lines.size()) {
        if (_lines[i].find("server ") != std::string::npos) {
            std::cout << "found one!"<< std::endl;
            start = i++;
            break;
        }
        i++;
    }
    while (i < _lines.size()) {
        if (_lines[i].find("{") != std::string::npos) 
            bracket_count++;
        if (_lines[i].find("}") != std::string::npos && bracket_count == 0) {
            std::cout << "Closed one!!"<< std::endl;
            end = i;
            break;
        }
        line_count = std::count(_lines[i].begin(), _lines[i].end(), '{');
        if (line_count > 0)
            bracket_count -= line_count;
        i++;
    }
    if (i == _lines.size())
        return -1;
    return start;
}