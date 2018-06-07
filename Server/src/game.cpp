#include "game.hpp"
#include <stdio.h>

#include "NetworkMessages/transformMessage.hpp"

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

            if (p.posDirty) {
                TransformMessage msg;       

                for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
                    if (*it == *jt)
                        continue;

                    // offset now has the value of the buffer's size
                    send(*jt, msg.Serialize(p), msg.Size(), 0);
                }            
            }
        }
        // Server tick end.     
    }
}