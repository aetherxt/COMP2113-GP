#include "course.h"

#include <climits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Colors
#define GREEN_COLOR "\033[32m"
#define TREE_COLOR "\033[38;5;22m"
#define SAND_COLOR "\033[33m"
#define WATER_COLOR "\033[34m"
#define BLUE "\033[36m"
#define RESET "\033[0m"

using namespace std;

// Check if ball is on the green
static bool isOnGreen(int row, int col, int greenCenterY, int greenCenterX, int greenRX, int greenRY) {
    double dxg = (double)(col - greenCenterX) / greenRX;
    double dyg = (double)(row - greenCenterY) / greenRY;
    return sqrt(dxg * dxg + dyg * dyg) <= 1.0;
}

// Calculate fairway half-width based on row with smooth variation
static int getFairwayHalfWidth(int row, int courseHeight) {
    double t = (double)row / courseHeight;
    double halfWidth = 4.25 + 1.75 * sin(t * M_PI * 1.5);  // Use sine wave to model variation in width
    return (int)round(halfWidth);
}

// Generate a random course based on the selected par
Course generateCourse(int par) {
    Course c;
    srand(static_cast<unsigned int>(time(0)));

    c.par = par;

    c.width = 50;
    c.height = 24;

    c.map.resize(c.height, vector<char>(c.width, ' '));

    const int roughLayer = 2;
    const int treeLayer = 2;
    const int centerX = c.width / 2;
    const int maxDrift = 30;

    vector<int> centerPath(c.height);
    centerPath[c.height - 1] = centerX;

    int dogLegDir = (rand() % 2 == 0) ? 1 : -1;
    int dogLegMultiplier = 2 + rand() % 2;  // 2 or 3 for max angle
    int dogLegShift = 0;
    int midStart = c.height * 2 / 3;
    int midEnd = c.height / 3;
    bool hasDogLeg = (rand() % 100 < 50); // 50% chance to have a dog leg

    // Generate fairway with possible dogleg
    for (int row = c.height - 2; row >= 0; row--) {
        int drift;
        if (hasDogLeg && row < midStart && row >= midEnd) {
            drift = dogLegDir * dogLegMultiplier;
            dogLegShift += abs(drift);
        } else {
            drift = (rand() % 3) - 1;
        }

        centerPath[row] = centerPath[row + 1] + drift;

        if (centerPath[row] < centerX - maxDrift)
            centerPath[row] = centerX - maxDrift;
        if (centerPath[row] > centerX + maxDrift)
            centerPath[row] = centerX + maxDrift;

        int margin = getFairwayHalfWidth(row, c.height) + roughLayer + treeLayer + 1;
        if (centerPath[row] < margin)
            centerPath[row] = margin;
        if (centerPath[row] >= c.width - margin)
            centerPath[row] = c.width - margin - 1;
    }

    // Calculate dog leg angle
    int dogLegVertical = midStart - midEnd;
    double dogLegAngle = atan((double)dogLegShift / dogLegVertical) * 180.0 / M_PI;
    bool hasWaterHazard = (dogLegAngle > 30.0);

    const int greenCenterY = 3;
    const int greenCenterX = centerPath[greenCenterY];
    const int greenRX = 4;
    const int greenRY = 2;

    c.holeRow = greenCenterY;
    c.holeCol = greenCenterX;
    c.greenRX = greenRX;
    c.greenRY = greenRY;

    // Fill in terrain
    for (int row = 0; row < c.height; row++) {
        int fairwayHalfWidth = getFairwayHalfWidth(row, c.height);
        for (int col = 0; col < c.width; col++) {
            int distFairway = INT_MAX;
            if (row >= greenCenterY) {
                distFairway = abs(col - centerPath[row]);
            }

            double dxg = (double)(col - greenCenterX) / greenRX;
            double dyg = (double)(row - greenCenterY) / greenRY;
            double greenDist = sqrt(dxg * dxg + dyg * dyg);

            double dxR = (double)(col - greenCenterX) / (greenRX + roughLayer);
            double dyR = (double)(row - greenCenterY) / (greenRY + roughLayer);
            double roughEllipse = sqrt(dxR * dxR + dyR * dyR);

            double dxT = (double)(col - greenCenterX) / (greenRX + roughLayer + treeLayer);
            double dyT = (double)(row - greenCenterY) / (greenRY + roughLayer + treeLayer);
            double treeEllipse = sqrt(dxT * dxT + dyT * dyT);

            bool inFairway = (distFairway <= fairwayHalfWidth);
            bool inGreen = (greenDist <= 1.0);

            if (inFairway || inGreen) {
                c.map[row][col] = '.';
            } else if (distFairway <= fairwayHalfWidth + roughLayer || roughEllipse <= 1.0) {
                c.map[row][col] = '#';
            } else if (distFairway <= fairwayHalfWidth + roughLayer + treeLayer || treeEllipse <= 1.0) {
                c.map[row][col] = '^';
            }
        }
    }

    // Add green and hole
    for (int row = 0; row < c.height; row++) {
        for (int col = 0; col < c.width; col++) {
            double dxg = (double)(col - greenCenterX) / greenRX;
            double dyg = (double)(row - greenCenterY) / greenRY;
            double dist = sqrt(dxg * dxg + dyg * dyg);

            if (dist > 0.85 && dist <= 1.05) {
                int dx = col - greenCenterX;
                int dy = row - greenCenterY;
                char ch;

                if (dy < 0 && abs(dx) <= greenRX / 2)
                    ch = '_';
                else if (dy > 0 && abs(dx) <= greenRX / 2)
                    ch = '_';
                else if (dx < 0 && dy < 0)
                    ch = '/';
                else if (dx > 0 && dy < 0)
                    ch = '\\';
                else if (dx > 0 && dy > 0)
                    ch = '/';
                else if (dx < 0 && dy > 0)
                    ch = '\\';
                else
                    ch = '|';

                c.map[row][col] = ch;
            }
        }
    }

    c.map[greenCenterY][greenCenterX] = 'H';

    int numTraps = 2 + rand() % 2;
    for (int i = 0; i < numTraps; i++) {
        double angle = (rand() % 360) * M_PI / 180.0;

        int trapX = greenCenterX + (int)round(cos(angle) * (greenRX + 1));
        int trapY = greenCenterY + (int)round(sin(angle) * (greenRY + 1));

        // Add sand traps
        for (int dr = 0; dr <= 1; dr++) {
            for (int dc = 0; dc <= 1; dc++) {
                int r = trapY + dr;
                int col = trapX + dc;
                if (r >= 0 && r < c.height && col >= 0 && col < c.width) {
                    char curr = c.map[r][col];
                    if (curr != 'H' && curr != 'T' && curr != '.') {
                        c.map[r][col] = 'S';
                    }
                }
            }
        }
    }

    // Add water hazards if dog leg angle is above 30 degrees
    if (hasWaterHazard) {
        int waterRow = (midStart + midEnd) / 2;
        // Place water on the inside of the dog leg
        int waterCol = centerPath[waterRow] - (dogLegDir > 0 ? 6 : -6);

        // Add water hazard
        for (int dr = -2; dr <= 2; dr++) {
            for (int dc = -2; dc <= 2; dc++) {
                int r = waterRow + dr;
                int col = waterCol + dc;
                if (r >= 0 && r < c.height && col >= 0 && col < c.width) {
                    char curr = c.map[r][col];
                    if (curr != 'H' && curr != 'T' && curr != '.') {
                        c.map[r][col] = '~';
                        c.waterHazards.insert(std::make_pair(r, col));
                    }
                }
            }
        }

        // Add trees around water
        for (int dr = -3; dr <= 3; dr++) {
            for (int dc = -3; dc <= 3; dc++) {
                if (dr >= -2 && dr <= 2 && dc >= -2 && dc <= 2) continue;  // Skip water area
                int r = waterRow + dr;
                int col = waterCol + dc;
                if (r >= 0 && r < c.height && col >= 0 && col < c.width) {
                    char curr = c.map[r][col];
                    if (curr == ' ' || curr == '#') {
                        c.map[r][col] = '^';
                    }
                }
            }
        }
    }

    // Add rough around sand traps
    for (int row = 0; row < c.height; row++) {
        for (int col = 0; col < c.width; col++) {
            if (c.map[row][col] == 'S') {
                for (int dr = -roughLayer; dr <= roughLayer; dr++) {
                    for (int dc = -roughLayer; dc <= roughLayer; dc++) {
                        int r = row + dr;
                        int co = col + dc;
                        if (r >= 0 && r < c.height && co >= 0 && co < c.width) {
                            char curr = c.map[r][co];
                            if (curr == ' ' || curr == '^') {
                                c.map[r][co] = '#';
                            }
                        }
                    }
                }
            }
        }
    }

    // Add trees around rough
    for (int row = 0; row < c.height; row++) {
        for (int col = 0; col < c.width; col++) {
            if (c.map[row][col] == '#') {
                for (int dr = -treeLayer; dr <= treeLayer; dr++) {
                    for (int dc = -treeLayer; dc <= treeLayer; dc++) {
                        int r = row + dr;
                        int co = col + dc;
                        if (r >= 0 && r < c.height && co >= 0 && co < c.width) {
                            if (c.map[r][co] == ' ') {
                                c.map[r][co] = '^';
                            }
                        }
                    }
                }
            }
        }
    }

    int teeY = c.height - 1;
    int teeX = centerPath[teeY];
    c.map[teeY][teeX] = 'T';  // Place tee

    return c;
}

