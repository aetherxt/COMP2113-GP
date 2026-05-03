#include "game.h"

#include <climits>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <set>

#include "shot.h"

using namespace std;

static int getInput() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(10000, '\n');
    return val;
}

// Get maximum strength for player based on terrain
static int getMaxStrengthForTerrain(char terrain) {
    switch (terrain) {
        case '#':  // Rough
            return 80;
        case '^':  // Trees
            return 60;
        case 'S':  // Sand
            return 30;
        case '~':  // Water
            return 40;
        default:
            return 100;
    }
}

// Find closest valid position when ball is out of bounds
static Position findClosestValidPosition(const Course& course, int row, int col) {
    Position closest;
    closest.row = row;
    closest.col = col;

    int minDist = INT_MAX;
    for (int r = 0; r < course.height; r++) {
        for (int c = 0; c < course.width; c++) {
            char terrain = course.map[r][c];
            if (terrain != ' ') {
                int dx = r - row;
                int dy = c - col;
                int dist = dx * dx + dy * dy;
                if (dist < minDist) {
                    minDist = dist;
                    closest.row = r;
                    closest.col = c;
                }
            }
        }
    }

    return closest;
}

// Main game loop for playing a round
void playRound(Player& p) {
    srand(static_cast<unsigned int>(time(0)));

    int parChoice = -1;
    // Course selection
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

    // Find tee position
    int ballRow = -1, ballCol = -1;
    for (int row = course.height - 1; row >= 0; row--) {
        for (int col = 0; col < course.width; col++) {
            if (course.map[row][col] == 'T') {
                ballRow = row;
                ballCol = col;
                break;
            }
        }
        if (ballRow != -1) break;
    }

    int shots = 0;
    bool holed = false;

    while (!holed) {
        cout << "Shots taken: " << shots << endl;

        int strengthInput, directionInput;
        bool confirmed = false;

        while (!confirmed) {
            // Get current terrain
            char currentTerrain = course.map[ballRow][ballCol];
            int maxStrength = getMaxStrengthForTerrain(currentTerrain);

            // Get strength input with validation
            bool validStrength = false;
            while (!validStrength) {
                cout << "Enter strength (1-" << maxStrength << "): ";
                strengthInput = getInput();
                if (strengthInput >= 1 && strengthInput <= maxStrength) {
                    validStrength = true;
                } else {
                    cout << "Invalid strength. Please enter a value between 1 and " << maxStrength << "." << endl;
                }
            }

            // Get direction input with validation
            bool validDirection = false;
            while (!validDirection) {
                cout << "Enter direction (-180 to 180): ";
                directionInput = getInput();
                if (directionInput >= -180 && directionInput <= 180) {
                    validDirection = true;
                } else {
                    cout << "Invalid direction. Please enter a value between -180 and 180." << endl;
                }
            }

            double effectivePower = (strengthInput / 100.0) * (p.strength / 100.0);

            LandingZone lz = calculateLandingZone(ballRow, ballCol, strengthInput, directionInput, parChoice, p.strength);

            // Show preview with current ball position
            displayWithLandingZone(course, lz, strengthInput, directionInput, effectivePower, ballRow, ballCol, shots + 1);

            cout << "Confirm shot? (1 = Take shot, 2 = Reselect): ";
            int confirm = getInput();

            if (confirm == 1) {
                confirmed = true;

                shots++;
                Position landing = generateBallLanding(course, lz);

                // Check if ball is out of bounds
                if (landing.row < 0 || landing.row >= course.height ||
                    landing.col < 0 || landing.col >= course.width ||
                    course.map[landing.row][landing.col] == ' ') {
                    cout << "Out of bounds. 1 stroke penalty." << endl;
                    shots++;
                    landing = findClosestValidPosition(course, landing.row, landing.col);
                }

                // Check if ball landed in water hazard
                if (course.waterHazards.count(std::make_pair(landing.row, landing.col)) > 0) {
                    cout << "Ball landed in water. 2 stroke penalty." << endl;
                    shots += 2;
                }

                ballRow = landing.row;
                ballCol = landing.col;

                // Display result with ball
                displayWithLandingZone(course, lz, strengthInput, directionInput, effectivePower, ballRow, ballCol, shots - 1);

                // Check if ball is on the hole
                if (ballRow == course.holeRow && ballCol == course.holeCol) {
                    holed = true;
                    cout << "Holed in with " << shots << " shot(s)!" << endl;
                } else {
                    // Check if ball is on the green
                    double dxg = (double)(ballCol - course.holeCol) / course.greenRX;
                    double dyg = (double)(ballRow - course.holeRow) / course.greenRY;
                    double greenDist = sqrt(dxg * dxg + dyg * dyg);

                    if (greenDist <= 1.0) {
                        holed = true;
                        cout << "   Hole Complete! Finished in " << shots << " shot(s)!" << endl;
                    } else {
                        // Print terrain
                        char terrain = course.map[ballRow][ballCol];
                        if (terrain == '~') {
                            cout << "  Ball landed in water." << endl;
                        } else if (terrain == '.') {
                            cout << "  Ball landed on the fairway." << endl;
                        } else if (terrain == '#') {
                            cout << "  Ball landed in the rough." << endl;
                        } else if (terrain == '^') {
                            cout << "  Ball landed in the trees." << endl;
                        } else if (terrain == 'S') {
                            cout << "  Ball landed in a sand trap" << endl;
                        } else {
                            cout << "  Ball landed." << endl;
                        }
                    }
                }
            } else {
                cout << "\n  Reselecting parameters...\n"
                     << endl;
            }
        }
    }

    // Print final score
    int score = shots - parChoice;
    cout << "\n  Final Score: " << shots << " shot(s) on a Par " << parChoice << "\nScore: (";
    if (shots == 1)
        cout << "Hole in 1";
    else if (score <= -3)
        cout << "Albatross (-3)";
    else if (score == -2)
        cout << "Eagle (-2)";
    else if (score == -1)
        cout << "Birdie (-1)";
    else if (score == 0)
        cout << "Par (0)";
    else if (score == 1)
        cout << "Bogey (1)";
    else if (score == 2)
        cout << "Double Bogey (2)";
    else if (score > 2)
        cout << score - 2 << " over Double Bogey";
    cout << ")" << endl
         << endl;

    // Award coins based on shots under par
    if (score < 0) {
        int coinsEarned = -score;
        p.coins += coinsEarned;
        cout << "  Coins earned: " << coinsEarned << endl;
        cout << "  Total coins: " << p.coins << endl;
    }
}