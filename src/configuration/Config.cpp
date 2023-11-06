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

Config::Config(std::vector<std::string> lines) : max_body_size(1000000) {
    _lines = lines;
    std::string var = "";
    int index = 0;
    while (index < lines.size()) {
        findVarName(lines[index], index);
        index++;
    }
    // outputConfig();
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
    setAttribute(variable, value, index, std::distance(line.begin(), it));
}

void Config::setAttribute(std::string variable, std::string value, int &index, int line_i) {
    std::string options[6] = { // all possible options for server block
        "listen",
        "server_name",
        "index",
        "root",
        "location",
        "max_body_size"
    };

    for (int i = 0; i < 6; i++) {
        if (variable == options[i]) { // if variable name matches expected name we store it
            switch (i) {
                case 0:
                    _port = std::stoi(value);
                    break;
                case 1:
                    setServerName(value, index, line_i);
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
                case 5:
                    setMaxBodySize(value);
                    break;
            }
        }
    }
}

/*
    in this function we are looping through the line and storing all server names
*/

void Config::setServerName(std::string server_name, int &index, int line_i) {
    std::string name;
    std::string line = _lines[index].substr(line_i); // start from first server name
    std::string::iterator it = line.begin();
    _server_names.push_back(server_name);
    while (it != line.end() && (*it == ' ' || *it == '\t')) // skip whitespace
        it++;
    while (it != line.end() && *it != ';') {
        while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') // copy every character of the name to name
            name += *it++;
        _server_names.push_back(name);
        name = "";
        while (it != line.end() && (*it == ' ' || *it == '\t'))
            it++;
    }
}


void Config::setMaxBodySize(std::string value) {
    max_body_size = std::stoull(value) * 1000000; // mutiply by a million to convert it to megabyte
}

/*
    in this function we are iterating through every line of each location block
    and storing the values in a temporary location object
    when we reach the closing bracket we store the temporary object in the _locations map
*/

void Config::setLocation(std::string path, int &index) {
    Location temp;
    std::vector<std::string> variables;
    index++;    // skip opening bracket as we already got the value
    temp.setPath(path);
    while (_lines[index].find("}") == std::string::npos) {
        temp.findVarName(_lines[index], variables, 0);
        index++;
    }
    temp.setAtrributes(variables);
    _locations[path] = temp;
}

// OUTPUT

void Config::outputConfig() {
    std::cout << "port: " << _port << std::endl;
    std::cout << "index: " << _index << std::endl;
    std::cout << "root: " << _root << std::endl;
    std::cout << "server_names: " << std::endl;
    outputServerNames();
    std::cout << "locations: " << std::endl;
    for (auto it = _locations.begin(); it != _locations.end(); it++) {
        std::cout << "path: " << it->first << std::endl;
        it->second.outputLocation();
    }
    std::cout << "max_body_size: " << max_body_size << std::endl;
    std::cout << "----------------------------\n" << std::endl;
}

void Config::outputServerNames() {
    for (std::string name : _server_names)
        std::cout << name << std::endl;
}

void Config::outputLines() {
    for (std::string line : _lines)
        std::cout << line << std::endl;
}