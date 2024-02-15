/* C Libraries */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>

/* CPP Libraries */
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <filesystem>

/* Header inclusions */
#include "Server.hpp"
#include "Config.hpp"
#include "ConfigParser.hpp"
#include "ConfigParserUtils.hpp"
#include "Client.hpp"
#include "Response.hpp"
#include "Location.hpp"
#include "BigServer.hpp"
#include "Socket.hpp"
#include "CgiOut.hpp"

void ExitWithMessage(std::string message);