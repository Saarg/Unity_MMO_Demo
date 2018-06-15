#include "player.hpp"

#include "playerThread.hpp"

Player::Player(int id): id(id), playerSeen() {
        
}

Player::~Player() {
    
}

short Player::Serialize(char* buffer, short offset) {
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

    return offset;
}

short Player::Deserialize(char* buffer, short offset) {    
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

    return offset;
}

int Player::GetSocket() { 
    return pthread != NULL ? pthread->GetSocket() : 0;
}