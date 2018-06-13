#include "game.hpp"
#include <stdio.h>

#include "NetworkMessages/despawnMessage.hpp"
#include "NetworkMessages/enableMessage.hpp"
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
    Player& p1 = players[clientSocket] = Player();

    // Sending spawn message to all clients including myself
    SpawnMessage msg;

    msg.prefabId = 0;
    msg.objectId = clientSocket;

    msg.hasAuthority = true;
    msg.Send(clientSocket);

    for (std::vector<int>::iterator it = clients.begin(); it != clients.end(); it++) {
        msg.hasAuthority = false;

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

        Player& p2 = players[*it];                    
        float distVector[3] = {p1.position[0], p1.position[1], p1.position[2]};

        distVector[0] -= p2.position[0];
        distVector[1] -= p2.position[1];
        distVector[2] -= p2.position[2];

        float sqrMagnitude = distVector[0]*distVector[0] + distVector[1]*distVector[1] + distVector[2]*distVector[2];
        float sqrRadius = p1.interestRadius*p1.interestRadius;

        if (sqrMagnitude > sqrRadius) { // Player not in interest radius
            EnableMessage emsg;
            emsg.objectId = *it;
            emsg.toEnable = false;

            emsg.position[0] = p2.position[0];
            emsg.position[1] = p2.position[1];
            emsg.position[2] = p2.position[2];
            
            emsg.rotation[0] = p2.rotation[0];
            emsg.rotation[1] = p2.rotation[1];
            emsg.rotation[2] = p2.rotation[2];
            emsg.rotation[3] = p2.rotation[3];

            p1.playerSeen.erase(*it);

            emsg.Send(clientSocket);

            emsg.objectId = clientSocket;
            emsg.Send(*it);            
        }
    }

    // Player can now be seen by others
    clients.push_back(clientSocket);

    players[clientSocket].pthread = new PlayerThread(clientSocket, players[clientSocket], *this);
    players[clientSocket].pthread->Run();
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
            Player& p1 = players[*it];

            if (p1.posDirty) {
                p1.posDirty = false;

                TransformMessage msg;
                msg.sourceId = *it;
                msg.position[0] = p1.position[0];
                msg.position[1] = p1.position[1];
                msg.position[2] = p1.position[2];

                msg.rotation[0] = p1.rotation[0];
                msg.rotation[1] = p1.rotation[1];
                msg.rotation[2] = p1.rotation[2];
                msg.rotation[3] = p1.rotation[3];

                for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
                    if (*it == *jt)
                        continue;
                    
                    Player& p2 = players[*jt];                    
                    float distVector[3] = {p2.position[0], p2.position[1], p2.position[2]};

                    distVector[0] -= p1.position[0];
                    distVector[1] -= p1.position[1];
                    distVector[2] -= p1.position[2];

                    float sqrMagnitude = distVector[0]*distVector[0] + distVector[1]*distVector[1] + distVector[2]*distVector[2];
                    float sqrRadius = p2.interestRadius*p2.interestRadius;

                    if (sqrMagnitude > sqrRadius) { // Player not in interest radius
                        if (p2.playerSeen.count(*it) != 0) {
                            EnableMessage emsg;
                            emsg.objectId = *it;
                            emsg.toEnable = false;

                            emsg.position[0] = p1.position[0];
                            emsg.position[1] = p1.position[1];
                            emsg.position[2] = p1.position[2];
                            
                            emsg.rotation[0] = p1.rotation[0];
                            emsg.rotation[1] = p1.rotation[1];
                            emsg.rotation[2] = p1.rotation[2];
                            emsg.rotation[3] = p1.rotation[3];

                            p2.playerSeen.erase(*it);

                            emsg.Send(*jt);
                        }
                    } else { // Player in interest radius
                        if (p2.playerSeen.count(*it) == 0) {
                            EnableMessage emsg;
                            emsg.objectId = *it;
                            emsg.toEnable = true;

                            emsg.position[0] = p1.position[0];
                            emsg.position[1] = p1.position[1];
                            emsg.position[2] = p1.position[2];
                            
                            emsg.rotation[0] = p1.rotation[0];
                            emsg.rotation[1] = p1.rotation[1];
                            emsg.rotation[2] = p1.rotation[2];
                            emsg.rotation[3] = p1.rotation[3];

                            p2.playerSeen[*it] = &p1;

                            emsg.Send(*jt);                            
                        } else {
                            msg.Send(*jt);
                        }
                    }
                }  
            }
        }
        // Server tick end.     
    }
}