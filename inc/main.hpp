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
#include <vector>

/* Header inclusions */
#include "./server/server.hpp"

/* config info */
#define PORT 8080

