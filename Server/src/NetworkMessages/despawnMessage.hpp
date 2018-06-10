#ifndef DESPAWN_MESSAGE_HPP
#define DESPAWN_MESSAGE_HPP

#include <iostream>

#include "networkMessage.hpp"

class DespawnMessage: public NetworkMessage {
    public:
        DespawnMessage(): NetworkMessage(3, 1 + sizeof(int)) {}

        ~DespawnMessage() {}

        virtual char* Serialize() {
            short offset = 0;

            (*(int*)(buffer + offset)) = size;
            offset += sizeof(int);

            buffer[offset++] = id;

            // client id
            (*(int*)(buffer + offset)) = objectId;
            offset += sizeof(int);
            

            return buffer;
        }

        virtual void Deserialize(char* buffer) {
            short offset = 1;

            // client id
            objectId = (*(int*)(buffer + offset));
            offset += sizeof(int);   
        }

        int objectId = -1;
};
#endif