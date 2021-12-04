#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

const int OK = -1;
typedef vector<int> Row;
typedef vector<Row> Board;

bool Mark(Board& b, int n) {
    for (auto& row : b) {
        for (auto& num : row) {
            if (num == n) {
                num = OK;
            }
        }
    }

    // Check winner
    for (int i = 0; i < 5; ++i) {
        // Check row
        if (b[i][0] == OK &&
            b[i][1] == OK &&
            b[i][2] == OK &&
            b[i][3] == OK &&
            b[i][4] == OK) {
            return true;
        }
        // Check col
        if (b[0][i] == OK &&
            b[1][i] == OK &&
            b[2][i] == OK &&
            b[3][i] == OK &&
            b[4][i] == OK) {
            return true;
        }
    }

    return false;
}

int Sum(Board& b) {
    int sum = 0;
    for (auto& row : b) {
        for (auto& num : row) {
            if (num != OK) {
                sum += num;
            }
        }
    }

    return sum;
}

int main()
{
    string s;
    getline(cin, s);

    vector<int> nums;
    istringstream iss(s);
    string n;
    while (getline(iss, n, ',')) {
        nums.push_back(stoi(n));
    }

    vector<Board> boards;
    vector<Board> boards2 = boards;

    while (!cin.eof()) {
        Board b;
        for (int i = 0; i < 5; ++i) {
            Row r;
            for (int j = 0; j < 5; ++j) {
                int x;
                cin >> x;
                r.push_back(x);
            }
            b.push_back(r);
        }
        boards.push_back(b);
    }

    int part1 = -1;
    int part2 = -1;
    for (auto& num : nums) {
        vector<Board> liveboards;
        for (Board& b : boards) {
            if (Mark(b, num)) {
                if (part1 == -1) {
                    part1 = Sum(b);
                    part1 *= num;
                }
            }
            else {
                liveboards.push_back(b);
            }
        }

        if (liveboards.empty()) {
            part2 = Sum(boards[0]);
            part2 *= num;
            break;
        }

        swap(boards, liveboards);
    }

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}