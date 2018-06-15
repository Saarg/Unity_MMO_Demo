#include "server.hpp"

#include <unistd.h>

#include "game.hpp"
#include "NetworkMessages/transformMessage.hpp"
#include "NetworkMessages/spawnMessage.hpp"

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
                std::cout << "Player from server " << this->id << " entered the border" << std::endl;
                players->insert(std::pair<int,Player>(msg.sourceId, Player()));
                Player& p = players->at(msg.sourceId);

                p.Deserialize(buffer, 1 + sizeof(int));

                SpawnMessage spawnMsg;
                spawnMsg.prefabId = 0;
                spawnMsg.objectId = msg.sourceId;
                spawnMsg.hasAuthority = false;

                // Send Spawn
                game->SendMsgToAll(&spawnMsg);
            }

            players->at(msg.sourceId).Deserialize(buffer, 1 + sizeof(int));
            players->at(msg.sourceId).posDirty = true;
        }

        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
}