#include <iostream>
#include <thread>
#include <vector>
#include <map>
#include <chrono>

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#include "login.hpp"
#include "game.hpp"
#include "player.hpp"

std::vector<int> clients;
std::map<int, Player> players;

int main(int argc, char const *argv[])
{
    Game game(clients, players);
    Login login(clients, players);

    std::cout << '\n' << "Press [ENTER] to quit" << std::endl;
    
    login.Run();
    game.Run();

    while (std::cin.get() != '\n') {
        std::cout << '\n' << "Press [ENTER] to quit" << std::endl;
    }

    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
        close(*it);
    }
}