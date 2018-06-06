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
        read(client, &length, 4);        

        char id = 0;
        read(client, &id, 1);

        char* buffer = new char[length];
        read(client, buffer, length);
        
        if (id == 1) {
            player.posDirty = true;
            
            buffer += sizeof(int);

            player.x = *((float*) buffer);
            buffer += sizeof(float);
            player.y = *((float*) buffer);
            buffer += sizeof(float);            
            player.z = *((float*) buffer);
            buffer += sizeof(float);

            player.qw = *((float*) buffer);
            buffer += sizeof(float);
            player.qx = *((float*) buffer);
            buffer += sizeof(float);            
            player.qy = *((float*) buffer);
            buffer += sizeof(float);         
            player.qz = *((float*) buffer);
            buffer += sizeof(float);      
        } else if (id == 2){
            std::cout << length << " " << (int)id << ";" << std::endl;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }
}