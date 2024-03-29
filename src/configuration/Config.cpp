#include "Config.hpp"

namespace fs = std::filesystem;

Config::Config(std::vector<std::string> lines) : _port(8080), _client_max_body_size(1000000)
{
    _lines = lines;
    std::string var = "";
    int index = 0;
    int size = lines.size();
    setDefaultErrorPages();
    while (index < size)
    {
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

void Config::findVarName(std::string line, int &index)
{
    std::string variable = "";
    std::string value = "";
    std::string::iterator it = line.begin();
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (it == line.end() || *it == '#') // after every key we check if line is finished
        return;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';')
    { // copy word till first whitespace and repeat for value
        variable += *it++;
    }
    if (variable == "}")
        return;
    while (it != line.end() && (*it == ' ' || *it == '\t'))
        it++;
    if (it == line.end() || *it == '#')
        return;
    while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';')
    {
        value += *it++;
    }
    setAttribute(variable, value, index, std::distance(line.begin(), it));
}

void Config::setAttribute(std::string variable, std::string value, int &index, int line_i)
{
    if (value.empty())
        return;
    std::string options[8] = {// all possible options for server block
                              "listen",
                              "server_name",
                              "index",
                              "root",
                              "location",
                              "client_max_body_size",
                              "error_page",
                              "file_if_dir"};

    _server_names.push_back("");

    for (int i = 0; i < 8; i++)
    {    
        if (variable == options[i])
        { // if variable name matches expected name we store it
            switch (i)
            {
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
                setRoot(value);
                break;
            case 4:
                setLocation(value, index);
                break;
            case 5:
                setMaxBodySize(value);
                break;
            case 6:
                setErrorPage(value, index, line_i);
                break;
            case 7:
                _file_if_dir = value;
                break;
            }
        }
    }
}

/*
    in this function we are looping through the line and storing all server names
*/

void Config::setServerName(std::string server_name, int &index, int line_i)
{
    _server_names.clear();
    std::string name;
    std::string line = _lines[index].substr(line_i); // start from first server name
    std::string::iterator it = line.begin();
    _server_names.push_back(server_name);
    while (it != line.end() && (*it == ' ' || *it == '\t')) // skip whitespace
        it++;
    while (it != line.end() && *it != ';')
    {
        while (it != line.end() && *it != ' ' && *it != '\t' && *it != ';') // copy every character of the name to name
            name += *it++;
        _server_names.push_back(name);
        name = "";
        while (it != line.end() && (*it == ' ' || *it == '\t'))
            it++;
    }
}

void Config::setRoot(std::string value)
{
    if (fs::exists(value) && fs::is_directory(value))
    {
        _root = value;
    }
    else
    {
        _root = "root";
    }
}

void Config::setErrorPage(std::string error_code, int &index, int line_i)
{
    std::string page;
    std::string line = _lines[index].substr(line_i); // start from index where page is
    std::string::iterator it = line.begin();
    while (it != line.end() && (*it == ' ' || *it == '\t')) // skip whitespace
        it++;
    while (it != line.end() && *it != ';' && *it != ' ' && *it != '\t')
        page += *it++;
    _error_pages[error_code] = page;
}

void Config::setMaxBodySize(std::string value)
{
    char multiplier = 'M';
    if (toupper(value[value.size() - 1]) == 'B')
    {
        if (isdigit(value[value.size() - 2]))
        {
            _client_max_body_size = std::stoull(value.substr(0, value.size() - 1));
            return;
        }
        value.pop_back();
    }
    if (isalpha(value[value.size() - 1]))
    {
        multiplier = toupper(value[value.size() - 1]);
        value.pop_back();
    }
    _client_max_body_size = std::stoull(value);
    if (multiplier == 'K')
        _client_max_body_size *= 1000; // mutiply by a thousand to convert it to kilobyte
    else if (multiplier == 'M')
        _client_max_body_size *= 1000000; // mutiply by a million to convert it to megabyte
    else if (multiplier == 'G')
        _client_max_body_size *= 1000000000; // you get it
    else
        _client_max_body_size *= 1000000; // default is megabyte
}

/*
    in this function we are iterating through every line of each location block
    and storing the values in a temporary location object
    when we reach the closing bracket we store the temporary object in the _locations map
*/

void Config::setDefaultErrorPages()
{
    _error_pages["400"] = "docs/error_pages/400.html";
    _error_pages["401"] = "docs/error_pages/401.html";
    _error_pages["403"] = "docs/error_pages/403.html";
    _error_pages["404"] = "docs/error_pages/404.html";
    _error_pages["405"] = "docs/error_pages/405.html";
    _error_pages["413"] = "docs/error_pages/413.html";
    _error_pages["415"] = "docs/error_pages/415.html";
    _error_pages["418"] = "docs/error_pages/418.html";
    _error_pages["500"] = "docs/error_pages/500.html";
    _error_pages["501"] = "docs/error_pages/501.html";
    _error_pages["505"] = "docs/error_pages/505.html";
}

void Config::setLocation(std::string path, int &index)
{
    Location temp;
    std::vector<std::string> variables;
    index++; // skip opening bracket as we already got the value
    temp.setPath(path);
    while (_lines[index].find("}") == std::string::npos)
    {
        temp.findVarName(_lines[index], variables, 0);
        index++;
    }
    temp.setAtrributes(variables);
    _locations[path] = temp;
}

// OUTPUT

void Config::outputConfig()
{
    std::cout << "port: " << _port << std::endl;
    std::cout << "index: " << _index << std::endl;
    std::cout << "root: " << _root << std::endl;
    std::cout << "server_names: " << std::endl;
    std::cout << "default file for dir req: " << _file_if_dir << std::endl;
    outputServerNames();
    std::cout << "locations: " << std::endl;
    for (auto it = _locations.begin(); it != _locations.end(); it++)
    {
        std::cout << "path: " << it->first << std::endl;
        it->second.outputLocation();
    }
    std::cout << "client_max_body_size: " << _client_max_body_size << std::endl;
    std::cout << "error_pages: " << std::endl;
    for (auto it = _error_pages.begin(); it != _error_pages.end(); it++)
    {
        std::cout << "error_code: " << it->first << std::endl;
        std::cout << "error_page: " << it->second << std::endl;
    }
}

void Config::outputServerNames()
{
    for (std::string name : _server_names)
        std::cout << name << std::endl;
}

void Config::outputLines()
{
    for (std::string line : _lines)
        std::cout << line << std::endl;
}

std::string trimTillLastSlash(std::string path)
{
    std::string::iterator it = path.end();
    while (it != path.begin() && *it != '/')
        it--;

    return (path.substr(0, std::distance(path.begin(), it)));
}

Location Config::getLocation(std::string path)
{
    if (path == "/")
        return _locations[path];
    while (1)
    {
        if (_locations.find(path) != _locations.end())
            return (_locations[path]);
        if (path == "/" || path.empty())
            return (_locations["/"]);
        path = trimTillLastSlash(path);
    }
};

std::vector<std::string> Config::getServerNames()
{
    return _server_names;
};