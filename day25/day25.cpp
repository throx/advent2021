#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main()
{
    vector<string> grid;

    string s;
    while (getline(cin, s)) {
        grid.push_back(s);
    }

    int turn = 0;
    int sx = grid[0].length();
    int sy = grid.size();
    bool moved = true;
    while (moved) {
        moved = false;
        vector<string> next = grid;

        // Move E
        for (int y = 0; y < sy; ++y) {
            for (int x = 0; x < sx; ++x) {
                if (grid[y][x] == '>') {
                    int nx = (x + 1) % sx;
                    if (grid[y][nx] == '.') {
                        next[y][x] = '.';
                        next[y][nx] = '>';
                        moved = true;
                    }
                }
            }
        }

        grid = next;

        // Move S
        for (int y = 0; y < sy; ++y) {
            int ny = (y + 1) % sy;
            for (int x = 0; x < sx; ++x) {
                if (grid[y][x] == 'v') {
                    if (grid[ny][x] == '.') {
                        next[y][x] = '.';
                        next[ny][x] = 'v';
                        moved = true;
                    }
                }
            }
        }

        swap(grid, next);
        ++turn;

        //cout << "Turn: " << turn << endl;
        //for (auto s : grid) cout << s << endl;
        //cout << endl;

    }

    // RESULTS...

    cout << "Part 1: " << turn << endl;
}