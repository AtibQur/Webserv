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

public:
    // STRUCTORS
    Config() {};
    Config(std::vector<std::string> lines);
    ~Config() {};

    // METHODS
    const std::string& ConfigKeyToString(ConfigKey configKey);
    void findVarName(std::string line, int &index);

    // SETTERS
    void setAttribute(std::string variable, std::string value, int &index, int line_i);
    void setPort();
    void setLocation(std::string path, int &index);
    void setServerName(std::string server_name, int &index, int line_i);

    // OUTPUT
    void outputConfig();
    void outputLines();
    void outputServerNames();

};
#endif