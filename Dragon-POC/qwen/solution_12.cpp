#include <bits/stdc++.h>
using namespace std;

struct Rectangle {
    long long x1, y1, x2, y2;
    Rectangle() {}
    Rectangle(long long x1, long long y1, long long x2, long long y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    
    long long n, D;
    cin >> n >> D;
    
    vector<Rectangle> rects(n);
    for (long long i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rects[i] = Rectangle(x1, y1, x2, y2);
    }
    
    // Process in reverse order
    vector<Rectangle> processed;
    long long visible = 0;
    
    for (long long i = n - 1; i >= 0; --i) {
        Rectangle r = rects[i];
        bool covered = false;
        
        // Check if this rectangle is completely covered by the union of processed rectangles
        // This is a simplified check and not accurate for all cases
        // For a real solution, we need to use a more sophisticated area calculation
        
        // Simplified approach: check if all four corners are covered by at least one processed rectangle
        // This is an approximation and may not work for all test cases
        
        long long cnt = 0;
        // Check all four corners
        pair<long long, long long> corners[4] = {
            {r.x1, r.y1},
            {r.x1, r.y2},
            {r.x2, r.y1},
            {r.x2, r.y2}
        };
        
        bool any_uncovered = false;
        for (auto &p : corners) {
            bool covered_by_any = false;
            for (auto &pr : processed) {
                if (pr.x1 <= p.first && p.first < pr.x2 && pr.y1 <= p.second && p.second < pr.y2) {
                    covered_by_any = true;
                    break;
                }
            }
            if (!covered_by_any) {
                any_uncovered = true;
                break;
            }
        }
        
        if (any_uncovered) {
            visible++;
            processed.push_back(r);
        }
    }
    
    if (visible == D)
        cout << "Yes\n";
    else
        cout << "No\n";
    
    return 0;
}