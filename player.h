#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Player {
    int coins;
    int strength;
    int accuracy;
};

void initPlayer(Player &p);

#endif