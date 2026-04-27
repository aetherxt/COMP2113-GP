#ifndef MENU_H
#define MENU_H
#include "player.h"

void displayMainMenu(const Player &p);
int getMenuChoice();
void playGame(Player &p);
void upgradeShop(Player &p);

#endif