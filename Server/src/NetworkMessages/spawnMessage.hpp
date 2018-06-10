#ifndef SPAWN_MESSAGE_HPP
#define SPAWN_MESSAGE_HPP

#include <iostream>

#include "networkMessage.hpp"

class SpawnMessage: public NetworkMessage {
    public:
        SpawnMessage(): NetworkMessage(2, 1 + 2 * sizeof(int) + sizeof(bool)) {}

        ~SpawnMessage() {}

        virtual char* Serialize() {
            short offset = 0;

            (*(int*)(buffer + offset)) = size;
            offset += sizeof(int);

            buffer[offset++] = id;

            // player prefab id is 0
            (*(int*)(buffer + offset)) = prefabId;
            offset += sizeof(int);
            // client id
            (*(int*)(buffer + offset)) = objectId;
            offset += sizeof(int);   

            (*(bool*)(buffer + offset)) = hasAuthority;
            offset += sizeof(bool);   
            
            return buffer;
        }

        virtual void Deserialize(char* buffer) {
            short offset = 1;

            // player prefab id is 0
            prefabId = (*(int*)(buffer + offset));
            offset += sizeof(int);
            // object requesting spawn or id of new object
            objectId = (*(int*)(buffer + offset));
            offset += sizeof(int);   

            hasAuthority = (*(bool*)(buffer + offset));
            offset += sizeof(bool);   
        }

        int prefabId = -1;
        int objectId = -1;
        bool hasAuthority = false;
};
#endif