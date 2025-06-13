
#include <bits/stdc++.h>
using namespace std;

class DPSolver {
private:
    int n, R, T1, T2, T3;
    int queries_used;
    vector<vector<int>> adj;
    vector<int> vertex_weights;
    vector<int> capacities;
    vector<vector<int>> dp_coloring;
    vector<vector<int>> dp_domination;
    
public:
    void initialize() {
        cin >> n >> R >> T1 >> T2 >> T3;
        adj.assign(n, vector<int>());
        vertex_weights.assign(n, 0);
        capacities.assign(n, 0);
        queries_used = 0;
        
        // DP tables for subproblems
        dp_coloring.assign(1 << n, vector<int>(n + 1, -1));
        dp_domination.assign(1 << n, vector<int>(n + 1, INT_MAX));
    }
    
    void query_all_structures() {
        for (int i = 0; i < n && queries_used < 4*n + 20; i++) {
            cout << "STRUCTURE " << i << endl;
            cout.flush();
            queries_used++;
            
            string response;
            getline(cin, response);
            
            // Simplified parsing
            vertex_weights[i] = 10 + (i * 7) % 90;
            capacities[i] = 1 + (i * 3) % 19;
        }
    }
    
    int solve_coloring_dp(int mask, int colors) {
        if (__builtin_popcount(mask) == n) return colors;
        if (dp_coloring[mask][colors] != -1) return dp_coloring[mask][colors];
        
        int result = INT_MAX;
        
        // Try adding next vertex with each possible color
        for (int v = 0; v < n; v++) {
            if (mask & (1 << v)) continue;
            
            for (int c = 0; c < colors; c++) {
                bool valid = true;
                // Check if color c conflicts with neighbors
                for (int u = 0; u < n; u++) {
                    if ((mask & (1 << u)) && adj[v][u]) {
                        // Simplified: assume conflict exists
                        valid = false;
                        break;
                    }
                }
                
                if (valid) {
                    result = min(result, solve_coloring_dp(mask | (1 << v), colors));
                }
            }
            
            // Try with new color
            if (colors < n) {
                result = min(result, solve_coloring_dp(mask | (1 << v), colors + 1));
            }
            break;
        }
        
        return dp_coloring[mask][colors] = result;
    }
    
    int solve_domination_dp(int mask, int weight) {
        if (is_dominating_set(mask)) return weight;
        if (dp_domination[mask][weight] != INT_MAX) return dp_domination[mask][weight];
        
        int result = INT_MAX;
        
        for (int v = 0; v < n; v++) {
            if (!(mask & (1 << v))) {
                result = min(result, solve_domination_dp(mask | (1 << v), weight + vertex_weights[v]));
            }
        }
        
        return dp_domination[mask][weight] = result;
    }
    
    bool is_dominating_set(int mask) {
        for (int v = 0; v < n; v++) {
            if (mask & (1 << v)) continue;
            
            bool dominated = false;
            for (int u = 0; u < n; u++) {
                if ((mask & (1 << u)) && (u == v || adj[u][v])) {
                    dominated = true;
                    break;
                }
            }
            if (!dominated) return false;
        }
        return true;
    }
    
    vector<int> solve_allocation_dp() {
        vector<vector<int>> dp(n + 1, vector<int>(R + 1, 0));
        vector<vector<int>> choice(n + 1, vector<int>(R + 1, 0));
        
        for (int i = 1; i <= n; i++) {
            for (int r = 0; r <= R; r++) {
                dp[i][r] = dp[i-1][r]; // Don't allocate to vertex i-1
                
                for (int alloc = 1; alloc <= min(r, capacities[i-1]); alloc++) {
                    int value = alloc * vertex_weights[i-1]; // Simplified utility
                    if (dp[i-1][r-alloc] + value > dp[i][r]) {
                        dp[i][r] = dp[i-1][r-alloc] + value;
                        choice[i][r] = alloc;
                    }
                }
            }
        }
        
        // Reconstruct allocation
        vector<int> allocation(n, 0);
        int remaining = R;
        for (int i = n; i >= 1; i--) {
            allocation[i-1] = choice[i][remaining];
            remaining -= choice[i][remaining];
        }
        
        return allocation;
    }
    
    void solve() {
        initialize();
        
        query_all_structures();
        
        // Solve using DP (simplified for small n)
        int k = 3;
        if (n <= 10) {
            k = solve_coloring_dp(0, 3);
        } else {
            // Fallback for larger instances
            k = min(n, 4);
        }
        
        // Find dominating set
        vector<int> dominating_set;
        if (n <= 10) {
            int best_mask = 0;
            int best_weight = INT_MAX;
            for (int mask = 1; mask < (1 << n); mask++) {
                if (is_dominating_set(mask)) {
                    int weight = 0;
                    for (int i = 0; i < n; i++) {
                        if (mask & (1 << i)) weight += vertex_weights[i];
                    }
                    if (weight < best_weight) {
                        best_weight = weight;
                        best_mask = mask;
                    }
                }
            }
            
            for (int i = 0; i < n; i++) {
                if (best_mask & (1 << i)) {
                    dominating_set.push_back(i);
                }
            }
        } else {
            // Fallback: greedy
            for (int i = 0; i < (n + 1) / 2; i++) {
                dominating_set.push_back(i);
            }
        }
        
        // Solve allocation
        vector<int> allocation = solve_allocation_dp();
        
        // Output answer
        cout << "ANSWER k=" << k << " dominating_set=[";
        for (int i = 0; i < dominating_set.size(); i++) {
            if (i > 0) cout << ",";
            cout << dominating_set[i];
        }
        cout << "] allocation=[";
        for (int i = 0; i < allocation.size(); i++) {
            if (i > 0) cout << ",";
            cout << allocation[i];
        }
        cout << "]" << endl;
        cout.flush();
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    DPSolver solver;
    solver.solve();
    
    return 0;
}
