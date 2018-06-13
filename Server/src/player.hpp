#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>

class PlayerThread;

class Player {
    public:
        Player();
        ~Player();

        short Serialize(char* buffer, short offset);
        short Deserialize(char* buffer, short offset);

        PlayerThread* pthread;        

        bool posDirty;

        float position[3];
        float rotation[4];

        float interestRadius = 8;
        std::map<int, Player*> playerSeen;
};

#endif