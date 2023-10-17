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
    std::vector<std::string> _lines;

public:
    // STRUCTORS
    Config() {};
    Config(std::vector<std::string> lines) {};
    ~Config() {};

    // METHODS
    const std::string& ConfigKeyToString(ConfigKey configKey);
};
#endif