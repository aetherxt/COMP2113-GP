#include "player.h"
#include <fstream>
using namespace std;

void initPlayer(Player &p) {
    p.coins = 0;
    p.strength = 100;
    p.accuracy = 1;
}

bool loadPlayer(Player& p, const string& filename) {
    ifstream fin;
    fin.open(filename.c_str());

    if (fin.fail()) {
        return false;
    }

    fin >> p.coins >> p.strength >> p.accuracy;

    if (fin.fail()) {
        fin.close();
        return false;
    }

    fin.close();
    return true;
}

bool savePlayer(const Player& p, const string& filename) {
    ofstream fout;
    fout.open(filename.c_str());

    if (fout.fail()) {
        return false;
    }

    fout << p.coins << ' ' << p.strength << ' ' << p.accuracy << endl;

    fout.close();
    return true;
}