#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    Rect() {}
    Rect(long long a, long long b, long long c, long long d) : x1(a), y1(b), x2(c), y2(d) {}
};

bool intersect(const Rect &a, const Rect &b) {
    return !(a.x2 <= b.x1 || b.x2 <= a.x1 || a.y2 <= b.y1 || b.y2 <= a.y1);
}

bool contains(const Rect &outer, const Rect &inner) {
    return outer.x1 <= inner.x1 && outer.y1 <= inner.y1 && outer.x2 >= inner.x2 && outer.y2 >= inner.y2;
}

bool isIn(const Rect &a, const Rect &b) {
    return a.x1 >= b.x1 && a.y1 >= b.y1 && a.x2 <= b.x2 && a.y2 <= b.y2;
}

vector<Rect> subtract(const Rect &a, const Rect &b) {
    vector<Rect> res;
    if (!intersect(a, b)) {
        res.push_back(a);
        return res;
    }
    long long ax1 = a.x1, ay1 = a.y1, ax2 = a.x2, ay2 = a.y2;
    long long bx1 = b.x1, by1 = b.y1, bx2 = b.x2, by2 = b.y2;
    
    if (ax1 < bx1) {
        res.emplace_back(ax1, ay1, bx1, ay2);
    }
    if (ax2 > bx2) {
        res.emplace_back(bx2, ay1, ax2, ay2);
    }
    if (ay1 < by1) {
        res.emplace_back(max(ax1, bx1), ay1, min(ax2, bx2), by1);
    }
    if (ay2 > by2) {
        res.emplace_back(max(ax1, bx1), by2, min(ax2, bx2), ay2);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    long long n, D;
    cin >> n >> D;
    vector<Rect> rects(n);
    for (int i = 0; i < n; ++i) {
        long long x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        rects[i] = Rect(x1, y1, x2, y2);
    }
    vector<Rect> coverage;
    long long visible = 0;
    for (int i = n - 1; i >= 0; --i) {
        Rect r = rects[i];
        vector<Rect> current;
        current.push_back(r);
        for (const auto &cov : coverage) {
            vector<Rect> next;
            for (const auto &part : current) {
                if (!intersect(part, cov)) {
                    next.push_back(part);
                } else {
                    vector<Rect> subs = subtract(part, cov);
                    next.insert(next.end(), subs.begin(), subs.end());
                }
            }
            current = next;
        }
        if (!current.empty()) {
            ++visible;
            coverage.push_back(r);
        }
    }
    if (visible == D)
        cout << "Yes\n";
    else
        cout << "No\n";
}