#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// Structure to represent a vertical line event
struct Event {
    ll x;
    ll y1, y2;
    bool isStart;

    bool operator<(const Event& other) const {
        return x < other.x || (x == other.x && isStart > other.isStart);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    ll n, D;
    cin >> n >> D;

    vector<vector<ll>> rects(n, vector<ll>(4));
    for (ll i = 0; i < n; ++i) {
        cin >> rects[i][0] >> rects[i][1] >> rects[i][2] >> rects[i][3];
    }

    ll visible = 0;

    // Process rectangles in reverse order
    for (ll i = n - 1; i >= 0; --i) {
        ll x1 = rects[i][0], y1 = rects[i][1], x2 = rects[i][2], y2 = rects[i][3];

        // Step 1: Build events for the current rectangle
        vector<Event> events;
        events.push_back({x1, y1, y2, true});
        events.push_back({x2, y1, y2, false});

        // Step 2: Sort events by x-coordinate
        sort(events.begin(), events.end());

        // Step 3: Sweep line algorithm
        multiset<pair<ll, ll>> active; // active intervals: {y_start, y_end}
        bool isCovered = true;

        for (const auto& e : events) {
            if (e.isStart) {
                // Add interval
                active.insert({e.y1, e.y2});
            } else {
                // Remove interval
                auto it = active.find({e.y1, e.y2});
                if (it != active.end()) {
                    active.erase(it);
                }
            }

            // Check coverage of the current rectangle's y-interval
            ll curr_y1 = y1, curr_y2 = y2;
            bool covered = false;

            for (auto [a_y1, a_y2] : active) {
                if (a_y1 <= curr_y1 && a_y2 >= curr_y2) {
                    covered = true;
                    break;
                }
            }

            if (!covered) {
                isCovered = false;
                break;
            }
        }

        if (!isCovered) {
            visible++;
        }
    }

    if (visible == D) {
        cout << "Yes\n";
    } else {
        cout << "No\n";
    }

    return 0;
}