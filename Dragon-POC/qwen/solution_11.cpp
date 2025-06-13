#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    Rect() {}
    Rect(long long a, long long b, long long c, long long d) : x1(a), y1(b), x2(c), y2(d) {
        if (x1 > x2) swap(x1, x2);
        if (y1 > y2) swap(y1, y2);
    }
    long long area() const {
        return (x2 - x1) * (y2 - y1);
    }
};

// Subtract rect b from rect a, returns list of rects that are part of a but not in b
vector<Rect> subtract_rect(const Rect& a, const Rect& b) {
    vector<Rect> res;
    // No overlap
    if (a.x2 <= b.x1 || a.x1 >= b.x2 || a.y2 <= b.y1 || a.y1 >= b.y2) {
        res.push_back(a);
        return res;
    }
    // Left part
    if (a.x1 < b.x1)
        res.emplace_back(a.x1, a.y1, b.x1, a.y2);
    // Right part
    if (a.x2 > b.x2)
        res.emplace_back(b.x2, a.y1, a.x2, a.y2);
    // Bottom part (remaining between b.x1 and b.x2, below b.y1)
    if (a.y1 < b.y1)
        res.emplace_back(max(a.x1, b.x1), a.y1, min(a.x2, b.x2), b.y1);
    // Top part (remaining between b.x1 and b.x2, above b.y2)
    if (a.y2 > b.y2)
        res.emplace_back(max(a.x1, b.x1), b.y2, min(a.x2, b.x2), a.y2);
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    long long D;
    cin >> n >> D;
    vector<Rect> rects(n);
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rects[i] = Rect(x1, y1, x2, y2);
    }
    
    vector<Rect> covered;
    int count = 0;
    // Process in reverse order
    for (int i = n - 1; i >= 0; --i) {
        Rect r = rects[i];
        if (r.area() == 0) {
            continue;
        }
        // Current candidate rect r
        vector<Rect> current;
        current.push_back(r);
        // Subtract all covered rects
        for (const auto& cov : covered) {
            vector<Rect> next;
            for (const auto& cur : current) {
                vector<Rect> parts = subtract_rect(cur, cov);
                next.insert(next.end(), parts.begin(), parts.end());
            }
            current = next;
            if (current.empty()) break;
        }
        if (!current.empty()) {
            ++count;
            // Merge current into covered
            covered.insert(covered.end(), current.begin(), current.end());
        }
    }
    if (count == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }
}