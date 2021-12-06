#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <numeric>

using namespace std;

int main()
{
    string s;
    getline(cin, s);

    vector<__int64> fish(10);
    istringstream iss(s);
    string n;
    while (getline(iss, n, ',')) {
        ++fish[stoi(n)];
    }

    __int64 part1 = 0;
    for (int day = 0; day < 256; ++day) {
        fish[9] += fish[0];
        fish[7] += fish[0];
        fish.push_back(0);
        fish.erase(fish.begin());

        if (day == 79) {
            part1 = accumulate(fish.begin(), fish.end(), 0i64);
        }
    }

    __int64 part2 = accumulate(fish.begin(), fish.end(), 0i64);

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}