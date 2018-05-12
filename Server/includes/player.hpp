#ifndef PLAYER_HPP
#define PLAYER_HPP

class Player {
    public:
        bool posDirty;

        float x = 0;
        float y = 0;
        float z = 0;

        float qw = 0;
        float qx = 0;
        float qy = 0;
        float qz = 0;
};

#endif