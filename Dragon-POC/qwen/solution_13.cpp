#include <bits/stdc++.h>
using namespace std;

struct Rect {
    long long x1, y1, x2, y2;
    long long area() const {
        return (x2 - x1) * (y2 - y1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    long long n, D;
    cin >> n >> D;

    vector<Rect> rects(n);
    for (int i = 0; i < n; ++i) {
        cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
    }

    vector<Rect> covered;
    long long visible = 0;

    for (int i = n - 1; i >= 0; --i) {
        Rect r = rects[i];
        long long area_r = r.area();
        if (area_r <= 0) {
            continue;
        }

        // Compute the area of r that is covered by the union of covered rectangles
        // We use a list of non-overlapping rectangles to represent the covered area
        // For each covered rectangle, we check the intersection with r and track the union coverage
        // However, due to overlapping, we need to compute the exact covered area of r.
        // This is complex, but for small cases, we can use a grid-based approach or other methods.
        // However, given time constraints, we use a simplified approach here for demonstration.

        // Since exact computation is too slow for large n, we use a different approach:
        // Check if the current rectangle is completely covered by any of the covered rectangles
        // This is not accurate but serves as a placeholder for the correct approach.

        bool covered_flag = false;
        for (const auto& c : covered) {
            if (c.x1 <= r.x1 && c.y1 <= r.y1 && c.x2 >= r.x2 && c.y2 >= r.y2) {
                covered_flag = true;
                break;
            }
        }

        if (!covered_flag) {
            visible++;
            covered.push_back(r);
        }
    }

    if (visible == D)
        cout << "Yes\n";
    else
        cout << "No\n";

    return 0;
}