#include "Location.hpp"
#include <filesystem>

Location::Location(){
    _path = "";
    _index = "";
    _autoindex = false;
}

Location::Location(std::string index, std::vector<std::string> methods, std::string path) {
    _index = index;
    _methods = methods;
    _path = path;
    _autoindex = false;
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


/*
    this function is similar to the one in Config.cpp but in this one we put in a vector
    at the end of the function we check if the line is finished or if there is another variable name
    if so we recursively call the function with the index of the next variable name
*/

void Location::findVarName(std::string line, std::vector<std::string> &variables, int index) {
    std::string variable;
    std::string value;
    if (index != 0)
        line = line.substr(index);
    std::string::iterator it = line.begin();
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (*it == '#')
        return ;
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
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    variables.push_back(variable);
    variables.push_back(value);
    if (it != line.end() && *it != ';')
        findVarName(line, variables, it - line.begin());
}

/*
    in this function we are iterating through the vector of variables and setting the attributes
*/

void Location::setAtrributes(std::vector<std::string> variables) {
    int i = 0;
    bool methodSet = false;
    while (i < variables.size()) {
        if (variables[i] == "index") // if the name matches we set the value of the next index
            _index = variables[++i];
        else if (variables[i] == "methods") {
            setMethods(variables, i);
            methodSet = true;
        }
        else if (variables[i] == "autoindex")
            setAutoIndex(variables[++i]);
        else if (variables[i] == "cgi")
        {
            setCgi(variables, i);
        }
        i++;
    }
    if (!methodSet) {
        _methods.push_back("GET");
        _methods.push_back("POST");
        _methods.push_back("DELETE");
    }
}

void Location::setCgi(std::vector<std::string> variables, int &index) {
    if (!variables[index + 1].empty()) {
        _cgi.push_back(variables[index + 1]);
        index++;
    }
    if (!variables[index + 1].empty()) {
        _cgi.push_back(variables[index + 1]);
        index++;
    }
}

void Location::setAutoIndex(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), ::toupper);
    if (value == "ON")
        _autoindex = true;
    else if (value == "OFF")
        _autoindex = false;
    else
        _autoindex = false;
}

/*
    while the variables vector matches valid HTTP methods we continue to add them to the _methods vector
*/

void Location::setMethods(std::vector<std::string> variables, int &index) {
    std::string methods = trim(variables[++index]);
    std::transform(methods.begin(), methods.end(), methods.begin(), ::toupper);
    while (methods == "GET" || methods == "POST" || methods == "DELETE") {
        _methods.push_back(methods);
        if (index + 1 == variables.size())
            break;
        methods = variables[++index];
        std::transform(methods.begin(), methods.end(), methods.begin(), ::toupper);
    }
    index--;
}

void Location::ListDirectory() {
    
}

// OUTPUT

void Location::outputLocation() {
    ListDirectory();
    if (!_index.empty())
        std::cout << "index: " << _index << std::endl;
    if (!_methods.empty()) {
        std::cout << "methods: " << std::endl;
        for (auto it = _methods.begin(); it != _methods.end(); it++)
            std::cout << *it << std::endl;
    }
    std::cout << "autoindex: " << _autoindex << std::endl;
    if (!_cgi.empty()) {
        std::cout << "cgi: " << std::endl;
        for (auto it = _cgi.begin(); it != _cgi.end(); it++)
            std::cout << *it << std::endl;
    }
    if (!_path.empty())
        std::cout << "path: " << _path << std::endl;

}