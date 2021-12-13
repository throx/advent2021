#include <iostream>
#include <string>
#include "../shared/Point.h"
#include "../shared/Split.h"
#include <vector>

using namespace std;

struct Fold {
    int axis;
    int pos;
};

Point2 DoFold(Point2 p, const Fold& f)
{
    if (p[f.axis] < f.pos) {
        return p;
    }
    if (f.axis == 0) {
        return Point2({ f.pos * 2 - p[0], p[1] });
    }
    return Point2({ p[0], f.pos * 2 - p[1] });
}

int main()
{
    set<Point2> pts;

    string s("x"s);
    while (true) {
        getline(cin, s);
        if (s.empty()) {
            break;
        }

        auto v = SplitI64(s, ',');
        pts.insert(Point2({ v[0], v[1] }));
    }

    vector<Fold> folds;
    while (getline(cin, s)) {
        folds.push_back({
            s[11] == 'x' ? 0 : 1,
            stoi(s.substr(13))
            });
    }

    // Part 1
    set<Point2> folded;
    for (auto& p : pts) {
        folded.insert(DoFold(p, folds[0]));
    }

    // Part 2
    folded.clear();
    for (auto p : pts) {
        for (auto& f : folds) {
            p = DoFold(p, f);
        }
        folded.insert(p);
    }

    cout << "Part 1: " << folded.size() << endl;
    cout << "Part 2: " << endl;

    vector<string> output;
    for (auto& p : folded) {
        if (output.size() <= p[1]) {
            output.resize(p[1] + 1);
        }
        if (output[p[1]].length() <= p[0]) {
            output[p[1]].resize(p[0] + 1, ' ');
        }
        output[p[1]][p[0]] = 'Û';
    }

    for (auto& line : output) {
        cout << line << endl;
    }
}