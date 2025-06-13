#include <bits/stdc++.h>
using namespace std;

struct Point {
    long long x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<long long>()(p.x) ^ (hash<long long>()(p.y) << 16);
        }
    };
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long n, D;
    cin >> n >> D;

    vector<vector<long long>> rects(n, vector<long long>(4));
    for (long long i = 0; i < n; ++i) {
        cin >> rects[i][0] >> rects[i][1] >> rects[i][2] >> rects[i][3];
    }

    unordered_set<Point> processed;
    long long visible = 0;

    for (long long i = n - 1; i >= 0; --i) {
        long long x1 = rects[i][0], y1 = rects[i][1];
        long long x2 = rects[i][2], y2 = rects[i][3];

        Point p0 = {x1, y1};
        Point p1 = {x2, y2};
        Point p2 = {x1, y2};
        Point p3 = {x2, y1};
        Point p4 = {(x1 + x2) / 2, (y1 + y2) / 2};

        bool found = false;
        if (processed.find(p0) == processed.end()) {
            found = true;
        } else if (processed.find(p1) == processed.end()) {
            found = true;
        } else if (processed.find(p2) == processed.end()) {
            found = true;
        } else if (processed.find(p3) == processed.end()) {
            found = true;
        } else if (processed.find(p4) == processed.end()) {
            found = true;
        }

        if (found) {
            visible++;
            processed.insert(p0);
            processed.insert(p1);
            processed.insert(p2);
            processed.insert(p3);
            processed.insert(p4);
        }
    }

    if (visible == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }

    return 0;
}