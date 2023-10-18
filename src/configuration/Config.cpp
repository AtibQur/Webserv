#include "Config.hpp"
/* TO BE IMPLEMENTED 
    Config::Config() {
        this->config_map[Config::ConfigKeyToString(ConfigKey::LISTEN)] = parsed_map[Config::ConfigKeyToString(ConfigKey::LISTEN)];
        this->config_map[Config::ConfigKeyToString(ConfigKey::SERVER_NAME)] = parsed_map[Config::ConfigKeyToString(ConfigKey::SERVER_NAME)];
        this->config_map[Config::ConfigKeyToString(ConfigKey::INDEX)] = parsed_map[Config::ConfigKeyToString(ConfigKey::INDEX)];
        this->config_map[Config::ConfigKeyToString(ConfigKey::ROOT)] = parsed_map[Config::ConfigKeyToString(ConfigKey::ROOT)];
    }


    const std::string& Config::ConfigKeyToString(ConfigKey configKey) {
        static std::unordered_map<ConfigKey, std::string> stringMap{
            {ConfigKey::LISTEN, "listen"},
            {ConfigKey::SERVER_NAME, "server_name"},
            {ConfigKey::INDEX, "index"},
            {ConfigKey::ROOT, "root"}
        };

        return stringMap[configKey];
    }
*/

Config::Config(std::vector<std::string> lines) {
    for(std::string line : lines)
        _lines.push_back(line);
}

void Config::outputLines() {
    for (std::string line : _lines)
        std::cout << line << std::endl;
}