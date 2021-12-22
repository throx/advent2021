#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <numeric>
#include <cassert>
#include "../shared/Point.h"
#include "../shared/Rect.h"

using namespace std;

// on x=-3..43,y=-28..22,z=-6..38
const regex INPUT_R("(on|off) x=(-?\\d+)..(-?\\d+),y=(-?\\d+)..(-?\\d+),z=(-?\\d+)..(-?\\d+)");

const Rect3 P1BOUND({ -50, -50, -50 }, { 51, 51, 51 });

set<Rect3> ExtractIntersection(set<Rect3>& rects, const Rect3& r)
{
    set<Rect3> intset;
    int cont = 0;
    auto it = rects.begin();
    while (it != rects.end()) {
        if (it->Intersects(r)) {
            if (!r.Contains(*it)) {
                intset.insert(*it);
            }
            else {
                ++cont;
            }
            it = rects.erase(it);
        }
        else {
            ++it;
        }
    }

    cout << "Extract " << intset.size() << endl;
    cout << "Elim " << cont << endl;
    return intset;
}

set<Rect3> Partition(const set<Rect3>& intset, const Rect3& r)
{
    if (intset.empty()) {
        return {};
    }

    // Get points of interest
    set<__int64> p0, p1, p2;
    p0.insert(r.start()[0]);
    p0.insert(r.end()[0]);
    p1.insert(r.start()[1]);
    p1.insert(r.end()[1]);
    p2.insert(r.start()[2]);
    p2.insert(r.end()[2]);

    for (auto& r1 : intset) {
        p0.insert(r1.start()[0]);
        p0.insert(r1.end()[0]);
        p1.insert(r1.start()[1]);
        p1.insert(r1.end()[1]);
        p2.insert(r1.start()[2]);
        p2.insert(r1.end()[2]);
    }

    vector<__int64> v0(p0.begin(), p0.end());
    vector<__int64> v1(p1.begin(), p1.end());
    vector<__int64> v2(p2.begin(), p2.end());

    set<Rect3> bound;
    for (int i0 = 0; i0 < v0.size() - 1; ++i0) {
        for (int i1 = 0; i1 < v1.size() - 1; ++i1) {
            for (int i2 = 0; i2 < v2.size() - 1; ++i2) {
                Rect3 rr({ v0[i0], v1[i1], v2[i2] }, { v0[i0 + 1], v1[i1 + 1], v2[i2 + 1] });
                assert(rr.Size() > 0);
                bound.insert(rr);
            }
        }
    }

    cout << "Partition to " << bound.size() << endl;
    return bound;
}

// This is really slow.  Runs way faster when I don't call it
void Coalesce(set<Rect3>& rects)
{
    int x = rects.size();
    cout << "Coalescing " << x << " rects by ";

    for (auto it1 = rects.begin(); it1 != rects.end(); ++it1) {
        auto it2 = it1;
        ++it2;
        while (it2 != rects.end()) {
            if (BoundSize(*it1, *it2) == it1->Size() + it2->Size()) {
                Rect3 r = Bound(*it1, *it2);
                rects.erase(it1);
                rects.erase(it2);
                rects.insert(r);
                it1 = rects.begin();
                break;
            }

            ++it2;
        }

        if (it1 == rects.end()) {
            break;
        }
    }
    cout << x - rects.size() << " to " << rects.size() << "." << endl;
}


void Add(set<Rect3>& rects, const Rect3& r)
{
    auto intset = ExtractIntersection(rects, r);
    if (intset.empty()) {
        rects.insert(r);
        return;
    }

    auto bound = Partition(intset, r);

    for (auto& bb : bound) {
        if (bb.Intersects(r)) {
            rects.insert(bb);
            continue;
        }
        for (auto& r1 : intset) {
            if (bb.Intersects(r1)) {
                rects.insert(bb);
                break;
            }
        }
    }

    //Coalesce(rects);
}

void Subtract(set<Rect3>& rects, const Rect3& r)
{
    auto intset = ExtractIntersection(rects, r);
    if (intset.empty()) {
        return;
    }

    auto bound = Partition(intset, r);

    for (auto& bb : bound) {
        if (bb.Intersects(r)) {
            continue;
        }
        for (auto& r1 : intset) {
            if (bb.Intersects(r1)) {
                rects.insert(bb);
                break;
            }
        }
    }

    //Coalesce(rects);
}

void Dump(const set<Rect3>& rects)
{
    for (int z = 9; z < 14; ++z) {
        for (int y = 9; y < 14; ++y) {
            for (int x = 9; x < 14; ++x) {
                if (any_of(rects.begin(), rects.end(), [&](auto& r) { return r.Contains({ x,y,z }); })) {
                    cout << '#';
                }
                else {
                    cout << '.';
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    for (auto& r : rects) {
        cout << r << endl;
    }
}

int main()
{
    set<Rect3> p1rects;
    set<Rect3> p2rects;

    int line = 1;
    string s;
    while (getline(cin, s)) {
        cout << "Processing: " << line << " - " << s << endl;
        ++line;

        smatch sm;
        if (regex_match(s.cbegin(), s.cend(), sm, INPUT_R)) {
            bool on = sm[1].str().length() < 3;
            __int64 x1 = stoll(sm[2]);
            __int64 x2 = stoll(sm[3]);
            __int64 y1 = stoll(sm[4]);
            __int64 y2 = stoll(sm[5]);
            __int64 z1 = stoll(sm[6]);
            __int64 z2 = stoll(sm[7]);

            Rect3 r = Rect3::Inclusive({ x1, y1, z1 }, { x2, y2, z2 });
            if (on) {
                Add(p2rects, r);
                if (P1BOUND.Contains(r)) {
                    Add(p1rects, r);
                }
            }
            else {
                Subtract(p2rects, r);
                if (P1BOUND.Contains(r)) {
                    Subtract(p1rects, r);
                }
            }
        }
    }

    //Dump(p2rects);

    __int64 part1 = 0;
    for (auto& r : p1rects) {
        part1 += r.Size();
    }

    __int64 part2 = 0;
    for (auto& r : p2rects) {
        part2 += r.Size();
    }

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}