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
	}
    if (conf.empty()) {
        ExitWithMessage("Error: no server blocks found in config file");
    }
    return (conf);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "2 arguments please ./webserv [config_file]" << std::endl;
        return 1;
    }

    std::vector<Config> conf = confloop(argv[1]);

    // Server server(&conf[0]);

    // server.Start();

    return 0;
}