#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    Rect() {}
    Rect(long long a, long long b, long long c, long long d) : x1(a), y1(b), x2(c), y2(d) {}
};

// Check if point (x, y) is inside rectangle r
bool isInside(const Rect &r, long long x, long long y) {
    return (x >= r.x1 && x < r.x2 && y >= r.y1 && y < r.y2);
}

// Check if rectangle r is fully covered by the coverage set
bool isCovered(const Rect &r, const vector<Rect> &coverage) {
    // Check the four corners and the center point of r
    long long x1 = r.x1, y1 = r.y1, x2 = r.x2, y2 = r.y2;
    long long cx = (x1 + x2) / 2;
    long long cy = (y1 + y2) / 2;
    bool covered[5] = {false}; // 0: (x1, y1), 1: (x2, y1), 2: (x1, y2), 3: (x2, y2), 4: center

    for (const auto &s : coverage) {
        if (isInside(s, x1, y1)) covered[0] = true;
        if (isInside(s, x2, y1)) covered[1] = true;
        if (isInside(s, x1, y2)) covered[2] = true;
        if (isInside(s, x2, y2)) covered[3] = true;
        if (isInside(s, cx, cy)) covered[4] = true;
    }

    // If all points are covered, then the rectangle is considered covered
    return covered[0] && covered[1] && covered[2] && covered[3] && covered[4];
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long n, D;
    cin >> n >> D;

    vector<Rect> rects(n);
    for (long long i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rects[i] = Rect(x1, y1, x2, y2);
    }

    vector<Rect> coverage;
    long long visible_count = 0;

    // Process in reverse order
    for (long long i = n - 1; i >= 0; --i) {
        const Rect &r = rects[i];
        if (!isCovered(r, coverage)) {
            visible_count++;
            coverage.push_back(r);
        }
    }

    if (visible_count == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }

    return 0;
}