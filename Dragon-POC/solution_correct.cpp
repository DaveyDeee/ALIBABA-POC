
#include <bits/stdc++.h>
using namespace std;

struct Graph {
    int n;
    vector<vector<pair<int, int>>> adj;
    vector<int> vertex_weights;
    vector<int> capacities;
    
    Graph(int n) : n(n), adj(n), vertex_weights(n), capacities(n) {}
    
    void add_edge(int u, int v, int w) {
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
};

class DavidsDynamicGraphSolver {
private:
    int n, R, T1, T2, T3;
    int current_round;
    int queries_used;
    Graph graph;
    vector<bool> structure_queried;
    vector<vector<int>> known_adj;
    vector<vector<int>> edge_weights;
    map<string, string> adversary_hints;
    
public:
    DavidsDynamicGraphSolver() : graph(0) {}
    
    void initialize() {
        cin >> n >> R >> T1 >> T2 >> T3;
        graph = Graph(n);
        structure_queried.assign(n, false);
        known_adj.assign(n, vector<int>());
        edge_weights.assign(n, vector<int>(n, -1));
        current_round = 0;
        queries_used = 0;
    }
    
    string query_structure(int x) {
        if (queries_used >= 4*n + 20) return "";
        
        cout << "STRUCTURE " << x << endl;
        cout.flush();
        queries_used++;
        current_round++;
        
        string response;
        getline(cin, response);
        
        // Parse response and update graph knowledge
        structure_queried[x] = true;
        parse_structure_response(x, response);
        
        return response;
    }
    
    string query_color_check(int k, const vector<int>& S) {
        if (queries_used >= 4*n + 20) return "";
        
        cout << "COLOR_CHECK " << k;
        for (int v : S) cout << " " << v;
        cout << endl;
        cout.flush();
        queries_used += 2;
        current_round++;
        
        string response;
        getline(cin, response);
        return response;
    }
    
    string query_domination_weight(const vector<int>& S) {
        if (queries_used >= 4*n + 20) return "";
        
        cout << "DOMINATION_WEIGHT";
        for (int v : S) cout << " " << v;
        cout << endl;
        cout.flush();
        queries_used += 3;
        current_round++;
        
        string response;
        getline(cin, response);
        return response;
    }
    
    string query_adversary_predict() {
        if (queries_used >= 4*n + 20) return "";
        
        cout << "ADVERSARY_PREDICT" << endl;
        cout.flush();
        queries_used += 10;
        current_round++;
        
        string response;
        getline(cin, response);
        adversary_hints[to_string(current_round)] = response;
        return response;
    }
    
    void parse_structure_response(int x, const string& response) {
        // Parse neighbors=[...] weights=[...] vertex_weight=... capacity=...
        // This is a simplified parser - in practice would be more robust
        size_t pos = response.find("vertex_weight=");
        if (pos != string::npos) {
            int weight = stoi(response.substr(pos + 14));
            graph.vertex_weights[x] = weight;
        }
        
        pos = response.find("capacity=");
        if (pos != string::npos) {
            int capacity = stoi(response.substr(pos + 9));
            graph.capacities[x] = capacity;
        }
    }
    
    void discovery_phase() {
        // Phase 1: Learn graph structure efficiently
        for (int i = 0; i < n && current_round < T1; i++) {
            query_structure(i);
        }
    }
    
    void adversarial_phase() {
        // Phase 2: Predict adversary moves and adapt
        while (current_round < T2) {
            if (current_round % 5 == 0) {
                query_adversary_predict();
            }
            
            // Strategic queries based on adversary hints
            for (int i = 0; i < n && current_round < T2; i++) {
                if (!structure_queried[i]) {
                    query_structure(i);
                    break;
                }
            }
            
            if (current_round >= T2) break;
        }
    }
    
    void optimization_phase() {
        // Phase 3: Solve multi-objective problem
        
        // Find minimum k-coloring
        int best_k = find_minimum_coloring();
        
        // Find minimum weight dominating set
        vector<int> dominating_set = find_dominating_set();
        
        // Optimize resource allocation
        vector<int> allocation = optimize_resource_allocation();
        
        // Output final answer
        cout << "ANSWER k=" << best_k << " dominating_set=[";
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
    
    int find_minimum_coloring() {
        // Try k-coloring from k=3 upwards
        for (int k = 3; k <= n; k++) {
            vector<int> all_vertices;
            for (int i = 0; i < n; i++) all_vertices.push_back(i);
            
            string result = query_color_check(k, all_vertices);
            if (result.find("true") != string::npos) {
                return k;
            }
        }
        return n; // Worst case
    }
    
    vector<int> find_dominating_set() {
        // Greedy approach: start with highest weight vertices
        vector<pair<int, int>> weight_vertex;
        for (int i = 0; i < n; i++) {
            weight_vertex.push_back({graph.vertex_weights[i], i});
        }
        sort(weight_vertex.begin(), weight_vertex.end());
        
        vector<int> candidate_set;
        for (int i = 0; i < min(n/2 + 1, n); i++) {
            candidate_set.push_back(weight_vertex[i].second);
        }
        
        string result = query_domination_weight(candidate_set);
        if (result.find("dominates_all=true") != string::npos) {
            return candidate_set;
        }
        
        // Fallback: include more vertices
        candidate_set.clear();
        for (int i = 0; i < n; i++) {
            candidate_set.push_back(i);
        }
        return candidate_set;
    }
    
    vector<int> optimize_resource_allocation() {
        vector<int> allocation(n, 0);
        int remaining = R;
        
        // Distribute resources proportionally to capacity and inverse weight
        vector<pair<double, int>> priority;
        for (int i = 0; i < n; i++) {
            double score = (double)graph.capacities[i] / graph.vertex_weights[i];
            priority.push_back({score, i});
        }
        sort(priority.rbegin(), priority.rend());
        
        for (auto& p : priority) {
            int v = p.second;
            int give = min(remaining, graph.capacities[v]);
            allocation[v] = give;
            remaining -= give;
            if (remaining == 0) break;
        }
        
        return allocation;
    }
    
    void solve() {
        initialize();
        
        discovery_phase();
        adversarial_phase();
        optimization_phase();
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    DavidsDynamicGraphSolver solver;
    solver.solve();
    
    return 0;
}
