#ifndef TRANSFORM_MESSAGE_HPP
#define TRANSFORM_MESSAGE_HPP

#include <iostream>

#include "networkMessage.hpp"
#include "../player.hpp"

class TransformMessage: public NetworkMessage {
    public:
        TransformMessage(): NetworkMessage(1, 1 + sizeof(int) + 7*sizeof(float)) {}

        ~TransformMessage() {}

        virtual char* Serialize() {
            short offset = 0;

            (*(int*)(buffer + offset)) = size;
            offset += sizeof(int);

            buffer[offset++] = id;

            
            (*(int*)(buffer + offset)) = sourceId;
            offset += sizeof(int);
            
            (*(float*)(buffer + offset)) = position[0];
            offset += sizeof(float);   
            (*(float*)(buffer + offset)) = position[1];
            offset += sizeof(float);   
            (*(float*)(buffer + offset)) = position[2];
            offset += sizeof(float);   

            (*(float*)(buffer + offset)) = rotation[0];
            offset += sizeof(float);   
            (*(float*)(buffer + offset)) = rotation[1];
            offset += sizeof(float);   
            (*(float*)(buffer + offset)) = rotation[2];
            offset += sizeof(float);   
            (*(float*)(buffer + offset)) = rotation[3];
            offset += sizeof(float);   

            return buffer;
        }

        virtual void Deserialize(char* buffer) {
            short offset = 1;
            
            sourceId = (*(int*)(buffer + offset));
            offset += sizeof(int);
            
            position[0] = (*(float*)(buffer + offset));
            offset += sizeof(float); 
            position[1] = (*(float*)(buffer + offset));
            offset += sizeof(float); 
            position[2] = (*(float*)(buffer + offset));
            offset += sizeof(float); 

            rotation[0] = (*(float*)(buffer + offset));
            offset += sizeof(float); 
            rotation[1] = (*(float*)(buffer + offset));
            offset += sizeof(float); 
            rotation[2] = (*(float*)(buffer + offset));
            offset += sizeof(float); 
            rotation[3] = (*(float*)(buffer + offset));
            offset += sizeof(float); 
        }

        int sourceId;
        float position[3];
        float rotation[4];
};
#endif