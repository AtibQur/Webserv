#include "main.hpp"

void ExitWithMessage(std::string message) {
    std::cerr << message << std::endl;
    std::quick_exit(EXIT_FAILURE);
}

void confloop(std::string file_name) {
    ConfigParser        confParser(file_name);
    confParser.readFile();
    // std::vector<Config>  conf;
    int end = 0;
	int start = confParser.findServerBlock(0, end);
	while(start > -1)
    {
        std::cout << "start: " << start << std::endl;
        std::cout << "end: " << end << std::endl;
        // ConfigParser CPTmp;
        // CPTmp.setServerContent(confParser.subVector(start, end));
        // Config  tmp(CPTmp);

        // conf.push_back(tmp);
		start = confParser.findServerBlock(end, end);
	}
    // if (conf.empty()) {
    //     ExitWithMessage("Error: no server blocks found in config file");
    // }
    // return (conf);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }
    
    confloop(argv[1]);

    // Server server;

    // server.start();

    return 0;
}