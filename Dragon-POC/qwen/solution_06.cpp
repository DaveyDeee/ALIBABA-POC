#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
};

// Function to check if point (x, y) is inside rectangle r
bool isPointInside(const Rect& r, long long x, long long y) {
    return (x >= r.x1 && x < r.x2 && y >= r.y1 && y < r.y2);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    long long n, D;
    cin >> n >> D;
    
    vector<Rect> rects(n);
    for (long long i = 0; i < n; ++i) {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }
    
    // Process in reverse order
    vector<Rect> coverage;
    long long visible = 0;
    
    for (long long i = n - 1; i >= 0; --i) {
        const Rect& r = rects[i];
        bool covered = false;
        
        // Check if all four corners are covered by existing coverage
        long long corners[4][2] = {
            {r.x1, r.y1},
            {r.x2, r.y1},
            {r.x1, r.y2},
            {r.x2, r.y2}
        };
        int coveredCorners = 0;
        for (int c = 0; c < 4; ++c) {
            long long x = corners[c][0];
            long long y = corners[c][1];
            bool found = false;
            for (const auto& cov : coverage) {
                if (isPointInside(cov, x, y)) {
                    found = true;
                    break;
                }
            }
            if (found) {
                coveredCorners++;
            }
        }
        // If at least one corner is not covered, then the rectangle might be visible
        if (coveredCorners < 4) {
            visible++;
            coverage.push_back(r);
        }
    }
    
    if (visible == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
    
    return 0;
}