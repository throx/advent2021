#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <map>
#include "../shared/Point.h"

using namespace std;

const regex INPUT_R("(\\d+),(\\d+) -> (\\d+),(\\d+)");
typedef map<Point2, int> Vents;

void PlotLine(Vents &v, const Point2& p1, const Point2& p2, bool diag)
{
    if (p1[0] == p2[0]) {
        __int64 start = min(p1[1], p2[1]);
        __int64 end = max(p1[1], p2[1]);
        for (auto i = start; i <= end; ++i) {
            v[Point2({ p1[0], i })] += 1;
        }

        return;
    }

    if (p1[1] == p2[1]) {
        __int64 start = min(p1[0], p2[0]);
        __int64 end = max(p1[0], p2[0]);
        for (auto i = start; i <= end; ++i) {
            v[Point2({ i, p1[1] })] += 1;
        }

        return;
    }

    if (abs(p1[0] - p2[0]) == abs(p1[1] - p2[1]))
    {
        if (!diag) return;

        __int64 xinc = (p1[0] < p2[0]) ? 1 : -1;
        __int64 yinc = (p1[1] < p2[1]) ? 1 : -1;
        auto y = p1[1];
        for (auto x = p1[0]; x != p2[0] + xinc; x += xinc) {
            v[Point2({ x, y })] += 1;
            y += yinc;
        }

        return;
    }

    throw "Boom";
}

void PlotXYLine(Vents& v, const Point2& p1, const Point2& p2)
{
    PlotLine(v, p1, p2, false);
}

void PlotXYDLine(Vents& v, const Point2& p1, const Point2& p2)
{
    PlotLine(v, p1, p2, true);
}

int main()
{
    Vents vents1;
    Vents vents2;

    string s;
    while (getline(cin, s)) {
        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            Point2 p1({ stoi(sm[1]), stoi(sm[2]) });
            Point2 p2({ stoi(sm[3]), stoi(sm[4]) });
            PlotXYLine(vents1, p1, p2);
            PlotXYDLine(vents2, p1, p2);
        }
        else {
            throw "Boom";
        }
    }

    auto part1 = count_if(vents1.begin(), vents1.end(), [&](auto x) { return x.second >= 2; });
    auto part2 = count_if(vents2.begin(), vents2.end(), [&](auto x) { return x.second >= 2; });


    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}