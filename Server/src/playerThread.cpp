#include "playerThread.hpp"
#include <stdio.h>

#include "NetworkMessages/spawnMessage.hpp"
#include "NetworkMessages/despawnMessage.hpp"

PlayerThread::PlayerThread(int client, Player& player, Game& game): client(client), player(player), game(game) {

}

PlayerThread::~PlayerThread() {
    
}

void PlayerThread::Run() { 
    running = true;    
    
    loop = new std::thread(&PlayerThread::Loop, this);
    loop->detach();
}

void PlayerThread::Loop() {
    while (running) {
        int length = 0;
        read(client, &length, sizeof(int));

        char* buffer = new char[length];
        read(client, buffer, length);

        short id = buffer[0];
        
        if (id == 1) {
            player.Deserialize(buffer, 1 + sizeof(int));
            player.posDirty = true;            
        } else if (id == 2) {
            SpawnMessage msg;
            msg.Deserialize(buffer);

            if (msg.objectId == client) {
                std::cout << "client: " << client << " requested spawn of prefab:" << msg.prefabId << std::endl;
                game.Spawn(msg.prefabId, msg.objectId);
            }
        } else if (id == 3) {
            DespawnMessage msg;
            msg.Deserialize(buffer);

            if (msg.objectId == client) {
                std::cout << "client: " << client << " gracefully disconected" << std::endl;
                game.DespawnPlayer(client);
            } else if (msg.objectId >= 10000) {
                std::cout << "client: " << client << " requested despawn of object:" << msg.objectId << std::endl;
                game.Despawn(msg.objectId);    
            } else {
                std::cout << "client: " << client << " requested despawn of object:" << msg.objectId << " whitout the right to do so!" << std::endl;                
            }
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }

    std::cout << "Client " << client << "'s thread dying" << std::endl;
    delete this;
}