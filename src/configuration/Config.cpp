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
    _lines = lines;
    std::string var = "";
    for (std::string line : lines) {
        findVarName(line);
    }
    std::cout << "port: " << _port << std::endl;
    std::cout << "server_name: " << _server_name << std::endl;
    std::cout << "index: " << _index << std::endl;
    std::cout << "root: " << _root << std::endl;
    
}

void Config::outputLines() {
    for (std::string line : _lines)
        std::cout << line << std::endl;
}

void Config::findVarName(std::string line) {
    std::string variable = "";
    std::string value = "";
    std::string::iterator it = line.begin();
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (it == line.end() || *it == '#')
        return ;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') {
        variable += *it++;
    }
    if (variable == "}")
        return ;
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (it == line.end() || *it == '#')
        return ;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') {
        value += *it++;
    }
    setAttribute(variable, value);
}

void Config::setAttribute(std::string variable, std::string value) {
    std::string options[4] = {"listen", "server_name", "index", "root"};
    for (int i = 0; i < 4; i++) {
        if (variable == options[i]) {
            switch (i) {
                case 0:
                    _port = std::stoi(value);
                    break;
                case 1:
                    _server_name = value;
                    break;
                case 2:
                    _index = value;
                    break;
                case 3:
                    _root = value;
                    break;
            }
        }
    }
}