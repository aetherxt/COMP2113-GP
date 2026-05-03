#include "menu.h"

#include <iostream>
#include <limits>

#include "course.h"
using namespace std;

const int UPGRADE_COST = 2;
void displayMainMenu(const Player& p) {
    // main menu display
    cout << "TUI Golf" << endl;
    cout << " Coins: " << p.coins << endl;
    cout << " Strength: " << p.strength << " | Accuracy: " << p.accuracy << endl;
    cout << "  1. Play" << endl;
    cout << "  2. Upgrade" << endl;
    cout << "  0. Quit" << endl;
    cout << "Enter your choice: ";
}

int getMenuChoice() {
    // returns menu choice to main.cpp for switch
    int choice;
    while (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return choice;
}

void playGame(Player& p) {
    int parChoice = -1;

    while (parChoice != 3 && parChoice != 4 && parChoice != 5) {
        cout << "Select a Hole" << endl;
        cout << "  3. Par 3" << endl;
        cout << "  4. Par 4" << endl;
        cout << "  5. Par 5" << endl;
        cout << "Enter your choice: ";

        parChoice = getMenuChoice();

        if (parChoice != 3 && parChoice != 4 && parChoice != 5) {
            cout << "\nInvalid choice\n"
                 << endl;
        }
    }

    Course course = generateCourse(parChoice);
    displayCourse(course);

    // Game logic will be implemented here
    cout << "Game starting on Par " << parChoice << " hole..." << endl;
}

void upgradeShop(Player& p) {
    int choice = -1;

    while (choice != 0) {
        // shop display
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
            // strength iterates by 10 and accuracy iterates by 2 because the course is much longer than it is wide so this is relative to the course design
            if (p.coins >= UPGRADE_COST) {
                p.coins -= UPGRADE_COST;
                p.strength += 10;  // upgrade strength by 10, costs 2 coins
                cout << "\nStrength upgraded to " << p.strength << "\n"
                     << endl;
            } else {
                cout << "\nNot enough coins\n"
                     << endl;
            }
        } else if (choice == 2) {
            if (p.coins >= UPGRADE_COST) {
                p.coins -= UPGRADE_COST;
                p.accuracy += 2;  // upgrade accuracy by 2, costs 2 coins
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