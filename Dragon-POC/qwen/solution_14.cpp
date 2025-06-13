#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
};

// Check if point (x, y) is inside any of the processed rectangles
bool isCovered(long long x, long long y, const vector<Rect>& rects) {
    for (const auto& r : rects) {
        if (x >= r.x1 && x < r.x2 && y >= r.y1 && y < r.y2) {
            return true;
        }
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    int n, D;
    cin >> n >> D;
    
    vector<Rect> rects(n);
    for (int i = 0; i < n; ++i) {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }
    
    vector<Rect> active;
    int count = 0;
    
    // Process in reverse order
    for (int i = n - 1; i >= 0; --i) {
        const Rect& r = rects[i];
        // Check the top-right corner
        long long x = r.x2 - 1; // Inside the rectangle
        long long y = r.y2 - 1;
        
        if (!isCovered(x, y, active)) {
            count++;
            active.push_back(r);
        }
    }
    
    if (count == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
    
    return 0;
}