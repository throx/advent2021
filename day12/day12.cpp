#include <iostream>
#include <string>
#include "../shared/Split.h"
#include <map>
#include <set>
#include <vector>

using namespace std;

typedef map<string, vector<string>> Edges;
Edges edges;

int NumPaths(string start, set<string> done, bool used1)
{
    int count = 0;
    if (islower(start[0])) {
        done.insert(start);
    }
    for (auto& x : edges[start]) {
        if (x == "end") {
            ++count;
        }
        else if (done.find(x) == done.end()) {
            count += NumPaths(x, done, used1);
        }
        else if (x != "start" && !used1) {
            count += NumPaths(x, done, true);
        }
    }

    return count;
}

int main()
{
    while (!cin.eof()) {
        string s;
        cin >> s;

        auto v = Split(s, '-');
        edges[v[0]].push_back(v[1]);
        edges[v[1]].push_back(v[0]);
    }

    set<string> done;
    int part1 = NumPaths("start", done, true);
    int part2 = NumPaths("start", done, false);

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}