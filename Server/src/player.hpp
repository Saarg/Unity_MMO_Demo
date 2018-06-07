#ifndef PLAYER_HPP
#define PLAYER_HPP

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
};

#endif