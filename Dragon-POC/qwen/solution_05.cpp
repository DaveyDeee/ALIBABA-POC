#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    Rect() {}
    Rect(long long a, long long b, long long c, long long d) : x1(a), y1(b), x2(c), y2(d) {}
};

// Function to check if point (x, y) is covered by any of the rectangles in rects
bool isCoveredByAny(long long x, long long y, const vector<Rect>& rects) {
    for (const auto& r : rects) {
        if (x >= r.x1 && x < r.x2 && y >= r.y1 && y < r.y2) {
            return true;
        }
    }
    return false;
}

// Check if rect a is completely covered by the union of rects in rects
bool isCovered(const Rect& a, const vector<Rect>& rects) {
    // Check some key points of a
    long long x1 = a.x1, y1 = a.y1;
    long long x2 = a.x2, y2 = a.y2;
    long long cx = (x1 + x2) / 2;
    long long cy = (y1 + y2) / 2;

    // Check all four corners and the center
    if (!isCoveredByAny(x1, y1, rects)) return false;
    if (!isCoveredByAny(x1, y2, rects)) return false;
    if (!isCoveredByAny(x2, y1, rects)) return false;
    if (!isCoveredByAny(x2, y2, rects)) return false;
    if (!isCoveredByAny(cx, cy, rects)) return false;

    // If all points are covered, assume the whole rect is covered (not always true)
    return true;
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
    
    vector<Rect> covered;
    long long visible = 0;
    
    for (long long i = n - 1; i >= 0; --i) {
        const Rect& r = rects[i];
        if (!isCovered(r, covered)) {
            visible++;
            covered.push_back(r);
        }
    }
    
    if (visible == D) {
        cout << "Yes" << endl;
    } else {
        cout << "No" << endl;
    }
    
    return 0;
}