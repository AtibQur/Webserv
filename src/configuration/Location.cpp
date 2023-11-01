#include "Location.hpp"

Location::Location(std::string index, std::vector<std::string> methods, std::string path) {
    _index = index;
    _methods = methods;
    _path = path;
}

// SETTERS

void Location::setIndex(std::string index) {
    _index = index;
}


void Location::setPath(std::string path) {
    _path = path;
}

// GETTERS

std::string Location::getIndex() {
    return (_index);
}

std::vector<std::string> Location::getMethods() {
    return (_methods);
}

std::string Location::getPath() {
    return (_path);
}

// UTILS

std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// METHODS

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
            setMethods(variables, i);
        i++;
    }
}

void Location::setMethods(std::vector<std::string> variables, int &index) {
    std::string methods = trim(variables[++index]);
    while (methods == "GET" || methods == "POST" || methods == "DELETE") {
        std::cout << "methods: " << methods << std::endl;
        _methods.push_back(methods);
        methods = variables[++index];
    }
    std::cout << "methods: " << methods << std::endl;

}

// OUTPUT

void Location::outputLocation() {
    if (!_index.empty())
        std::cout << "index: " << _index << std::endl;
    if (!_methods.empty()) {
        std::cout << "methods: " << std::endl;
        for (std::string method : _methods)
            std::cout << method << std::endl;
    }
    if (!_path.empty())
        std::cout << "path: " << _path << std::endl;
}