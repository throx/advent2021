#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include "../shared/Split.h"

using namespace std;

int main()
{
    // Ben had an awesome idea, so I'll steal it
    // Generate all possible solutions (should be only 5040), creating an index back
    // to the permutation.

    const vector<string> DIGITS = { "abcefg", "cf", "acdeg", "acdfg", "bcdf", "abdfg", "abdefg", "acf", "abcdefg", "abcdfg" };
    string perm = "abcdefg";
    map<set<string>, string> permmap;
    do {
        set<string> digits;
        for (auto& d : DIGITS) {
            string digit;
            for (char c : d) {
                digit.push_back(perm[c - 'a']);
            }
            sort(digit.begin(), digit.end());
            digits.insert(digit);
        }
        permmap[digits] = perm;
    } while (next_permutation(perm.begin(), perm.end()));

    // Now just read each line
    int part1 = 0;
    int part2 = 0;

    while (!cin.eof()) {
        string line;
        getline(cin, line);

        auto parts = Split(line, '|');
        auto samples = Split(parts[0], ' ');
        auto outputs = Split(parts[1], ' ');

        for (auto& s : samples) {
            sort(s.begin(), s.end());
        }
        set<string> samples_set(samples.begin(), samples.end());

        // Look up the solution in our pre-prepared index
        perm = permmap[samples_set];

        // Map the digits
        map<string, int> dmap;
        for (int i = 0; i < DIGITS.size(); ++i) {
            string digit;
            for (char c : DIGITS[i]) {
                digit.push_back(perm[c - 'a']);
            }
            sort(digit.begin(), digit.end());
            dmap[digit] = i;
        }

        int out = 0;
        for (auto& o : outputs) {
            sort(o.begin(), o.end());
            int d = dmap[o];
            out = out * 10 + d;

            switch (d) {
            case 1:
            case 4:
            case 7:
            case 8:
                ++part1;
            }
        }
        part2 += out;
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}