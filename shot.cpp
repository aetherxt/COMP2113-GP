#include "shot.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Colors
#define RED "\033[31m"
#define GREEN_COLOR "\033[32m"
#define TREE_COLOR "\033[38;5;22m"
#define SAND_COLOR "\033[33m"
#define BLUE "\033[36m"
#define RESET "\033[0m"

using namespace std;

// Check if ball is on the green
static bool isOnGreen(int row, int col, const Course& course) {
    double dxg = (double)(col - course.holeCol) / course.greenRX;
    double dyg = (double)(row - course.holeRow) / course.greenRY;
    return sqrt(dxg * dxg + dyg * dyg) <= 1.0;
}

// Calculate landing zone based on input parameters and player strength
LandingZone calculateLandingZone(int fromRow, int fromCol, int strengthInput, int directionInput, int parChoice, double playerStrength) {
    LandingZone lz;

    double maxDist;
    if (parChoice == 3)
        maxDist = 18.0;
    else if (parChoice == 4)
        maxDist = 12.0;
    else
        maxDist = 7.0;

    // Calculate based on player strength stat and input
    double effectivePower = (strengthInput / 100.0) * (playerStrength / 100.0);
    double distance = effectivePower * maxDist;

    double dirRad = directionInput * M_PI / 180.0;

    lz.centerRow = fromRow - (int)round(distance * cos(dirRad));
    lz.centerCol = fromCol + (int)round(distance * sin(dirRad));

    // Different ball landing zone based on strength, the more strength the less accurate
    if (strengthInput >= 80)
        lz.ellipseHeight = 5;
    else if (strengthInput >= 60)
        lz.ellipseHeight = 4;
    else
        lz.ellipseHeight = 3;

    lz.rx = (int)round(strengthInput * 0.10);
    if (lz.rx < 2) lz.rx = 2;

    lz.wCap = (lz.rx - 1 < 1) ? 1 : lz.rx - 1;
    lz.wMid = lz.rx;

    return lz;
}

