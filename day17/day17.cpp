#include <iostream>
#include <string>
#include "../shared/Rect.h"

using namespace std;

//const Rect TARGET = Rect::Inclusive({20, -10}, {30, -5});
const Rect TARGET = Rect::Inclusive({ 244, -91 }, { 303, -54 });

int Hits(Point2 vel)
{
    Point2 pos;
    int maxy = 0;

    while (pos[1] > 0 || pos[1] >= TARGET.start()[1])
    {
        if (TARGET.Contains(pos)) {
            return maxy;
        }

        pos += vel;
        maxy = max(maxy, (int)pos[1]);
        if (vel[0] > 0) --vel[0];
        if (vel[0] < 0) ++vel[0];
        --vel[1];
    }

    return -999;
}

int main()
{
    // Really roughly velocity can range from X: 0 to TARGET.end, Y: one more than max of abs of start, end.
    // Any more than these max values and either the initial shot completely overshoots the range, or
    // it completely overshoots on the way "back down" (as it must pass through zero because symmetry).
    // This assumes the 'y' target is negative.
    int xmin = 0;
    int xmax = TARGET.end()[0];
    int ymax = max(abs(TARGET.start()[1]), abs(TARGET.end()[1]) + 1);
    int ymin = -ymax;

    Hits({ 6, 9 });
    int part1 = 0;
    int part2 = 0;
    for (int y = ymax; y >= ymin; --y) {
        for (int x = xmin; x <= xmax; ++x) {
            int maxy = Hits({ x, y });
            part1 = max(part1, maxy);
            if (maxy != -999) {
                ++part2;
            }
        }
    }
found:

    // RESULTS...

    cout << "Part 1: " << part1 << endl;
    cout << "Part 2: " << part2 << endl;
}