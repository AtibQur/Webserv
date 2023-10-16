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

public:
    // STRUCTORS
    Config() {};
    ~Config() {};

    // METHODS
    const std::string& ConfigKeyToString(ConfigKey configKey);
};
#endif