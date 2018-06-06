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

            if (p.posDirty) {
                short offset = 0;
                char* buffer = new char[1 + 2*sizeof(int) + 7*sizeof(float)];

                (*(int*)(buffer + offset)) = sizeof(int) + 7*sizeof(float);
                offset += sizeof(int);

                buffer[offset++] = 1;

                (*(int*)(buffer + offset)) = *it;
                offset += sizeof(int);

                (*(float*)(buffer + offset)) = p.x;
                offset += sizeof(float);
                (*(float*)(buffer + offset)) = p.y;
                offset += sizeof(float);
                (*(float*)(buffer + offset)) = p.z;
                offset += sizeof(float);

                (*(float*)(buffer + offset)) = p.qw;
                offset += sizeof(float);
                (*(float*)(buffer + offset)) = p.qx;
                offset += sizeof(float);
                (*(float*)(buffer + offset)) = p.qy;
                offset += sizeof(float);
                (*(float*)(buffer + offset)) = p.qz;
                offset += sizeof(float);                

                for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
                    if (*it == *jt)
                        continue;

                    // offset now has the value of the buffer's size
                    send(*jt, buffer, offset, 0);
                }            
            }
        }
        // Server tick end.     
    }
}