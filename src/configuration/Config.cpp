#include "Config.hpp"

/* TO BE IMPLEMENTED 
    Config::Config() {
        this->config_map[Config::ConfigKeyToString(ConfigKey::SERVER_NAME)] = parsed_map[Config::ConfigKeyToString(ConfigKey::SERVER_NAME)];
        this->config_map[Config::ConfigKeyToString(ConfigKey::INDEX)] = parsed_map[Config::ConfigKeyToString(ConfigKey::INDEX)];
        this->config_map[Config::ConfigKeyToString(ConfigKey::ROOT)] = parsed_map[Config::ConfigKeyToString(ConfigKey::ROOT)];
        this->config_map[Config::ConfigKeyToString(ConfigKey::LISTEN)] = parsed_map[Config::ConfigKeyToString(ConfigKey::LISTEN)];
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

/*
    in this function we are looping through the parsed server block and
    retrieving the values for each key and storing them in the class object
*/

Config::Config(std::vector<std::string> lines) {
    _lines = lines;
    std::string var = "";
    int index = 0;
    while (index < lines.size()) {
        // std::cout << "line: " << line << std::endl;
        findVarName(lines[index], index);
        index++;
    }
}

/*
    for each line which isn't empty or a comment we are looping through the line
    trying it to find its name and value
    if succeeded we are calling the setAttribute function to store the value in the class object
*/

void Config::findVarName(std::string line, int &index) {
    std::string variable = "";
    std::string value = "";
    std::string::iterator it = line.begin();
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (it == line.end() || *it == '#') // after every key we check if line is finished
        return ;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') { // copy word till first whitespace and repeat for value
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
    std::cout << std::distance(line.begin(), it) << std::endl;
    setAttribute(variable, value, index);
}

void Config::setAttribute(std::string variable, std::string value, int &index) {
    std::string options[5] = { // all possible options for server block
        "listen",
        "server_name",
        "index",
        "root",
        "location"
    };

    for (int i = 0; i < 5; i++) {
        if (variable == options[i]) { // if variable name matches expected name we store it
            switch (i) {
                case 0:
                    _port = std::stoi(value);
                    break;
                case 1:
                    setServerName(value, index);
                    break;
                case 2:
                    _index = value;
                    break;
                case 3:
                    _root = value;
                    break;
                case 4:
                    setLocation(value, index);
                    break;
            }
        }
    }
}

void Config::setServerName(std::string server_name, int &index) {
    std::string name;
    std::string::iterator it = server_name.begin();
    _server_names.push_back(name);
    while (it != server_name.end() && *it != ';') {
        while (it != server_name.end() && *it != ' ' && *it != '\t' && *it != ';')
            name += *it++;
        _server_names.push_back(name);
        name = "";
    }
}

void Config::setLocation(std::string path, int &index) {
    Location temp;
    std::vector<std::string> variables;
    index++;    // skip opening bracket as we already got the value
    temp.setPath(path);
    while (_lines[index].find("}") == std::string::npos) {
        temp.findVarName(_lines[index], variables);
        index++;
    }
    temp.setAtrributes(variables);
    _locations[path] = temp;
}

// OUTPUT

void Config::outputLines() {
    for (std::string line : _lines)
        std::cout << line << std::endl;
}