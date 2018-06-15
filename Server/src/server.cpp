#include "server.hpp"

#include <unistd.h>

#include "game.hpp"
#include "NetworkMessages/transformMessage.hpp"
#include "NetworkMessages/spawnMessage.hpp"
#include "NetworkMessages/despawnMessage.hpp"
#include "NetworkMessages/enableMessage.hpp"

Server::Server(int id, int socket, Game* game): id(id), socket(socket), game(game), players() {
    if (id == 9501) {
        x = -20;
        z = 0;

        size_x = 39;
        size_y = 40;
        size_z = 39;
    } else if (id == 9502) {
        x = 20;
        z = 0;

        size_x = 39;
        size_y = 40;
        size_z = 39;
    }

    if (socket > 0) {
        msgReceptionThread = new std::thread(&Server::MsgReception, this);
    }

    players = new std::map<int, Player>();
}

Server::~Server() {
    if (socket > 0) {
        close(socket);
        delete players;

        socket = 0;
    }
}

void Server::Update() {
    for (std::map<int, Player>::iterator it=players->begin(); it!=players->end(); it++) {
        int id = it->first;
        Player& p = it->second;

        if (p.posDirty) {
            p.posDirty = false;

            TransformMessage msg;
            msg.sourceId = id;
            msg.position[0] = p.position[0];
            msg.position[1] = p.position[1];
            msg.position[2] = p.position[2];

            msg.rotation[0] = p.rotation[0];
            msg.rotation[1] = p.rotation[1];
            msg.rotation[2] = p.rotation[2];
            msg.rotation[3] = p.rotation[3];

            game->SendMsgToAllInterested(&msg, &p);
        }
    }
}

void Server::MsgReception() {
    while (socket > 0) {
        int length = 0;
        read(socket, &length, sizeof(int));

        if (length <= 0)
            continue;

        char* buffer = new char[length];
        read(socket, buffer, length);

        short id = buffer[0];

        if (id == 1) {
            TransformMessage msg;
            msg.Deserialize(buffer);
            msg.sourceId = -msg.sourceId;

            if (players->count(msg.sourceId) == 0)  {
                std::cout << "\033[1;33m Player " << -msg.sourceId << " from server " << this->id << " entered the border \033[1;37m" << std::endl;

                AddPlayer(msg.sourceId);
            }

            players->at(msg.sourceId).Deserialize(buffer, 1 + sizeof(int));
            players->at(msg.sourceId).posDirty = true;
        } else if (id == 3) {
            DespawnMessage msg;
            msg.Deserialize(buffer);

            msg.objectId = -msg.objectId;

            if (players->count(msg.objectId) != 0)  {            
                std::cout << "\033[1;33m Player " << -msg.objectId << " from server " << this->id << " left the border \033[1;37m" << std::endl;

                RemovePlayer(msg.objectId);
            }
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }

    std::cout << "\033[1;33m Server " << id << "'s thread dying \033[1;37m" << std::endl;
}

void Server::SpawnFor(Player& p1) {
    for (std::map<int, Player>::iterator it=players->begin(); it!=players->end(); it++) {
        int id = it->first;
        // Player& p = it->second;

        SpawnMessage spawnMsg;
        spawnMsg.prefabId = 0;
        spawnMsg.objectId = id;
        spawnMsg.hasAuthority = false;

        // Send Spawn
        game->SendMsgTo(&spawnMsg, p1);
    }
}

void Server::AddPlayer(int index, bool withMsg) {
    players->insert(std::pair<int,Player>(index, Player(index)));

    if (socket <= 0 || !withMsg)
        return;

    SpawnMessage spawnMsg;
    spawnMsg.prefabId = 0;
    spawnMsg.objectId = index;
    spawnMsg.hasAuthority = false;

    // Send Spawn
    game->SendMsgToAll(&spawnMsg);

    Player& p = players->at(index);

    EnableMessage emsg;
    emsg.objectId = index;
    emsg.toEnable = false;

    emsg.position[0] = p.position[0];
    emsg.position[1] = p.position[1];
    emsg.position[2] = p.position[2];

    emsg.rotation[0] = p.rotation[0];
    emsg.rotation[1] = p.rotation[1];
    emsg.rotation[2] = p.rotation[2];
    emsg.rotation[3] = p.rotation[3];

    game->SendMsgToAllNotInterested(&emsg, &p);  
}


void Server::RemovePlayer(int index, bool withMsg) {
    players->erase(index);

    if (socket <= 0 || !withMsg)
        return;

    DespawnMessage despawnMsg;

    despawnMsg.objectId = index;

    // Send Despawn
    game->SendMsgToAll(&despawnMsg);
}