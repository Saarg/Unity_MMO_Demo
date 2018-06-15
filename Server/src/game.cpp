#include "game.hpp"

#include "NetworkMessages/despawnMessage.hpp"
#include "NetworkMessages/enableMessage.hpp"
#include "NetworkMessages/spawnMessage.hpp"
#include "NetworkMessages/transformMessage.hpp"

#include "player.hpp"
#include "playerThread.hpp"

Game::Game(int serverId, std::vector<int>& clients, std::map<int, Server*>&  servers, std::map<int, Player>& players): 
    server(serverId, 0), clients(clients), servers(servers), players(players) {
    
    server.SetPlayers(&players);
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
    Player& p1 = players[clientSocket] = Player(clientSocket);

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

    for (std::map<int, Server*>::iterator it=servers.begin(); it!=servers.end(); it++) {
        it->second->SpawnFor(players[clientSocket]);
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

    for (std::map<int, Server*>::iterator it=servers.begin(); it!=servers.end(); it++) {
        it->second->RemovePlayer(clientSocket, false);
        msg.Send(it->second->GetSocket());
    }
}

void Game::SendMsgTo(NetworkMessage* msg, int targetId) {
    int socket = players[targetId].pthread->GetSocket();

    if (socket > 0)
        msg->Send(socket);
}

void Game::SendMsgTo(NetworkMessage* msg, Player& target) {
    int socket = target.pthread->GetSocket();

    if (socket > 0)
        msg->Send(socket);
}

void Game::SendMsgToAllInterested(NetworkMessage* msg, Player* p1) {
    SendMsgToAll(msg, p1);
}

void Game::SendMsgToAllNotInterested(NetworkMessage* msg, Player* p1) {
    SendMsgToAll(msg, p1, false);
}

void Game::SendMsgToAll(NetworkMessage* msg, Player* p1, bool interestedOnly) {
    for (std::map<int, Player>::iterator it=players.begin(); it!=players.end(); it++) {
        int id = it->first;
        Player& p2 = it->second;

        if (p1 == NULL) {
            SendMsgTo(msg, id);
            continue;
        }

        int p1Id = p1->id;

        float distVector[3] = {p2.position[0], p2.position[1], p2.position[2]};

        distVector[0] -= p1->position[0];
        distVector[1] -= p1->position[1];
        distVector[2] -= p1->position[2];

        float sqrMagnitude = distVector[0]*distVector[0] + distVector[1]*distVector[1] + distVector[2]*distVector[2];
        float sqrRadius = p2.interestRadius*p2.interestRadius;

        if (sqrMagnitude > sqrRadius) { // Player not in interest radius
            if (p2.playerSeen.count(p1Id) != 0) {
                EnableMessage emsg;
                emsg.objectId = p1Id;
                emsg.toEnable = false;

                emsg.position[0] = p1->position[0];
                emsg.position[1] = p1->position[1];
                emsg.position[2] = p1->position[2];
                
                emsg.rotation[0] = p1->rotation[0];
                emsg.rotation[1] = p1->rotation[1];
                emsg.rotation[2] = p1->rotation[2];
                emsg.rotation[3] = p1->rotation[3];

                p2.playerSeen.erase(p1Id);
                SendMsgTo(&emsg, id);                                   
            }

            if (!interestedOnly)
                SendMsgTo(msg, id);
        } else { // Player in interest radius
            if (p2.playerSeen.count(p1Id) == 0) {
                EnableMessage emsg;
                emsg.objectId = p1Id;
                emsg.toEnable = true;

                emsg.position[0] = p1->position[0];
                emsg.position[1] = p1->position[1];
                emsg.position[2] = p1->position[2];
                
                emsg.rotation[0] = p1->rotation[0];
                emsg.rotation[1] = p1->rotation[1];
                emsg.rotation[2] = p1->rotation[2];
                emsg.rotation[3] = p1->rotation[3];

                p2.playerSeen[p1Id] = p1;
                SendMsgTo(&emsg, id);                   
            }

            if (interestedOnly)
                SendMsgTo(msg, id);
        }
    }
}

void Game::RegisterServer(int id, int serverSocket) {
    std::cout << "\033[1;33m New server added with id " << id << " on socket " << serverSocket << "\033[1;37m" << std::endl;
    
    while (servers.count(id) != 0) {
        servers.erase(id);
    }

    servers[id] = new Server(id, serverSocket, this);
}

void Game::Loop() {
    while (true) {
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

        // Server tick start.
        for (std::map<int, Player>::iterator it=players.begin(); it!=players.end(); it++) {
            int p1Id = it->first;
            Player& p1 = it->second;

            // Update dirty pos
            if (p1.posDirty) {
                p1.posDirty = false;

                TransformMessage msg;
                msg.sourceId = p1Id;
                msg.position[0] = p1.position[0];
                msg.position[1] = p1.position[1];
                msg.position[2] = p1.position[2];

                msg.rotation[0] = p1.rotation[0];
                msg.rotation[1] = p1.rotation[1];
                msg.rotation[2] = p1.rotation[2];
                msg.rotation[3] = p1.rotation[3];

                for (std::vector<int>::iterator jt = clients.begin(); jt != clients.end(); jt++) {
                    if (p1Id == *jt)
                        continue;
                    
                    Player& p2 = players[*jt];                    
                    float distVector[3] = {p2.position[0], p2.position[1], p2.position[2]};

                    distVector[0] -= p1.position[0];
                    distVector[1] -= p1.position[1];
                    distVector[2] -= p1.position[2];

                    float sqrMagnitude = distVector[0]*distVector[0] + distVector[1]*distVector[1] + distVector[2]*distVector[2];
                    float sqrRadius = p2.interestRadius*p2.interestRadius;

                    if (sqrMagnitude > sqrRadius) { // Player not in interest radius
                        if (p2.playerSeen.count(p1Id) != 0) {
                            EnableMessage emsg;
                            emsg.objectId = p1Id;
                            emsg.toEnable = false;

                            emsg.position[0] = p1.position[0];
                            emsg.position[1] = p1.position[1];
                            emsg.position[2] = p1.position[2];
                            
                            emsg.rotation[0] = p1.rotation[0];
                            emsg.rotation[1] = p1.rotation[1];
                            emsg.rotation[2] = p1.rotation[2];
                            emsg.rotation[3] = p1.rotation[3];

                            p2.playerSeen.erase(p1Id);

                            emsg.Send(*jt);
                        }
                    } else { // Player in interest radius
                        if (p2.playerSeen.count(p1Id) == 0) {
                            EnableMessage emsg;
                            emsg.objectId = p1Id;
                            emsg.toEnable = true;

                            emsg.position[0] = p1.position[0];
                            emsg.position[1] = p1.position[1];
                            emsg.position[2] = p1.position[2];
                            
                            emsg.rotation[0] = p1.rotation[0];
                            emsg.rotation[1] = p1.rotation[1];
                            emsg.rotation[2] = p1.rotation[2];
                            emsg.rotation[3] = p1.rotation[3];

                            p2.playerSeen[p1Id] = &p1;

                            emsg.Send(*jt);                            
                        } else {
                            msg.Send(*jt);
                        }
                    }
                }  

                // Update for borders
                if (server.Id() == 9501 and servers.count(9502) != 0) {
                    Server* s = servers[9502];

                    if (p1.position[0] + p1.interestRadius > server.X() + server.Size_x()/2) {
                        msg.Send(s->GetSocket());

                        s->AddPlayer(p1Id, false);
                    } else if(s->ContainsPlayer(p1Id)) {
                        DespawnMessage msg;
                        msg.objectId = p1Id;
                        msg.Send(s->GetSocket());

                        s->RemovePlayer(p1Id, false);
                    }
                } else if(server.Id() == 9502 and servers.count(9501) != 0) {                
                        Server* s = servers[9501];

                    if (p1.position[0] - p1.interestRadius < server.X() - server.Size_x()/2) {
                        msg.Send(s->GetSocket());

                        s->AddPlayer(p1Id, false);                  
                    } else {
                        DespawnMessage msg;
                        msg.objectId = p1Id;
                        msg.Send(s->GetSocket());

                        s->RemovePlayer(p1Id, false);
                    }
                }
            }
        }

        for (std::map<int, Server*>::iterator it=servers.begin(); it!=servers.end(); it++) {
            it->second->Update();
        }
        // Server tick end.

        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();

        int duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start ).count();

        if (duration > 10) {
            std::cout << "\033[1;31m GameLoop took " << duration << " milliseconds \033[1;37m" << std::endl;
        } else {
            std::this_thread::sleep_for (std::chrono::milliseconds(33 - duration));
        }
    }
}
