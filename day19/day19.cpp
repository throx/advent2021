#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>
#include "../shared/Point.h"
#include "../shared/Split.h"

using namespace std;

typedef pair<int, int> IntPair;
typedef vector<Point3> Scanner;

// Transform a point by a tensor.  Probably more a matrix than
// a tensor.  Whatever.
typedef vector<Point3> Tensor;
Point3 XForm(const Point3& in, const Tensor& t)
{
    return Point3 ({
        in[0] * t[0][0] + in[1] * t[1][0] + in[2] * t[2][0],
        in[0] * t[0][1] + in[1] * t[1][1] + in[2] * t[2][1],
        in[0] * t[0][2] + in[1] * t[1][2] + in[2] * t[2][2]
    });
}

// Generate all the differences between points in a scan
typedef map<Point3, IntPair> Scanmapping;
Scanmapping GenerateScanMapping(const Scanner& scanner)
{
    Scanmapping sm;
    for (int i = 0; i < scanner.size(); ++i) {
        for (int j = 0; j < scanner.size(); ++j) {
            if (i == j) continue;
            sm[scanner[i] - scanner[j]] = { i, j };
        }
    }
    return sm;
}


int main()
{
    // Read in the data
    vector<Scanner> scanners;

    int id = -1;
    string s;
    Scanner scanner;
    while (getline(cin, s)) {
        if (s.empty()) continue;
        if (s[1] == '-') {
            if (id >= 0) {
                scanners.push_back(scanner);
                scanner.clear();
            }
            ++id;
        }
        else {
            auto v = SplitI64(s);
            scanner.push_back({ v[0], v[1], v[2] });
        }
    }
    scanners.push_back(scanner);

    // Create all possible xforms.  This includes reflections, which I can't be bothered excluding
    vector<Tensor> x1;
    x1.push_back({ {P3::X, P3::Y, P3::Z} });
    x1.push_back({ {-P3::X, P3::Y, P3::Z} });
    x1.push_back({ {P3::X, -P3::Y, P3::Z} });
    x1.push_back({ {P3::X, P3::Y, -P3::Z} });
    x1.push_back({ {-P3::X, -P3::Y, P3::Z} });
    x1.push_back({ {-P3::X, P3::Y, -P3::Z} });
    x1.push_back({ {P3::X, -P3::Y, -P3::Z} });
    x1.push_back({ {-P3::X, -P3::Y, -P3::Z} });

    vector<Tensor> xforms = x1;
    for (auto& xf : x1) {
        xforms.push_back({ xf[1], xf[2], xf[0] });
        xforms.push_back({ xf[2], xf[0], xf[1] });
        xforms.push_back({ xf[2], xf[1], xf[0] });
        xforms.push_back({ xf[1], xf[0], xf[2] });
        xforms.push_back({ xf[0], xf[2], xf[1] });
    }

    // Now, we don't know where things are, but we can get an idea using the differences
    // between points.  Basically, the difference between any two points will be a xform
    // of the difference between the same two points in another scan.

    // We'll create a master set of points that are populated with scan #0 to being with.
    // We erase scans from the list as they are added into master.
    Scanner master = scanners[0];
    scanners.erase(scanners.begin());

    // Somewhere to store scanner positions (ie coordinate translations)
    set<Point3> xlats;

    // Friggen awful goto to restart the matching whenever we get one more set added
    // to master.
restart:

    // Generate all the differences for master and throw them in a set
    set<Point3> scan1;
    auto sm1 = GenerateScanMapping(master);
    for (auto& k : sm1) {
        scan1.insert(k.first);
    }

    // Loop through the remaining scans
    for (auto scan_it = scanners.begin(); scan_it != scanners.end(); ++scan_it) {

        // Generate all the scan differences for this scan
        auto sm2 = GenerateScanMapping(*scan_it);

        // Loop through all possible transforms
        for (auto& xform : xforms) {

            // Throw the transformed differences into a set, and keep
            // the inverse mapping around so we can find the original
            // values again later
            set<Point3> scan2;
            map<Point3, Point3> reverse2;
            for (auto& k : sm2) {
                auto p = XForm(k.first, xform);
                scan2.insert(p);
                reverse2[p] = k.first;
            }

            // Do a set intersection between the master scan differences and
            // the transformed scan differences.  A match will have lots in common
            vector<Point3> intersection;
            set_intersection(scan1.begin(), scan1.end(), scan2.begin(), scan2.end(), back_inserter(intersection));

            // Now need to see the unique mappings because it's entirely possible
            // that points don't map uniquely (I found this a lot, which was annoying)
            map<int, set<int>> tally;
            for (auto& i : intersection) {
                auto r = reverse2[i];
                tally[sm1[i].first].insert(sm2[r].first);
                tally[sm1[i].second].insert(sm2[r].second);
            }

            // Only care about points with a unique mapping (ie the list of points they
            // map to has a size of one)
            set<IntPair> mapping;
            for (auto& k : tally) {
                if (k.second.size() == 1) {
                    mapping.insert({ k.first, *k.second.begin() });
                }
            }

            // If we get at least 12 unique points, we have a matching set of differences with
            // a matching transform
            if (mapping.size() >= 12) {
                cout << "Found a " << mapping.size() << " mapping" << endl;
                for (auto& i : mapping) {
                    cout << "  " << i.first << " = " << i.second << endl;
                }
                cout << scanners.size() - 1 << " left.";
                cout << endl;

                // Figure out the xlat from scan -> master using the first pair of points
                auto mit = mapping.begin();
                Point3 xlat = master[mit->first] - XForm((*scan_it)[mit->second], xform);
                ++mit;

                // Check the second set of points, just to make sure!
                assert(xlat == master[mit->first] - XForm((*scan_it)[mit->second], xform));

                // Keep this around for part 2
                xlats.insert(xlat);

                // Add the points to master (master being a vector makes this horribly inefficient)
                for (auto& p : *scan_it) {
                    auto xp = XForm(p, xform) + xlat;
                    if (find(master.begin(), master.end(), xp) == master.end()) {
                        master.push_back(xp);
                    }
                }

                // Done with this scanner, delete it.
                scanners.erase(scan_it);
                goto restart;           //Yuck
            }
        }
    }

    // Should be all done
    assert(scanners.empty());

    // Find largest scanner dist
    __int64 part2 = 0;
    for (auto x : xlats) {
        for (auto y : xlats) {
            part2 = max(part2, Dist(x, y));
        }
    }

    // OMFG done!
    cout << "Part 1: " << master.size() << endl;
    cout << "Part 2: " << part2 << endl;
}