#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "main.hpp"

class Location {
private:
    std::string _path;
    std::string _index;
    std::vector<std::string> _methods;
    std::vector<std::string> _cgi;
    bool _autoindex;

public:
    // STRUCTORS
    Location();
    Location(std::string index, std::vector<std::string> methods, std::string path);
    ~Location() {};

    // METHODS
    void findVarName(std::string line, std::vector<std::string> &variables, int index);
    void setAtrributes(std::vector<std::string> variables);

    // SETTERS
    void setIndex(std::string index);
    void setMethods(std::vector<std::string> methods, int &index);
    void setPath(std::string path);
    void setCgi(std::vector<std::string> cgi, int &index);
    void setAutoIndex(std::string value);

    // GETTERS
    std::string getIndex();
    std::vector<std::string> getMethods();
    std::string getPath();
    bool getAutoIndex() { return _autoindex; };
    std::vector<std::string> getCgi() { return _cgi; };

    // OUTPUT
    void outputLocation();
    void ListDirectory();
};

#endif