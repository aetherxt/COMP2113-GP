#include "course.h"

#include <climits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

Course generateCourse(int par) {
    Course c;
    srand(static_cast<unsigned int>(time(0)));  // random seed

    c.par = par;

    c.width = 50;
    c.height = 24;

    c.map.resize(c.height, vector<char>(c.width, ' '));

    const int fairwayHalfWidth = 4;
    const int roughLayer = 2;
    const int treeLayer = 2;
    const int centerX = c.width / 2;
    const int maxDrift = 30;

    // fairway random generation
    vector<int> centerPath(c.height);
    centerPath[c.height - 1] = centerX;

    int dogLegDir = (rand() % 2 == 0) ? 1 : -1;

    for (int row = c.height - 2; row >= 0; row--) {
        int drift;
        int midStart = c.height * 2 / 3;
        int midEnd = c.height / 3;
        if (row < midStart && row >= midEnd) {
            drift = dogLegDir;
        } else {
            drift = (rand() % 3) - 1;
        }

        centerPath[row] = centerPath[row + 1] + drift;  // adding dogleg to course

        if (centerPath[row] < centerX - maxDrift)
            centerPath[row] = centerX - maxDrift;
        if (centerPath[row] > centerX + maxDrift)
            centerPath[row] = centerX + maxDrift;

        int margin = fairwayHalfWidth + roughLayer + treeLayer + 1;
        if (centerPath[row] < margin)
            centerPath[row] = margin;
        if (centerPath[row] >= c.width - margin)
            centerPath[row] = c.width - margin - 1;
    }

    // green generation
    const int greenCenterY = 3;
    const int greenCenterX = centerPath[greenCenterY];
    const int greenRX = 4;
    const int greenRY = 2;

    for (int row = 0; row < c.height; row++) {
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

    // green circle overlay
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

    // hole placement
    c.map[greenCenterY][greenCenterX] = 'H';

    // sand traps
    int numTraps = 2 + rand() % 2;
    for (int i = 0; i < numTraps; i++) {
        double angle;
        int attempts = 0;

        // put trap around green
        int trapX = greenCenterX + (int)round(cos(angle) * (greenRX + 1));
        int trapY = greenCenterY + (int)round(sin(angle) * (greenRY + 1));

        for (int dr = 0; dr <= 1; dr++) {
            for (int dc = 0; dc <= 1; dc++) {
                int r = trapY + dr;
                int col = trapX + dc;
                if (r >= 0 && r < c.height && col >= 0 && col < c.width) {
                    char curr = c.map[r][col];
                    // prevent sand in green, tee or hole
                    if (curr != 'H' && curr != 'T' && curr != '.') {
                        c.map[r][col] = 'S';
                    }
                }
            }
        }
    }

    // surround sand with rough
    for (int row = 0; row < c.height; row++) {
        for (int col = 0; col < c.width; col++) {
            if (c.map[row][col] == 'S') {
                for (int dr = -roughLayer; dr <= roughLayer; dr++) {
                    for (int dc = -roughLayer; dc <= roughLayer; dc++) {
                        int r = row + dr;
                        int co = col + dc;
                        if (r >= 0 && r < c.height && co >= 0 && co < c.width) {
                            char curr = c.map[r][co];
                            // only place rough on empty or tree cells
                            if (curr == ' ' || curr == '^') {
                                c.map[r][co] = '#';
                            }
                        }
                    }
                }
            }
        }
    }

    // surround course with trees
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

    // tee
    int teeY = c.height - 1;
    int teeX = centerPath[teeY];
    c.map[teeY][teeX] = 'T';

    return c;
}

void displayCourse(const Course& c) {
    cout << endl;
    cout << "  Par " << c.par << " Hole" << endl;
    cout << "  ";
    for (int col = 0; col < c.width; col++) cout << "-";
    cout << endl;

    for (int row = 0; row < c.height; row++) {
        cout << "  ";
        for (int col = 0; col < c.width; col++) {
            cout << c.map[row][col];
        }
        cout << endl;
    }

    cout << "  ";
    for (int col = 0; col < c.width; col++) cout << "-";
    cout << endl;
    cout << "  Legend: . Fairway/Green   H Hole     T Tee" << endl;
    cout << "          # Rough           ^ Tree     S Sand" << endl;
    cout << endl;
}