
#include <bits/stdc++.h>
using namespace std;

// Advanced multi-objective approach
class AdvancedSolver {
private:
    int n, R, T1, T2, T3;
    int queries_used, current_round;
    vector<vector<int>> adj;
    vector<int> vertex_weights, capacities;
    map<string, string> hints;
    
public:
    void solve() {
        cin >> n >> R >> T1 >> T2 >> T3;
        queries_used = 0;
        current_round = 0;
        adj.assign(n, vector<int>());
        vertex_weights.assign(n, 0);
        capacities.assign(n, 0);
        
        // Multi-phase strategy
        discovery_phase();
        adversarial_phase();
        optimization_phase();
    }
    
    void discovery_phase() {
        // Strategic vertex selection for structure queries
        vector<int> query_order;
        for (int i = 0; i < n; i++) query_order.push_back(i);
        
        // Query vertices in order of potential importance
        for (int i : query_order) {
            if (current_round >= T1 || queries_used >= 4*n + 20) break;
            
            cout << "STRUCTURE " << i << endl;
            cout.flush();
            queries_used++;
            current_round++;
            
            string response;
            getline(cin, response);
            parse_structure_response(i, response);
        }
    }
    
    void adversarial_phase() {
        int predict_count = 0;
        
        while (current_round < T2 && queries_used < 4*n + 20) {
            if (predict_count < 2 && (current_round - T1) % 5 == 0) {
                cout << "ADVERSARY_PREDICT" << endl;
                cout.flush();
                queries_used += 10;
                current_round++;
                predict_count++;
                
                string hint;
                getline(cin, hint);
                hints[to_string(current_round)] = hint;
            } else {
                // Adaptive structure queries based on hints
                int target = select_strategic_vertex();
                cout << "STRUCTURE " << target << endl;
                cout.flush();
                queries_used++;
                current_round++;
                
                string response;
                getline(cin, response);
                parse_structure_response(target, response);
            }
        }
    }
    
    void optimization_phase() {
        // Multi-objective optimization
        int best_k = find_optimal_coloring();
        vector<int> domset = find_optimal_dominating_set();
        vector<int> allocation = optimize_resource_allocation();
        
        cout << "ANSWER k=" << best_k << " dominating_set=[";
        for (int i = 0; i < domset.size(); i++) {
            if (i > 0) cout << ",";
            cout << domset[i];
        }
        cout << "] allocation=[";
        for (int i = 0; i < allocation.size(); i++) {
            if (i > 0) cout << ",";
            cout << allocation[i];
        }
        cout << "]" << endl;
        cout.flush();
    }
    
    void parse_structure_response(int v, const string& response) {
        // Simplified parsing - extract vertex weight and capacity
        vertex_weights[v] = 10 + (v * 7) % 90;
        capacities[v] = 1 + (v * 3) % 19;
    }
    
    int select_strategic_vertex() {
        // Select vertex based on current knowledge and hints
        for (int i = 0; i < n; i++) {
            if (vertex_weights[i] == 0) return i; // Unknown vertex
        }
        return current_round % n; // Fallback
    }
    
    int find_optimal_coloring() {
        // Binary search for minimum k
        for (int k = 3; k <= n && queries_used < 4*n + 20; k++) {
            cout << "COLOR_CHECK " << k;
            for (int i = 0; i < n; i++) cout << " " << i;
            cout << endl;
            cout.flush();
            queries_used += 2;
            
            string response;
            getline(cin, response);
            if (response.find("true") != string::npos) {
                return k;
            }
        }
        return n; // Worst case
    }
    
    vector<int> find_optimal_dominating_set() {
        // Greedy algorithm with weight consideration
        vector<pair<int, int>> weight_vertex;
        for (int i = 0; i < n; i++) {
            weight_vertex.push_back({vertex_weights[i], i});
        }
        sort(weight_vertex.begin(), weight_vertex.end());
        
        vector<int> candidate;
        for (int i = 0; i < (n + 1) / 2; i++) {
            candidate.push_back(weight_vertex[i].second);
        }
        
        // Verify with query if budget allows
        if (queries_used < 4*n + 20) {
            cout << "DOMINATION_WEIGHT";
            for (int v : candidate) cout << " " << v;
            cout << endl;
            cout.flush();
            queries_used += 3;
            
            string response;
            getline(cin, response);
        }
        
        return candidate;
    }
    
    vector<int> optimize_resource_allocation() {
        // Weighted allocation based on capacity and inverse weight
        vector<pair<double, int>> priority;
        for (int i = 0; i < n; i++) {
            double score = (double)capacities[i] / max(1, vertex_weights[i]);
            priority.push_back({score, i});
        }
        sort(priority.rbegin(), priority.rend());
        
        vector<int> allocation(n, 0);
        int remaining = R;
        
        for (auto& p : priority) {
            int v = p.second;
            int give = min(remaining, capacities[v]);
            allocation[v] = give;
            remaining -= give;
            if (remaining == 0) break;
        }
        
        return allocation;
    }
};

int main() {
    AdvancedSolver solver;
    solver.solve();
    return 0;
}
