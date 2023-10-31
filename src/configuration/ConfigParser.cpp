#include "main.hpp"
#include <algorithm>

ConfigParser::ConfigParser(std::vector<std::string> lines, int start, int end) {
    for (int i = start; i <= end; i++) {
        _lines.push_back(lines[i]);
    }
}

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

/* 
    in this function we are searching the first next server block and returning
    its index from the full config array
    we also modify the end variable to the index of the correct closing bracket of the server block
    so we can use the values to make subvectors for each server
*/

int ConfigParser::findServerBlock(int start, int &end) {
    int i = start;
    int bracket_count = 0;
    int line_count = 0;
    while (i < _lines.size()) {
        if (_lines[i].find("server ") != std::string::npos) {
            start = i++;
            break;
        }
        i++;
    }
    while (i < _lines.size()) {
        if (_lines[i].find("{") != std::string::npos) 
            bracket_count++;
        if (_lines[i].find("}") != std::string::npos && bracket_count == 0) {
            end = i;
            break;
        }
        line_count = std::count(_lines[i].begin(), _lines[i].end(), '}');
        if (line_count > 0)
            bracket_count -= line_count;
        i++;
    }
    if (i == _lines.size())
        return -1;
    return start;
}
