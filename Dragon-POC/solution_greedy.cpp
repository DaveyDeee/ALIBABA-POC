
#include <bits/stdc++.h>
using namespace std;

class GreedySolver {
private:
    int n, R, T1, T2, T3;
    int queries_used;
    vector<vector<int>> adj;
    vector<int> vertex_weights;
    vector<int> capacities;
    
public:
    void initialize() {
        cin >> n >> R >> T1 >> T2 >> T3;
        adj.assign(n, vector<int>());
        vertex_weights.assign(n, 0);
        capacities.assign(n, 0);
        queries_used = 0;
    }
    
    void query_structure(int x) {
        if (queries_used >= 4*n + 20) return;
        
        cout << "STRUCTURE " << x << endl;
        cout.flush();
        queries_used++;
        
        // Simplified response parsing
        string response;
        getline(cin, response);
        
        // Extract vertex weight and capacity (simplified)
        vertex_weights[x] = 10 + (x * 7) % 90; // Placeholder
        capacities[x] = 1 + (x * 3) % 19;     // Placeholder
    }
    
    bool query_color_check(int k) {
        if (queries_used >= 4*n + 20) return false;
        
        cout << "COLOR_CHECK " << k;
        for (int i = 0; i < n; i++) cout << " " << i;
        cout << endl;
        cout.flush();
        queries_used += 2;
        
        string response;
        getline(cin, response);
        return response.find("true") != string::npos;
    }
    
    void solve() {
        initialize();
        
        // Quick discovery phase
        for (int i = 0; i < n; i++) {
            query_structure(i);
        }
        
        // Find minimum k for coloring
        int k = 3;
        while (k <= n && !query_color_check(k)) {
            k++;
        }
        
        // Greedy dominating set (just pick first half)
        vector<int> dominating_set;
        for (int i = 0; i < (n + 1) / 2; i++) {
            dominating_set.push_back(i);
        }
        
        // Simple resource allocation
        vector<int> allocation(n, 0);
        int per_vertex = R / n;
        int remainder = R % n;
        
        for (int i = 0; i < n; i++) {
            allocation[i] = min(per_vertex + (i < remainder ? 1 : 0), capacities[i]);
        }
        
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
    
    GreedySolver solver;
    solver.solve();
    
    return 0;
}
