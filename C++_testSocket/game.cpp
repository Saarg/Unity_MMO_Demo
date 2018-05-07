#include "game.hpp"

Game::Game(std::vector<int>& clients, std::map<int, Player>& players): clients(clients), players(players) {

}

void Game::Run() { 
    loop = new std::thread(&Game::Loop, this);
}

void Game::Loop() {
    while (true) {
        std::this_thread::sleep_for (std::chrono::seconds(1));

        std::cout << "Server tick start." << std::endl;
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
            Player& p = players[*it];
            std::cout << "Client " << *it << " is at " << p.x << " " << p.y << " " << p.z << "." << std::endl;
        }
        std::cout << "Server tick end." << std::endl;        
    }
}