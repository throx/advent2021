#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "../shared/Point.h"

using namespace std;

typedef map<Point2, int> Depths;

int main()
{
    Depths depths;
    int y = 0;
    int maxx = 0;
    while (!cin.eof()) {
        string s;
        cin >> s;

        for (int x = 0; x < s.length(); ++x) {
            depths[{x, y}] = (s[x] - '0');
        }
        maxx = s.length();
        ++y;
    }
    int maxy = y;

    // Use a multiset to auto-sort for us
    multiset<int> basins;

    int part1 = 0;
    for (auto& p : depths) {
        bool lowest = true;

        // Part 1 - if we're at the lowest point.
        p.first.DoSquareNeighbours([&](auto& x) {
            auto it = depths.find(x);
            if (it != depths.end()) {
                if (it->second <= p.second) {
                    lowest = false;
                }
            }});

        if (lowest) {
            part1 += (p.second + 1);

            // Floodfill the basin - set up todo and done
            set<Point2> todo = { p.first };
            set<Point2> done;

            // While we have work
            while (!todo.empty()) {

                // Grab first item and move to done
                auto j = *todo.begin();
                todo.erase(todo.begin());
                done.insert(j);

                // For each neighbour of the work item
                j.DoSquareNeighbours([&](auto& x) {

                    // If we're in the map
                    auto it = depths.find(x);
                    if (it != depths.end()) {

                        // And it's not 9
                        if (it->second != 9) {

                            // And we haven't already done it
                            if (done.find(it->first) == done.end()) {

                                // Add it to do (set will take care of dupes)
                                todo.insert(it->first);
                            }
                        }
                    }});
            }

            // Add the basin size to the list
            basins.insert(done.size());
        }
    }

    auto it = basins.rbegin();
    int part2 = 1;
    for (int i = 0; i < 3; ++i) {
        part2 *= *it;
        ++it;
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}