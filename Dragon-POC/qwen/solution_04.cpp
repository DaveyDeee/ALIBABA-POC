#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    long long n, D;
    cin >> n >> D;

    vector<Rect> rects(n);
    for (long long i = 0; i < n; ++i) {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }

    // Process in reverse order
    reverse(rects.begin(), rects.end());

    vector<Rect> coverage;
    long long visible_count = 0;

    long long min_x = 0, max_x = 0, min_y = 0, max_y = 0;
    bool coverage_empty = true;

    for (const auto& r : rects) {
        long long x1 = r.x1, y1 = r.y1, x2 = r.x2, y2 = r.y2;

        // Coordinates of the four corners
        long long corners[4][2] = {
            {x1, y1},
            {x2, y1},
            {x1, y2},
            {x2, y2}
        };

        bool visible = false;
        for (int i = 0; i < 4; ++i) {
            long long px = corners[i][0];
            long long py = corners[i][1];

            bool covered = false;

            if (coverage_empty) {
                covered = false;
            } else {
                // Bounding box check
                if (px < min_x || px > max_x || py < min_y || py > max_y) {
                    covered = false;
                } else {
                    // Check all existing coverage rectangles
                    for (const auto& cov_r : coverage) {
                        if (px >= cov_r.x1 && px <= cov_r.x2 &&
                            py >= cov_r.y1 && py <= cov_r.y2) {
                            covered = true;
                            break;
                        }
                    }
                }
            }

            if (!covered) {
                visible = true;
                break;
            }
        }

        if (visible) {
            visible_count++;
        }

        // Update coverage's bounding box
        if (coverage_empty) {
            min_x = x1;
            max_x = x2;
            min_y = y1;
            max_y = y2;
            coverage_empty = false;
        } else {
            if (x1 < min_x) min_x = x1;
            if (x2 > max_x) max_x = x2;
            if (y1 < min_y) min_y = y1;
            if (y2 > max_y) max_y = y2;
        }

        coverage.push_back(r);
    }

    if (visible_count == D) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }

    return 0;
}