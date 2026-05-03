#include "game.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// terminal colors
#define RED "\033[31m"
#define RESET "\033[0m"

using namespace std;

static int getInput() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return val;
}

void playRound(Player& p) {
    int parChoice = -1;

    while (parChoice != 3 && parChoice != 4 && parChoice != 5) {
        cout << "Select a Hole" << endl;
        cout << "  3. Par 3" << endl;
        cout << "  4. Par 4" << endl;
        cout << "  5. Par 5" << endl;
        cout << "Enter your choice: ";

        parChoice = getInput();

        if (parChoice != 3 && parChoice != 4 && parChoice != 5) {
            cout << "\nInvalid choice\n"
                 << endl;
        }
    }

    Course course = generateCourse(parChoice);
    displayCourse(course);

    // strength input 1-100
    int strengthInput;
    cout << "Enter strength (1-100): ";
    strengthInput = getInput();
    if (strengthInput < 1) strengthInput = 1;
    if (strengthInput > 100) strengthInput = 100;

    // direction input
    int directionInput;
    cout << "Enter direction (-45 to 45): ";
    directionInput = getInput();
    if (directionInput < -45) directionInput = -45;
    if (directionInput > 45) directionInput = 45;

    // tee position
    int teeY = -1, teeX = -1;
    for (int row = course.height - 1; row >= 0; row--) {
        for (int col = 0; col < course.width; col++) {
            if (course.map[row][col] == 'T') {
                teeY = row;
                teeX = col;
                break;
            }
        }
        if (teeY != -1) break;
    }

    // max distance relative to course
    double maxDist;
    if (parChoice == 3)
        maxDist = 18.0;
    else if (parChoice == 4)
        maxDist = 12.0;
    else
        maxDist = 7.0;

    // scale by player strength
    double effectivePower = (strengthInput / 100.0) * (p.strength / 100.0);
    double distance = effectivePower * maxDist;

    double dirRad = directionInput * M_PI / 180.0;

    int landY = teeY - (int)round(distance * cos(dirRad));
    int landX = teeX + (int)round(distance * sin(dirRad));

    // landing zone height tiers based on strength
    int ellipseHeight;
    if (strengthInput >= 80)
        ellipseHeight = 5;
    else if (strengthInput >= 60)
        ellipseHeight = 4;
    else
        ellipseHeight = 3;

    // landing zone width scales with strength
    int rx = (int)round(strengthInput * 0.10);
    if (rx < 2) rx = 2;

    // course map copy for display
    vector<vector<char>> displayMap = course.map;
    vector<vector<bool>> isEllipse(course.height, vector<bool>(course.width, false));

    // helpers for drawing onto map
    auto drawCell = [&](int row, int col, char ch) {
        if (row >= 0 && row < course.height && col >= 0 && col < course.width) {
            displayMap[row][col] = ch;
            isEllipse[row][col] = true;
        }
    };

    auto drawCapRow = [&](int row, int halfW, char leftCh, char rightCh) {
        int leftCol = landX - halfW;
        int rightCol = landX + halfW;
        drawCell(row, leftCol, leftCh);
        if (rightCol != leftCol) drawCell(row, rightCol, rightCh);
        for (int col = leftCol + 1; col < rightCol; col++) {
            drawCell(row, col, '-');
        }
    };

    auto drawSideRow = [&](int row, int halfW, char leftCh, char rightCh) {
        int leftCol = landX - halfW;
        int rightCol = landX + halfW;
        drawCell(row, leftCol, leftCh);
        if (rightCol != leftCol) drawCell(row, rightCol, rightCh);
    };

    int topRow = landY - ellipseHeight / 2;
    int wCap = (rx - 1 < 1) ? 1 : rx - 1;  // narrower top/bottom for rounded look
    int wMid = rx;

    if (ellipseHeight == 5) {
        drawCapRow(topRow + 0, wCap, '/', '\\');
        drawSideRow(topRow + 1, wMid, '/', '\\');
        drawSideRow(topRow + 2, wMid, '|', '|');
        drawSideRow(topRow + 3, wMid, '\\', '/');
        drawCapRow(topRow + 4, wCap, '\\', '/');
    } else if (ellipseHeight == 4) {
        drawCapRow(topRow + 0, wCap, '/', '\\');
        drawSideRow(topRow + 1, wMid, '|', '|');
        drawSideRow(topRow + 2, wMid, '|', '|');
        drawCapRow(topRow + 3, wCap, '\\', '/');
    } else {  // ellipseHeight == 3
        drawCapRow(topRow + 0, wCap, '/', '\\');
        drawSideRow(topRow + 1, wMid, '|', '|');
        drawCapRow(topRow + 2, wCap, '\\', '/');
    }

    // display
    cout << "\n  Landing Zone (Strength: " << strengthInput
         << "%, Direction: " << directionInput << "deg)" << endl;
    cout << "  Effective Power: " << (int)(effectivePower * 100) << "%" << endl;
    cout << "  ";
    for (int col = 0; col < course.width; col++) cout << "-";
    cout << endl;

    for (int row = 0; row < course.height; row++) {
        cout << "  ";
        for (int col = 0; col < course.width; col++) {
            if (isEllipse[row][col]) {
                cout << RED << displayMap[row][col] << RESET;
            } else {
                cout << displayMap[row][col];
            }
        }
        cout << endl;
    }

    cout << "  ";
    for (int col = 0; col < course.width; col++) cout << "-";
    cout << endl;
    cout << "  Legend: . Fairway/Green   H Hole     T Tee" << endl;
    cout << "          # Rough           ^ Tree     S Sand" << endl;
    cout << "          " << RED << "- | / \\" << RESET << " Landing Zone" << endl;
    cout << endl;
}