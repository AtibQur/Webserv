#include "main.hpp"

void ExitWithMessage(std::string message) {
    std::cerr << message << std::endl;
    std::quick_exit(EXIT_FAILURE);
}

std::vector<Config> confloop(std::string file_name) {
    ConfigParser        confParser(file_name);
    confParser.readFile();

    std::vector<Config>  conf;
    int end = 0;
	int start = confParser.findServerBlock(0, end);
	while(start > -1) {
        // making subarray for each server block and constructing config object
        ConfigParser parse_temp(confParser.getLines(), start, end);
        Config  conf_temp(parse_temp.getLines());
        conf.push_back(conf_temp);
        // continue searching for server blocks from old end index till none are found
		start = confParser.findServerBlock(end, end);
        // conf_temp.outputConfig();
	}
    if (conf.empty()) {
        ExitWithMessage("Error: no server blocks found in config file");
    }
    return (conf);
}

const char* file = "config-files/config.conf";
int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     std::cout << "2 arguments please ./webserv [config_file]" << std::endl;
    //     return 1;
    // }

    std::cout << "\033[1;32mserver is running...\033[0m" << std::endl;
    // std::vector<Config> conf = confloop(argv[1]);
    std::vector<Config> conf = confloop(file);
    BigServer bigServer(conf);

    return 0;
}