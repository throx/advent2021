#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include "../shared/Point.h"

using namespace std;

int COST[] = { 1, 10, 100, 1000 };
//char STATE[] = "BDDACCBDBBACDACA";
char STATE[] = "BDDBACBCABADDACC";

// Part2
//#############
//#ab.c.d.e.fg#
//###h#l#p#t###
//  #i#m#q#u#
//  #j#n#r#v#
//  #k#o#s#w#
//  #########

// Insert:
// #D#C#B#A#
// #D#B#A#C#


typedef char Vertex;
struct Edge {
    Vertex v1;
    Vertex v2;
    set<Vertex> blocks;
    int dist;
};
typedef vector<Edge> Edges;
typedef map<Vertex, Edges> World;
typedef char Amphi;
typedef map<Vertex, Amphi> State;
typedef map<pair<Vertex, Vertex>, int> Dists;

static World world;
static Dists dists;

Amphi Goal(Vertex v)
{
    if (v < 'h') return 0;
    return 'A' + (v - 'h') / 4;
}

vector<Vertex> OtherGoal(Vertex v)
{
    if (v < 'h') return {};
    vector<Vertex> ret;
    ret.push_back('h' + ((v - 'h') ^ 1));
    ret.push_back('h' + ((v - 'h') ^ 2));
    ret.push_back('h' + ((v - 'h') ^ 3));
    return ret;
}

void MakeWorld()
{
    // From hallway to first spots
    world['a'] = {
        {'a', 'h', {'b'}, 3 },
        {'a', 'l', {'b', 'c'}, 5},
        {'a', 'p', {'b', 'c', 'd'}, 7},
        {'a', 't', {'b', 'c', 'd', 'e'}, 9},
    };
    world['b'] = {
        {'b', 'h', {}, 2},
        {'b', 'l', {'c'}, 4},
        {'b', 'p', {'c', 'd'}, 6},
        {'b', 't', {'c', 'd', 'e'}, 8},
    };
    world['c'] = {
        {'c', 'h', {}, 2},
        {'c', 'l', {}, 2},
        {'c', 'p', {'d'}, 4},
        {'c', 't', {'d', 'e'}, 6},
    };
    world['d'] = {
        {'d', 'h', {'c'}, 4},
        {'d', 'l', {}, 2},
        {'d', 'p', {}, 2},
        {'d', 't', {'e'}, 4},
    };
    world['e'] = {
        {'e', 'h', {'c', 'd'}, 6},
        {'e', 'l', {'d'}, 4},
        {'e', 'p', {}, 2},
        {'e', 't', {}, 2},
    };
    world['f'] = {
        {'f', 'h', {'c', 'd', 'e'}, 8},
        {'f', 'l', {'d', 'e'}, 6},
        {'f', 'p', {'e'}, 4},
        {'f', 't', {}, 2},
    };
    world['g'] = {
        {'g', 'h', {'c', 'd', 'e', 'f'}, 9},
        {'g', 'l', {'d', 'e', 'f'}, 7},
        {'g', 'p', {'e', 'f'}, 5},
        {'g', 't', {'f'}, 3},
    };

    // From hall to other spots
    for (char c = 'a'; c != 'h'; ++c) {
        Edges es = world[c];
        for (Edge e : es) {
            for (int i = 0; i < 3; ++i) {
                e.blocks.insert(e.v2);
                ++e.v2;
                ++e.dist;
                world[c].push_back(e);
            }
        }
    }

    // To hallway
    for (char c = 'a'; c != 'h'; ++c) {
        for (auto& e : world[c]) {
            world[e.v2].push_back({ e.v2, e.v1, e.blocks, e.dist });
        }
    }

    // Construct dists
    for (auto& kv : world) {
        for (auto& e : kv.second) {
            dists[{e.v1, e.v2}] = e.dist;
        }
    }
}

State MakeState()
{
    State s;
    for (char c = 'h'; c <= 'w'; ++c) {
        s.insert({ c, STATE[c - 'h'] });
    }
    return s;
}

State MakeGoal()
{
    State s;
    for (char c = 'h'; c <= 'w'; ++c) {
        s.insert({ c, Goal(c) });
    }
    return s;
}

set<Vertex> GetOpen(const State& state, const Vertex& from)
{
    set<Vertex> to;
    assert(state.find(from) != state.end());
    Amphi us = state.find(from)->second;

    for (auto& e : world[from]) {

        // Dest is clear
        if (state.find(e.v2) == state.end()) {

            // Nothing in the way
            bool blocked = false;
            for (auto& b : e.blocks) {
                if (state.find(b) != state.end()) {
                    blocked = true;
                    break;
                }
            }

            if (!blocked) {

                // Can only move to our own goal, or hallway
                if (Goal(e.v2) == 0) {
                    to.insert(e.v2);
                }
                else if (Goal(e.v2) == us) {
                    // Can only move to our own goal if it's empty or
                    // occupied by another of us

                    // TODO: Optimisation? Only makes sense to move to bottom of goal?

                    assert(OtherGoal(e.v2).size() > 0);
                    bool goalok = true;
                    for (auto& og : OtherGoal(e.v2)) {
                        auto it = state.find(og);
                        if (it != state.end() && it->second != us) {
                            goalok = false;
                            break;
                        }
                    }
                    if (goalok) {
                        to.insert(e.v2);
                    }
                }
            }
        }
    }

    return to;
}

int main()
{
    MakeWorld();
    State goal = MakeGoal();

    multimap<int, State> open;
    set<State> closed;

    open.insert({ 0, MakeState() });
    int bestcost = 0;

    while (!open.empty()) {
        int cost = open.begin()->first;
        State state = open.begin()->second;
        open.erase(open.begin());

        if (state == goal) {
            bestcost = cost;
            break;
        }

        if (closed.find(state) != closed.end()) {
            continue;
        }

        closed.insert(state);

        if (closed.size() % 10000 == 0) cout << closed.size() << ": " << cost << endl;

        for (auto& kv : state) {
            auto tos = GetOpen(state, kv.first);
            for (auto& to : tos) {
                State newstate = state;
                newstate.erase(kv.first);
                newstate.insert({ to, kv.second });

                int newcost = cost + dists[{kv.first, to}] * COST[kv.second - 'A'];
                assert(newcost > cost);
                open.insert({ newcost, newstate });
            }
        }
    }

    cout << "Part 2 Best Cost: " << bestcost << endl;
}