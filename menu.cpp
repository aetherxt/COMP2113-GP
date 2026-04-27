#include <iostream>
#include <limits>
#include "menu.h"
using namespace std;

void displayMainMenu(const Player &p) {
    cout << "========================================" << endl;
    cout << "TUI Golf" << endl;
    cout << "Coins: " << p.coins << endl;
    cout << "  1. Play" << endl;
    cout << "  2. Upgrade" << endl;
    cout << "  q. Quit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

int getMenuChoice() {
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void playGame(Player &p) {
    
}

void upgradeShop(Player &p) {
    
}