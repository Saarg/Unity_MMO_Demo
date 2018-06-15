#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <map>

class PlayerThread;

class Player {
    public:
        Player(int id = 0);
        ~Player();

        short Serialize(char* buffer, short offset);
        short Deserialize(char* buffer, short offset);

        int GetSocket();

        int id = 0;

        PlayerThread* pthread;        

        bool posDirty = false;

        float position[3];
        float rotation[4];

        float interestRadius = 10;
        std::map<int, Player*> playerSeen;
};

#endif