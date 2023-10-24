#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "main.hpp"

enum class ConfigKey {
    LISTEN,
    SERVER_NAME,
    INDEX,
    ROOT,
};

class Config {
private:
    int _port;
    std::string _server_name;
    std::string _index;
    std::string _root;
    std::vector<std::string> _lines;

public:
    // STRUCTORS
    Config() {};
    Config(std::vector<std::string> lines);
    ~Config() {};

    // METHODS
    const std::string& ConfigKeyToString(ConfigKey configKey);
    void outputLines();
    void findVarName(std::string line);

    // SETTERS
    void setAttribute(std::string variable, std::string value);
    void setPort();

};
#endif