#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

typedef pair<char, char> CP;

__int64 DoPart(const string& chain, map<CP, char>& m, int iters)
{
    map<CP, __int64> pairs;
    for (int i = 0; i < chain.length() - 1; ++i) {
        pairs[{chain[i], chain[i + 1]}]++;
    }

    for (int step = 0; step < iters; ++step) {
        map<CP, __int64> newpairs;
        for (auto x : pairs) {
            newpairs[{x.first.first, m[x.first]}] += x.second;
            newpairs[{m[x.first], x.first.second}] += x.second;
        }
        swap(pairs, newpairs);
    }

    map<char, __int64> count;
    for (auto& x : pairs) {
        count[x.first.first] += x.second;
        count[x.first.second] += x.second;
    }
    count[chain[0]]++;
    count[chain[chain.length() - 1]]++;

    for (auto& x : count) {
        x.second /= 2;
    }

    auto mm = minmax_element(count.begin(), count.end(), [&](auto& x, auto& y) {return x.second < y.second; });

    return mm.second->second - mm.first->second;
}

int main()
{
    string chain;
    getline(cin, chain);
    
    string s;
    getline(cin, s);

    map<CP, char> m;
    while (getline(cin, s)) {
        m[{s[0], s[1]}] = s[6];
    }

    cout << "Part 1: " << DoPart(chain, m, 10) << endl;
    cout << "Part 2: " << DoPart(chain, m, 40) << endl;
}