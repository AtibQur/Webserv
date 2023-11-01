#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "main.hpp"

class Location {
private:
    std::string _path;
    std::string _index;
    std::vector<std::string> _methods;

public:
    // STRUCTORS
    Location() {};
    Location(std::string index, std::vector<std::string> methods, std::string path);
    ~Location() {};

    // METHODS
    void findVarName(std::string line, std::vector<std::string> &variables);
    void setAtrributes(std::vector<std::string> variables);

    // SETTERS
    void setIndex(std::string index);
    void setMethods(std::vector<std::string> methods, int &index);
    void setPath(std::string path);

    // GETTERS
    std::string getIndex();
    std::vector<std::string> getMethods();
    std::string getPath();

    // OUTPUT
    void outputLocation();
};

#endif