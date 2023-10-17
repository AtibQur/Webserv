#include "../inc/main.hpp"

int main(int argc, char *argv[]) {
    if (argc != 1) {
        return 1;
    }

    Server server;

    server.Start();

    return 0;
}