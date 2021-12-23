#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <cassert>
#include "../shared/Point.h"

using namespace std;

int COST[] = { 1, 10, 100, 1000 };
//char STATE[] = "BACDBCDA";
char STATE[] = "BBACADDC";

// Part1
//#############
//#ab.c.d.e.fg#
//###h#j#l#n###
//  #i#k#m#o#
//  #########

// Part2
//#############
//#ab.c.d.e.fg#
//###h#l#p#t###
//  #i#m#q#u#
//  #j#n#r#v#
//  #k#o#s#w#
//  #########

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
    return 'A' + (v - 'h') / 2;
}

Vertex OtherGoal(Vertex v)
{
    if (v < 'h') return 0;
    return 'h' + ((v - 'h') ^ 1);
}

void MakeWorld()
{
    // From hallway
    world['a'] = {
        {'a', 'h', {'b'}, 3 },
        {'a', 'i', {'b', 'h'}, 4 },
        {'a', 'j', {'b', 'c'}, 5},
        {'a', 'k', {'b', 'c', 'j'}, 6 },
        {'a', 'l', {'b', 'c', 'd'}, 7},
        {'a', 'm', {'b', 'c', 'd', 'l'}, 8 },
        {'a', 'n', {'b', 'c', 'd', 'e'}, 9},
        {'a', 'o', {'b', 'c', 'd', 'e', 'n'}, 10}
    };
    world['b'] = {
        {'b', 'h', {}, 2},
        {'b', 'i', {'h'}, 3 },
        {'b', 'j', {'c'}, 4},
        {'b', 'k', {'c', 'j'}, 5},
        {'b', 'l', {'c', 'd'}, 6},
        {'b', 'm', {'c', 'd', 'l'}, 7},
        {'b', 'n', {'c', 'd', 'e'}, 8},
        {'b', 'o', {'c', 'd', 'e', 'n'}, 9}
    };
    world['c'] = {
        {'c', 'h', {}, 2},
        {'c', 'i', {'h'}, 3 },
        {'c', 'j', {}, 2},
        {'c', 'k', {'j'}, 3 },
        {'c', 'l', {'d'}, 4},
        {'c', 'm', {'d', 'l'}, 5},
        {'c', 'n', {'d', 'e'}, 6},
        {'c', 'o', {'d', 'e', 'n'}, 7}
    };
    world['d'] = {
        {'d', 'h', {'c'}, 4},
        {'d', 'i', {'c', 'h'}, 5 },
        {'d', 'j', {}, 2},
        {'d', 'k', {'j'}, 3 },
        {'d', 'l', {}, 2},
        {'d', 'm', {'l'}, 3},
        {'d', 'n', {'e'}, 4},
        {'d', 'o', {'e', 'n'}, 5}
    };
    world['e'] = {
        {'e', 'h', {'c', 'd'}, 6},
        {'e', 'i', {'c', 'd', 'h'}, 7 },
        {'e', 'j', {'d'}, 4},
        {'e', 'k', {'d', 'j'}, 5 },
        {'e', 'l', {}, 2},
        {'e', 'm', {'l'}, 3},
        {'e', 'n', {}, 2},
        {'e', 'o', {'n'}, 3}
    };
    world['f'] = {
        {'f', 'h', {'c', 'd', 'e'}, 8},
        {'f', 'i', {'c', 'd', 'e', 'h'}, 9 },
        {'f', 'j', {'d', 'e'}, 6},
        {'f', 'k', {'d', 'e', 'j'}, 7 },
        {'f', 'l', {'e'}, 4},
        {'f', 'm', {'e', 'l'}, 5},
        {'f', 'n', {}, 2},
        {'f', 'o', {'n'}, 3}
    };
    world['g'] = {
        {'g', 'h', {'c', 'd', 'e', 'f'}, 9},
        {'g', 'i', {'c', 'd', 'e', 'f', 'h'}, 10},
        {'g', 'j', {'d', 'e', 'f'}, 7},
        {'g', 'k', {'d', 'e', 'f', 'j'}, 8 },
        {'g', 'l', {'e', 'f'}, 5},
        {'g', 'm', {'e', 'f', 'l'}, 6},
        {'g', 'n', {'f'}, 3},
        {'g', 'o', {'f', 'n'}, 4}
    };

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
    for (char c = 'h'; c <= 'o'; ++c) {
        s.insert({ c, STATE[c - 'h'] });
    }
    return s;
}

State MakeGoal()
{
    State s;
    for (char c = 'h'; c <= 'o'; ++c) {
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
                    assert(OtherGoal(e.v2) != 0);
                    auto it = state.find(OtherGoal(e.v2));
                    if (it == state.end() || it->second == us) {
                        to.insert(e.v2);
                    }
                }
            }
        }
    }

    return to;
}

void Dump(const State& state)
{
    string s = 
        "#############\n"
        "#ab.c.d.e.fg#\n"
        "###h#j#l#n###\n"
        "  #i#k#m#o#  \n"
        "  #########  \n";
    for (auto& c : s) {
        if (isalpha(c)) {
            if (state.find(c) == state.end()) {
                c = '.';
            }
            else {
                c = state.find(c)->second;
            }
        }
    }
    cout << s;
    cout << endl;
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

        //cout << closed.size() << endl;
        //Dump(state);

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

    cout << "Part 1 Best Cost: " << bestcost << endl;
}