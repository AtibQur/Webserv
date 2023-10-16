/* C Libraries */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* CPP Libraries */
#include <iostream>

/* Header inclusions */
#include "Server.hpp"
#include "Config.hpp"
#include "ConfigParser.hpp"
#include "ConfigParserUtils.hpp"

/* config info */
#define PORT 8080

