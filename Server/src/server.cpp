#include "server.hpp"

#include <unistd.h>

Server::Server(int id, int socket): id(id), socket(socket), players() {
    if (id == 9501) {
        x = -20;
        y = 0;

        size_x = 39;
        size_y = 40;
        size_z = 39;
    } else if (id == 9502) {
        x = 20;
        y = 0;

        size_x = 39;
        size_y = 40;
        size_z = 39;
    }
}

Server::~Server() {
    if (socket != 0) {
        close(socket);
    }
}