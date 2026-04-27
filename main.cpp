#include <iostream>

#include "menu.h"
#include "player.h"

using namespace std;

const string SAVE_FILE = "player.txt";
int main() {
    Player player;

    if (!loadPlayer(player, SAVE_FILE)) {
        initPlayer(player);
        savePlayer(player, SAVE_FILE);
    }

    int choice = -1;

    while (choice != 0) {
        displayMainMenu(player);
        choice = getMenuChoice();

        switch (choice) {
            case 1:
                playGame(player);
                savePlayer(player, SAVE_FILE);
                break;
            case 2:
                upgradeShop(player);
                savePlayer(player, SAVE_FILE);
                break;
            case 0:
                savePlayer(player, SAVE_FILE);
                cout << "\nPlayer saved" << endl;
                cout << "Thanks for playing" << endl;
                break;
            default:
                cout << "\nInvalid choice\n" << endl;
                break;
        }
    }

    return 0;
}