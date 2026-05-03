#include "menu.h"

#include <iostream>

#include "game.h"

using namespace std;

const int UPGRADE_COST = 2;

void displayMainMenu(const Player& p) {
    cout << "TUI Golf" << endl;
    cout << " Coins: " << p.coins << endl;
    cout << " Strength: " << p.strength << " | Accuracy: " << p.accuracy << endl;
    cout << "  1. Play" << endl;
    cout << "  2. Upgrade" << endl;
    cout << "  0. Quit" << endl;
    cout << "Enter your choice: ";
}

int getMenuChoice() {
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter a number: ";
    }

    cin.ignore(10000, '\n');
    return choice;
}

void playGame(Player& p) {
    playRound(p);
}

void upgradeShop(Player& p) {
    int choice = -1;

    while (choice != 0) {
        cout << "Shop" << endl;
        cout << " Coins: " << p.coins << endl;
        cout << " Strength: " << p.strength << " | Accuracy: " << p.accuracy << endl;
        cout << "  1. Upgrade Strength +10";
        cout << "     Cost: " << UPGRADE_COST << " coins" << endl;
        cout << "  2. Upgrade Accuracy +2";
        cout << "      Cost: " << UPGRADE_COST << " coins" << endl;
        cout << "  0. Back to Main Menu" << endl;
        cout << "Enter your choice: ";

        choice = getMenuChoice();

        if (choice == 1) {
            if (p.coins >= UPGRADE_COST) {
                p.coins -= UPGRADE_COST;
                p.strength += 10;
                cout << "\nStrength upgraded to " << p.strength << "\n"
                     << endl;
            } else {
                cout << "\nNot enough coins\n"
                     << endl;
            }
        } else if (choice == 2) {
            if (p.coins >= UPGRADE_COST) {
                p.coins -= UPGRADE_COST;
                p.accuracy += 2;
                cout << "\nAccuracy upgraded to " << p.accuracy << "\n"
                     << endl;
            } else {
                cout << "\nNot enough coins\n"
                     << endl;
            }
        } else if (choice == 0) {
            cout << "\nReturning to main menu\n"
                 << endl;
        } else {
            cout << "\nInvalid choice\n"
                 << endl;
        }
    }
}