#include "playerThread.hpp"
#include <stdio.h>

PlayerThread::PlayerThread(int client, Player& player): client(client), player(player) {

}

void PlayerThread::Run() { 
    loop = new std::thread(&PlayerThread::Loop, this);
    loop->detach();
}

void PlayerThread::Loop() {
    while (true) {
        int length = 0;
        read(client, &length, sizeof(int));

        char* buffer = new char[length];
        read(client, buffer, length);

        short id = buffer[0];
        
        if (id == 1) {
            player.posDirty = true;
            player.Deserialize(buffer, sizeof(int) + 1);   
        } else if (id == 2){
            std::cout << length << " " << (int)id << ";" << std::endl;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
}