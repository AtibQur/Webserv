/* C Libraries */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>

/* CPP Libraries */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <stdexcept>

/* Header inclusions */
#include "Server.hpp"
#include "Config.hpp"
#include "ConfigParser.hpp"
#include "ConfigParserUtils.hpp"
#include "Client.hpp"
#include "Response.hpp"

/* config info */
#define PORT 8080

void ExitWithMessage(std::string message);