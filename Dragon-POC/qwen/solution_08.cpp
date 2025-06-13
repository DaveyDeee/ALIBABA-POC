#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    bool contains(long long x, long long y) const {
        return x >= x1 && x < x2 && y >= y1 && y < y2;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long n, D;
    cin >> n >> D;

    vector<Rect> rects(n);
    for (long long i = 0; i < n; ++i) {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }

    vector<Rect> S;
    long long visible = 0;

    for (long long i = n - 1; i >= 0; --i) {
        const Rect& r = rects[i];
        bool found = false;

        // Check four corners and center
        // Point 1: (x1, y1)
        {
            long long x = r.x1, y = r.y1;
            bool covered = false;
            for (const auto& s : S) {
                if (s.contains(x, y)) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                found = true;
            }
        }
        if (!found) {
            // Point 2: (x1, y2)
            long long x = r.x1, y = r.y2;
            bool covered = false;
            for (const auto& s : S) {
                if (s.contains(x, y)) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                found = true;
            }
        }
        if (!found) {
            // Point 3: (x2, y1)
            long long x = r.x2, y = r.y1;
            bool covered = false;
            for (const auto& s : S) {
                if (s.contains(x, y)) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                found = true;
            }
        }
        if (!found) {
            // Point 4: (x2, y2)
            long long x = r.x2, y = r.y2;
            bool covered = false;
            for (const auto& s : S) {
                if (s.contains(x, y)) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                found = true;
            }
        }
        if (!found) {
            // Center point
            long long x = (r.x1 + r.x2) / 2;
            long long y = (r.y1 + r.y2) / 2;
            bool covered = false;
            for (const auto& s : S) {
                if (s.contains(x, y)) {
                    covered = true;
                    break;
                }
            }
            if (!covered) {
                found = true;
            }
        }

        if (found) {
            visible++;
        }

        // Add current rect to S
        S.push_back(r);
    }

    if (visible == D) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }

    return 0;
}