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
        std::this_thread::sleep_for (std::chrono::milliseconds(1));

        // Server tick start.
        for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
            Player& p = players[*it];

            if (p.posDirty) {
                p.posDirty = false;

                TransformMessage msg;
                msg.sourceId = *it;
                msg.position[0] = p.position[0];
                msg.position[1] = p.position[1];
                msg.position[2] = p.position[2];

                msg.rotation[0] = p.rotation[0];
                msg.rotation[1] = p.rotation[1];
                msg.rotation[2] = p.rotation[2];
                msg.rotation[3] = p.rotation[3];

                for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
                    if (*it == *jt)
                        continue;

                    // offset now has the value of the buffer's size
                    msg.Send(*jt);
                }  
            }
        }
        // Server tick end.     
    }
}