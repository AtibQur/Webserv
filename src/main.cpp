#include "../inc/main.hpp"

int main(int argc, char *argv[]) {
    if (argc != 1) {
        return 1;
    }

    Server server;

    server.start();

    return 0;
}