// Display the course with the landing zone overlay
void displayWithLandingZone(const Course& course, const LandingZone& lz, int strengthInput, int directionInput, double effectivePower, int ballRow, int ballCol, int shots) {
    vector<vector<char> > displayMap = course.map;
    vector<vector<bool> > isEllipse(course.height, vector<bool>(course.width, false));
    vector<vector<bool> > isBall(course.height, vector<bool>(course.width, false));

    int topRow = lz.centerRow - lz.ellipseHeight / 2;

    auto drawCell = [&](int row, int col, char ch) {
        if (row >= 0 && row < course.height && col >= 0 && col < course.width) {
            displayMap[row][col] = ch;
            isEllipse[row][col] = true;
        }
    };

    auto drawCapRow = [&](int row, int halfW, char leftCh, char rightCh) {
        int leftCol = lz.centerCol - halfW;
        int rightCol = lz.centerCol + halfW;
        drawCell(row, leftCol, leftCh);
        if (rightCol != leftCol) drawCell(row, rightCol, rightCh);
        for (int col = leftCol + 1; col < rightCol; col++) {
            drawCell(row, col, '-');
        }
    };

    auto drawSideRow = [&](int row, int halfW, char leftCh, char rightCh) {
        int leftCol = lz.centerCol - halfW;
        int rightCol = lz.centerCol + halfW;
        drawCell(row, leftCol, leftCh);
        if (rightCol != leftCol) drawCell(row, rightCol, rightCh);
    };

    // Draw ellipse using calculated parameters
    if (lz.ellipseHeight == 5) {
        drawCapRow(topRow + 0, lz.wCap, '/', '\\');
        drawSideRow(topRow + 1, lz.wMid, '/', '\\');
        drawSideRow(topRow + 2, lz.wMid, '|', '|');
        drawSideRow(topRow + 3, lz.wMid, '\\', '/');
        drawCapRow(topRow + 4, lz.wCap, '\\', '/');
    } else if (lz.ellipseHeight == 4) {
        drawCapRow(topRow + 0, lz.wCap, '/', '\\');
        drawSideRow(topRow + 1, lz.wMid, '|', '|');
        drawSideRow(topRow + 2, lz.wMid, '|', '|');
        drawCapRow(topRow + 3, lz.wCap, '\\', '/');
    } else {
        drawCapRow(topRow + 0, lz.wCap, '/', '\\');
        drawSideRow(topRow + 1, lz.wMid, '|', '|');
        drawCapRow(topRow + 2, lz.wCap, '\\', '/');
    }

    // Place ball on display
    if (ballRow >= 0 && ballRow < course.height && ballCol >= 0 && ballCol < course.width) {
        displayMap[ballRow][ballCol] = 'O';
        isBall[ballRow][ballCol] = true;
    }

    // Display header
    cout << "\n  Shot #" << shots << " | Strength: " << strengthInput
         << "%, Direction: " << directionInput << "deg" << endl;
    cout << "  ";
    for (int col = 0; col < course.width; col++) cout << "-";
    cout << endl;

    // display course with colors and landing zone overlay
    for (int row = 0; row < course.height; row++) {
        cout << "  ";
        for (int col = 0; col < course.width; col++) {
            char ch = displayMap[row][col];
            if (isBall[row][col]) {
                cout << BLUE << 'O' << RESET;
            } else if (isEllipse[row][col]) {
                cout << RED << ch << RESET;
            } else if (ch == 'T' || ch == 'H') {
                cout << BLUE << ch << RESET;
            } else if (ch == '^') {
                cout << TREE_COLOR << ch << RESET;
            } else if (ch == 'S') {
                cout << SAND_COLOR << ch << RESET;
            } else if (isOnGreen(row, col, course)) {
                cout << GREEN_COLOR << ch << RESET;
            } else if (ch == '/' || ch == '\\' || ch == '|' || ch == '_') {
                cout << GREEN_COLOR << ch << RESET;
            } else {
                cout << ch;
            }
        }
        cout << endl;
    }

    // Legend
    cout << "  ";
    for (int col = 0; col < course.width; col++) cout << "-";
    cout << endl;
    cout << "  Legend: " << GREEN_COLOR << "." << RESET << " Fairway/Green   "
         << BLUE << "H" << RESET << " Hole     " << BLUE << "T" << RESET << " Tee" << endl;
    cout << "          # Rough           " << TREE_COLOR << "^" << RESET << " Tree     " << SAND_COLOR << "S" << RESET << " Sand" << endl;
    cout << "          " << RED << "- | / \\" << RESET << " Landing Zone   "
         << BLUE << "O" << RESET << " Ball" << endl;
    cout << endl;
}

// Generate a random landing position for the ball within the landing zone
Position generateBallLanding(const Course& course, const LandingZone& lz) {
    vector<Position> interiorCells;

    int topRow = lz.centerRow - lz.ellipseHeight / 2;

    for (int i = 0; i < lz.ellipseHeight; i++) {
        int row = topRow + i;
        if (row < 0 || row >= course.height) continue;

        bool isCap = false;
        if (lz.ellipseHeight == 5) {
            isCap = (i == 0 || i == 4);
        } else if (lz.ellipseHeight == 4) {
            isCap = (i == 0 || i == 3);
        } else {
            isCap = (i == 0 || i == 2);
        }

        int halfW = isCap ? lz.wCap : lz.wMid;
        int leftCol = lz.centerCol - halfW;
        int rightCol = lz.centerCol + halfW;

        for (int col = leftCol; col <= rightCol; col++) {
            if (col >= 0 && col < course.width) {
                Position pos;
                pos.row = row;
                pos.col = col;
                interiorCells.push_back(pos);
            }
        }
    }

    if (interiorCells.empty()) {
        Position pos;
        pos.row = lz.centerRow;
        pos.col = lz.centerCol;
        return pos;
    }

    int idx = rand() % interiorCells.size();
    return interiorCells[idx];
}