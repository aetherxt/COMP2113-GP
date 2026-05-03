#ifndef COURSE_H
#define COURSE_H
#include <vector>

struct Course {
    int par;
    int width;
    int height;
    std::vector<std::vector<char> > map;
};
Course generateCourse(int par);
void displayCourse(const Course& c);

#endif