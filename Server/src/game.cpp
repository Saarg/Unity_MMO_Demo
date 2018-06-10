#include "game.hpp"
#include <stdio.h>

#include "NetworkMessages/despawnMessage.hpp"
#include "NetworkMessages/spawnMessage.hpp"
#include "NetworkMessages/transformMessage.hpp"

#include "player.hpp"
#include "playerThread.hpp"

Game::Game(std::vector<int>& clients, std::map<int, Player>& players): clients(clients), players(players) {

}

void Game::Run() { 
    loop = new std::thread(&Game::Loop, this);
}

void Game::Spawn(int prefabId, int owner, bool hasAuthority) {
    objects[++lastValidId] = owner;

    SpawnMessage msg;
    msg.prefabId = prefabId;
    msg.objectId = lastValidId;

    for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
        msg.hasAuthority = (owner == *jt && hasAuthority);

        msg.Send(*jt);
    }
}

void Game::SpawnPlayer(int clientSocket) {
    // Init player
    clients.push_back(clientSocket);
    players[clientSocket] = Player();

    players[clientSocket].pthread = new PlayerThread(clientSocket, players[clientSocket], *this);
    players[clientSocket].pthread->Run();

    // Sending spawn message to all clients including myself
    SpawnMessage msg;

    msg.prefabId = 0;
    msg.objectId = clientSocket;

    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
        msg.hasAuthority = (clientSocket == *it);

        msg.Send(*it);
    }

    // Spawn all already spawned players
    SpawnMessage msg2;

    msg2.prefabId = 0;
    msg2.hasAuthority = false;               

    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
        if (clientSocket == *it)
            continue;

        msg2.objectId = *it;

        msg2.Send(clientSocket);
    }
}

void Game::Despawn(int objectId) {
    DespawnMessage msg;
    msg.objectId = objectId;

    for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
        msg.Send(*jt);
    }
}

void Game::DespawnPlayer(int clientSocket) {
    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    
    if (players.count(clientSocket) != 0) {
        players[clientSocket].pthread->running = false;
    }
    players.erase(clientSocket);

    DespawnMessage msg;
    msg.objectId = clientSocket;

    for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
        msg.Send(*jt);
    }

    close(clientSocket);
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

                    msg.Send(*jt);
                }  
            }
        }
        // Server tick end.     
    }
}