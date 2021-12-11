#include <iostream>
#include <string>
#include <vector>
#include "../shared/Point.h"
#include <map>

using namespace std;

typedef map<Point2, int> Octos;

void Inc(Octos& octos, Point2 p) {
    auto& e = octos[p];
    if (++e == 10) {
        p.DoNeighbours([&](auto n) {
            if (octos.find(n) != octos.end()) {
                Inc(octos, n);
            }});
    }
}

int main()
{
    Octos octos;
    int y = 0;
    
    while (!cin.eof()) {
        string s;
        cin >> s;

        for (int x = 0; x < s.length(); ++x) {
            octos[{x, y}] = s[x] - '0';
        }

        ++y;
    }

    int part1 = 0;
    int part2 = 0;
    for (int t = 0; t < 100 || part2 == 0; ++t) {
        int f = 0;
        for (auto& o : octos) {
            Inc(octos, o.first);
        }

        for (auto& o : octos) {
            if (o.second >= 10) {
                ++f;
                o.second = 0;
            }
        }

        cout << "Day " << t << ": " << f << " flashes" << endl;

        if (t < 100) {
            part1 += f;
        }

        if (f == 100 && part2 == 0) {
            part2 = t + 1;
        }
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}