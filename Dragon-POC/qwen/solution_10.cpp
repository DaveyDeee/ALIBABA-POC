#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    Rect() {}
    Rect(long long a, long long b, long long c, long long d) {
        x1 = a; y1 = b; x2 = c; y2 = d;
    }
    bool valid() {
        return x1 < x2 && y1 < y2;
    }
};

// Subtract src from r, returns the list of resulting rects
vector<Rect> subtract(Rect r, Rect src) {
    vector<Rect> res;
    // No overlap
    if (r.x2 <= src.x1 || r.x1 >= src.x2 || r.y2 <= src.y1 || r.y1 >= src.y2) {
        res.push_back(r);
        return res;
    }
    
    // Main rectangle is split into up to 4 parts
    // Left
    if (r.x1 < src.x1) {
        res.emplace_back(r.x1, r.y1, src.x1, r.y2);
    }
    // Right
    if (r.x2 > src.x2) {
        res.emplace_back(src.x2, r.y1, r.x2, r.y2);
    }
    // Bottom (between left and right)
    if (r.y1 < src.y1) {
        res.emplace_back(max(r.x1, src.x1), r.y1, min(r.x2, src.x2), src.y1);
    }
    // Top (between left and right)
    if (r.y2 > src.y2) {
        res.emplace_back(max(r.x1, src.x1), src.y2, min(r.x2, src.x2), r.y2);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    long long n, D;
    cin >> n >> D;
    
    vector<Rect> rects(n);
    for (long long i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rects[i] = Rect(x1, y1, x2, y2);
    }
    
    vector<Rect> S;
    long long visible = 0;
    
    for (long long i = n - 1; i >= 0; --i) {
        Rect r = rects[i];
        vector<Rect> current;
        current.push_back(r);
        
        for (const auto& s_rect : S) {
            vector<Rect> new_current;
            for (const auto& rc : current) {
                vector<Rect> parts = subtract(rc, s_rect);
                for (const auto& p : parts) {
                    if (p.valid()) {
                        new_current.push_back(p);
                    }
                }
            }
            current = new_current;
            if (current.empty()) break;
        }
        
        if (!current.empty()) {
            ++visible;
            S.push_back(r);
        }
    }
    
    if (visible == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
    
    return 0;
}