// Display the course in terminal and add colors
void displayCourse(const Course& c) {
    cout << endl;
    cout << "  Par " << c.par << " Hole" << endl;
    cout << "  ";
    for (int col = 0; col < c.width; col++) cout << "-";
    cout << endl;

    for (int row = 0; row < c.height; row++) {
        cout << "  ";
        for (int col = 0; col < c.width; col++) {
            char ch = c.map[row][col];
            if (ch == 'T' || ch == 'H') {
                cout << BLUE << ch << RESET;
            } else if (ch == '^') {
                cout << TREE_COLOR << ch << RESET;
            } else if (ch == 'S') {
                cout << SAND_COLOR << ch << RESET;
            } else if (ch == '~') {
                cout << WATER_COLOR << ch << RESET;
            } else if (isOnGreen(row, col, c.holeRow, c.holeCol, c.greenRX, c.greenRY)) {
                cout << GREEN_COLOR << ch << RESET;
            } else if (ch == '/' || ch == '\\' || ch == '|' || ch == '_') {
                cout << GREEN_COLOR << ch << RESET;
            } else {
                cout << ch;
            }
        }
        cout << endl;
    }

    // Print legend
    cout << "  ";
    for (int col = 0; col < c.width; col++) cout << "-";
    cout << endl;
    cout << "  Legend: " << GREEN_COLOR << "." << RESET << " Fairway/Green   "
         << BLUE << "H" << RESET << " Hole     " << BLUE << "T" << RESET << " Tee" << endl;
    cout << "          # Rough           " << TREE_COLOR << "^" << RESET << " Tree     " << SAND_COLOR << "S" << RESET << " Sand"
         << "     " << WATER_COLOR << "~" << RESET << " Water" << endl;
    cout << endl;
}