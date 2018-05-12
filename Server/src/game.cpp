#include "game.hpp"
#include <stdio.h>

Game::Game(std::vector<int>& clients, std::map<int, Player>& players): clients(clients), players(players) {

}

void Game::Run() { 
    loop = new std::thread(&Game::Loop, this);
}

void Game::Loop() {
    while (true) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));

        // Server tick start.
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
            Player& p = players[*it];

            char id = 0;
            read(*it, &id, 1);
            
            if (id == 1) {
                read(*it, &p.x, 4);
                read(*it, &p.y, 4);
                read(*it, &p.z, 4); 

                std::cout << "Client " << *it << " is at " << p.x << " " << p.y << " " << p.z << "." << std::endl;        
            }  
        }
        // Server tick end.     
    }
}