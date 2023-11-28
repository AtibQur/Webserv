#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "main.hpp"


enum class ConfigKey {
    LISTEN,
    SERVER_NAME,
    INDEX,
    ROOT,
};

class Location;

class Config {
private:
    int _port;
    std::vector<std::string> _server_names;
    std::string _index;
    std::string _root;
    std::map<std::string, Location> _locations;
    std::vector<std::string> _lines;
    unsigned long long max_body_size;
    std::map<std::string, std::string> _error_pages;

public:
    // STRUCTORS
    Config() : max_body_size(1000000), _root("root") {};
    Config(std::vector<std::string> lines);
    ~Config() {};

    // METHODS
    const std::string& ConfigKeyToString(ConfigKey configKey);
    void findVarName(std::string line, int &index);

    // SETTERS
    void setAttribute(std::string variable, std::string value, int &index, int line_i);
    void setLocation(std::string path, int &index);
    void setServerName(std::string server_name, int &index, int line_i);
    void setMaxBodySize(std::string value);
    void setErrorPage(std::string value, int &index, int line_i);

    // OUTPUT
    void outputConfig();
    void outputLines();
    void outputServerNames();

    // GETTERS
    int getPort() { return _port; };
    std::vector<std::string> getServerNames() { return _server_names; };
    std::string getIndex() { return _index; };
    std::string getRoot() { return _root; };
    Location getLocation(std::string path);
    unsigned long long getMaxBodySize() { return max_body_size; };
    std::string getErrorPage(std::string code) { return _error_pages[code]; };
    std::map<std::string, std::string> getErrorPages() { return _error_pages; };
    std::map<std::string, Location> getLocations() { return _locations; };

};

#endif