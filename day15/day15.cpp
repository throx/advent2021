#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <chrono>
#include "../shared/Point.h"

using namespace std;

int cost_to_target(const Point2& target, map<Point2, int>& cave)
{
    map<Point2, int> cost;

    // Lol, Dijkstra is faster.  Simpler comparison I guess?
    auto dijkstra_compare = [&](auto& x, auto& y) {
        return cost[x] > cost[y];
    };

    auto astar_compare = [&](auto& x, auto& y) {
        return cost[x] + Dist(x, target) > cost[y] + Dist(y, target);
    };

    priority_queue<Point2, vector<Point2>, decltype(astar_compare)> q(astar_compare);
    q.push({ 0, 0 });

    while (!q.empty()) {
        Point2 pos = q.top();
        if (pos == target)
        {
            break;
        }
        q.pop();

        pos.DoSquareNeighbours([&](auto& n) {
            if (cave.find(n) == cave.end()) return;
            int new_cost = cave[n] + cost[pos];
            if (cost.find(n) != cost.end()) {
                if (cost[n] <= new_cost) {
                    return;
                }
            }

            cost[n] = new_cost;
            q.push(n);
            });
    }

    return cost[target];
}

int main()
{
    auto t1 = chrono::high_resolution_clock::now();
    map<Point2, int> cave;

    int maxy = 0;
    int maxx = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;
        maxx = s.length();

        for (int x = 0; x < s.length(); ++x) {
            cave[{x, maxy}] = s[x] - '0';
        }

        ++maxy;
    }

    cout << "Part 1: " << cost_to_target({ maxx - 1, maxy - 1 }, cave) << endl;

    // Make 5 times bigger
    map<Point2, int> bigcave;
    for (int tx = 0; tx < 5; ++tx) {
        for (int ty = 0; ty < 5; ++ty) {
            for (auto& p : cave) {
                bigcave[{p.first[0] + tx * maxx, p.first[1] + ty * maxy}] = (p.second + tx + ty - 1) % 9 + 1;
            }
        }
    }
    cout << "Part 2: " << cost_to_target({ maxx * 5 - 1, maxy * 5 - 1 }, bigcave) << endl;
    auto t2 = chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << endl;
}