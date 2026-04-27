#include <iostream>
#include "menu.h"
#include "player.h"

using namespace std;

int main() {
    Player player;
    initPlayer(player);

    int choice = -1;
    while (choice != 0) {
        displayMainMenu(player);
        choice = getMenuChoice();

        switch (choice) {
            case 1:
                playGame(player);
                break;
            case 2:
                upgradeShop(player);
                break;
            case 0:
                cout << "\nThanks for playing" << endl;
                break;
            default:
                cout << "\nPlease try again.\n" << endl;
                break;
        }
    }

    return 0;
}