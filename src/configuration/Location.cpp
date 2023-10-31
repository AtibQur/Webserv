#include "Location.hpp"

Location::Location(std::string index, std::string methods) {
    _index = index;
    _methods = methods;
}

// SETTERS

void Location::setIndex(std::string index) {
    _index = index;
}

void Location::setMethods(std::string methods) {
    _methods = methods;
}

void Location::setPath(std::string path) {
    _path = path;
}

// GETTERS

std::string Location::getIndex() {
    return (_index);
}

std::string Location::getMethods() {
    return (_methods);
}


void Location::findVarName(std::string line, std::vector<std::string> &variables) {
    std::string variable;
    std::string value;
    std::string::iterator it = line.begin();

    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') {
        variable += *it++;
    }
    if (variable == "}")
        return ;
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (it == line.end() || *it == '#')
        return ;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') {
        value += *it++;
    }
    variables.push_back(variable);
    variables.push_back(value);
}

void Location::setAtrributes(std::vector<std::string> variables) {
    int i = 0;
    while (i < variables.size()) {
        if (variables[i] == "index")
            _index = variables[++i];
        else if (variables[i] == "methods")
            _methods = variables[++i];
        i++;
    }
}

// OUTPUT

void Location::outputLocation() {
    if (!_index.empty())
        std::cout << "index: " << _index << std::endl;
    if (!_methods.empty())
        std::cout << "methods: " << _methods << std::endl;
    if (!_path.empty())
        std::cout << "path: " << _path << std::endl;
}