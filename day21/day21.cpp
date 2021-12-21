// This is just part 2, because part 1 is trivial and I did it in Excel at work.

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <cassert>

using namespace std;

const int PLAYER1START = 5;
const int PLAYER2START = 10;
const int WIN = 21;

// State of the game at any time.
struct State {
    int score[2];
    int pos[2];
    int turn;

    // Order first by sum of scores, so first one we pull from "todo" list is
    // one that can't be returned to.
    bool operator< (const State& other) const {
        if (score[0] + score[1] < other.score[0] + other.score[1]) return true;
        if (score[0] + score[1] > other.score[0] + other.score[1]) return false;
        if (score[0] < other.score[0]) return true;
        if (score[0] > other.score[0]) return false;
        if (pos[0] < other.pos[0]) return true;
        if (pos[0] > other.pos[0]) return false;
        if (pos[1] < other.pos[1]) return true;
        if (pos[1] > other.pos[1]) return false;
        return turn < other.turn;
    }
};

typedef map<State, __int64> StateMap;

const __int64 PERMS[] = { 0, 0, 0, 1, 3, 6, 7, 6, 3, 1 };

int main()
{
    StateMap states;

    State init = {
        {0, 0},
        {PLAYER1START, PLAYER2START},
        0
    };

    states[init] = 1;

    // Treat as breadth first search, with the map sorted
    // in an order that means the starting states are never
    // revisited by later states.
    while (true) {

        // Find first non willing state
        auto it = states.begin();
        while (it->first.score[0] >= WIN || it->first.score[1] >= WIN) {
            ++it;
            if (it == states.end()) {
                goto done;  // I'm starting to use these way too much
            }
        }

        State state = it->first;
        __int64 perms = it->second;
        states.erase(it);       // <--- Had a stupid bug here erasing begin()

        // Check...
        assert(state.score[0] < WIN);
        assert(state.score[1] < WIN);

        // Loop through possible dice results
        for (int dice = 3; dice <= 9; ++dice) {

            // Play the turn
            State next = state;
            int t = next.turn;
            next.pos[t] += dice;
            if (next.pos[t] > 10) {
                next.pos[t] -= 10;
            }
            assert(next.pos[t] <= 10);
            assert(next.pos[t] > 0);
            next.score[t] += next.pos[t];

            next.turn = 1 - next.turn;

            // Store possible ways to get to this next state
            states[next] += (perms * PERMS[dice]);
        }
    }

done:
    // Count the wins
    __int64 wins[2] = { 0, 0 };
    for (auto& kv : states) {
        assert(kv.first.score[0] != kv.first.score[1]);
        if (kv.first.score[0] > kv.first.score[1]) {
            wins[0] += kv.second;
            assert(kv.first.turn == 1);
            assert(kv.first.score[0] >= WIN);
            assert(kv.first.score[1] < WIN);
        }
        else {
            wins[1] += kv.second;
            assert(kv.first.turn == 0);
            assert(kv.first.score[1] >= WIN);
            assert(kv.first.score[0] < WIN);
        }
    }

    cout << "Player 1 wins: " << wins[0] << endl;
    cout << "Player 2 wins: " << wins[1] << endl;
}