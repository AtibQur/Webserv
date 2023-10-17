#ifndef CONFIGPARSERUTILS_HPP
#define CONFIGPARSERUTILS_HPP

#include "main.hpp"

class ConfigParserUtils {
public:
    // STRUCTORS
    ConfigParserUtils() {};
    ~ConfigParserUtils() {};

    // METHODS
    std::string trim(const std::string& str);
};
#endif