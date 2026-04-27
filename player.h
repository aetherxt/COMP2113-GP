#ifndef PLAYER_H
#define PLAYER_H

#include <string>

struct Player {
    int coins;
    int strength;
    int accuracy;
};

void initPlayer(Player &p);
bool loadPlayer(Player& p, const std::string& filename);
bool savePlayer(const Player& p, const std::string& filename);

#endif