#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "main.hpp"

class Location {
private:
    std::string _path;
    std::string _index;
    std::string _methods;

public:
    // STRUCTORS
    Location() {};
    Location(std::string index, std::string methods);
    ~Location() {};

    // METHODS
    void setIndex(std::string index);
    void setMethods(std::string methods);
    void setPath(std::string path);
    void findVarName(std::string line, std::vector<std::string> &variables);
    void setAtrributes(std::vector<std::string> variables);

    // GETTERS
    std::string getIndex();
    std::string getMethods();

    // OUTPUT
    void outputLocation();
};

#endif