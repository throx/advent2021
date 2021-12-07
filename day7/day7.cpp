#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "../shared/Split.h"

using namespace std;

int main()
{
    string s;
    getline(cin, s);

    vector<int> crabs = SplitInt(s);
    int mx = *max_element(crabs.begin(), crabs.end());

    int part1 = numeric_limits<int>::max();
    int part2 = numeric_limits<int>::max();
    for (int x = 0; x <= mx; ++x) {
        int fuel1 = accumulate(crabs.begin(), crabs.end(), 0, [&](int s, auto& c) {
            return s + abs(c - x); 
            });
        part1 = min(part1, fuel1);

        int fuel2 = accumulate(crabs.begin(), crabs.end(), 0, [&](int s, auto& c) {
            int d = abs(c - x);
            return s + (d * (d + 1)) / 2;
            });
        part2 = min(part2, fuel2);
    }
    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}