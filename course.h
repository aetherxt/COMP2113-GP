#ifndef COURSE_H
#define COURSE_H
#include <set>
#include <utility>
#include <vector>

struct Course {
    int par;
    int width;
    int height;
    int holeRow;
    int holeCol;
    int greenRX;
    int greenRY;
    std::vector<std::vector<char> > map;
    std::set<std::pair<int, int> > waterHazards;  // Track water positions for penalty detection
};

Course generateCourse(int par);
void displayCourse(const Course& c);

#endif