#ifndef SHOT_H
#define SHOT_H

#include "course.h"

struct LandingZone {
    int centerRow;
    int centerCol;
    int ellipseHeight;
    int rx;
    int wCap;
    int wMid;
};

struct Position {
    int row;
    int col;
};

LandingZone calculateLandingZone(int fromRow, int fromCol, int strengthInput, int directionInput, int parChoice, double playerStrength);
void displayWithLandingZone(const Course& course, const LandingZone& lz, int strengthInput, int directionInput, double effectivePower, int ballRow, int ballCol, int shots);
Position generateBallLanding(const Course& course, const LandingZone& lz);

#endif