
#include <bits/stdc++.h>
using namespace std;

class InteractiveJudge {
private:
    struct Graph {
        int n;
        vector<vector<pair<int, int>>> adj;
        vector<int> vertex_weights;
        vector<int> capacities;
        vector<tuple<int, int, int>> adversarial_moves;
        int current_round;
        
        Graph() : current_round(0) {}
    };
    
    Graph graph;
    int R, T1, T2, T3;
    int queries_used;
    bool game_ended;
    
public:
    void load_test_case(const string& filename) {
        ifstream in(filename);
        in >> graph.n >> R >> T1 >> T2 >> T3;
        
        graph.adj.assign(graph.n, vector<pair<int, int>>());
        graph.vertex_weights.resize(graph.n);
        graph.capacities.resize(graph.n);
        
        string line;
        getline(in, line); // consume newline
        
        while (getline(in, line)) {
            if (line == "GRAPH") continue;
            if (line == "ADVERSARIAL") continue;
            if (line == "END") break;
            
            istringstream iss(line);
            string type;
            iss >> type;
            
            if (type == "VERTEX") {
                int id, weight, capacity;
                iss >> id >> weight >> capacity;
                graph.vertex_weights[id] = weight;
                graph.capacities[id] = capacity;
            } else if (type == "EDGE") {
                int u, v, weight;
                iss >> u >> v >> weight;
                graph.adj[u].push_back({v, weight});
                graph.adj[v].push_back({u, weight});
            } else if (type == "MOVE") {
                int round, u, v;
                iss >> round >> u >> v;
                graph.adversarial_moves.push_back({round, u, v});
            }
        }
        
        queries_used = 0;
        game_ended = false;
        graph.current_round = 0;
    }
    
    string handle_query(const string& query) {
        if (game_ended || queries_used >= 4 * graph.n + 20) {
            return "ERROR: Query limit exceeded";
        }
        
        istringstream iss(query);
        string type;
        iss >> type;
        
        graph.current_round++;
        
        // Apply adversarial moves if in adversarial phase
        if (graph.current_round > T1 && graph.current_round <= T2) {
            apply_adversarial_moves();
        }
        
        if (type == "STRUCTURE") {
            int x;
            iss >> x;
            queries_used += 1;
            return handle_structure_query(x);
        } else if (type == "COLOR_CHECK") {
            int k;
            iss >> k;
            vector<int> S;
            int v;
            while (iss >> v) S.push_back(v);
            queries_used += 2;
            return handle_color_check_query(k, S);
        } else if (type == "DOMINATION_WEIGHT") {
            vector<int> S;
            int v;
            while (iss >> v) S.push_back(v);
            queries_used += 3;
            return handle_domination_weight_query(S);
        } else if (type == "DISTANCE_MATRIX") {
            vector<int> S;
            int v;
            while (iss >> v) S.push_back(v);
            queries_used += 5;
            return handle_distance_matrix_query(S);
        } else if (type == "ADVERSARY_PREDICT") {
            queries_used += 10;
            return handle_adversary_predict_query();
        } else if (type == "ANSWER") {
            game_ended = true;
            return handle_answer_query(query);
        }
        
        return "ERROR: Unknown query type";
    }
    
    string handle_structure_query(int x) {
        if (x < 0 || x >= graph.n) return "ERROR: Invalid vertex";
        
        ostringstream oss;
        oss << "neighbors=[";
        for (int i = 0; i < graph.adj[x].size(); i++) {
            if (i > 0) oss << ",";
            oss << graph.adj[x][i].first;
        }
        oss << "] weights=[";
        for (int i = 0; i < graph.adj[x].size(); i++) {
            if (i > 0) oss << ",";
            oss << "(" << graph.adj[x][i].first << "," << graph.adj[x][i].second << ")";
        }
        oss << "] vertex_weight=" << graph.vertex_weights[x];
        oss << " capacity=" << graph.capacities[x];
        
        return oss.str();
    }
    
    string handle_color_check_query(int k, const vector<int>& S) {
        // Simplified coloring check - assume it's possible if k >= 3
        bool feasible = (k >= 3);
        return feasible ? "feasible=true" : "feasible=false";
    }
    
    string handle_domination_weight_query(const vector<int>& S) {
        int total_weight = 0;
        for (int v : S) {
            if (v >= 0 && v < graph.n) {
                total_weight += graph.vertex_weights[v];
            }
        }
        
        // Check if S dominates all vertices (simplified)
        bool dominates_all = S.size() >= (graph.n + 1) / 2;
        
        ostringstream oss;
        oss << "total_weight=" << total_weight << " dominates_all=" << (dominates_all ? "true" : "false");
        return oss.str();
    }
    
    string handle_distance_matrix_query(const vector<int>& S) {
        // Simplified distance matrix (all distances = 1 or 2)
        ostringstream oss;
        oss << "matrix=[";
        for (int i = 0; i < S.size(); i++) {
            if (i > 0) oss << ",";
            oss << "[";
            for (int j = 0; j < S.size(); j++) {
                if (j > 0) oss << ",";
                oss << (i == j ? 0 : (abs(S[i] - S[j]) == 1 ? 1 : 2));
            }
            oss << "]";
        }
        oss << "]";
        return oss.str();
    }
    
    string handle_adversary_predict_query() {
        // Generate hint based on current round
        ostringstream oss;
        oss << "hint=\"Will modify edges based on query pattern\"";
        return oss.str();
    }
    
    string handle_answer_query(const string& query) {
        // Parse answer and calculate score
        int score = calculate_score(query);
        
        ostringstream oss;
        oss << "Score: " << score << "/1000";
        return oss.str();
    }
    
    void apply_adversarial_moves() {
        for (auto& move : graph.adversarial_moves) {
            if (get<0>(move) == graph.current_round) {
                int u = get<1>(move);
                int v = get<2>(move);
                
                // Add edge if not exists, remove if exists
                bool found = false;
                for (auto it = graph.adj[u].begin(); it != graph.adj[u].end(); ++it) {
                    if (it->first == v) {
                        graph.adj[u].erase(it);
                        found = true;
                        break;
                    }
                }
                
                if (found) {
                    for (auto it = graph.adj[v].begin(); it != graph.adj[v].end(); ++it) {
                        if (it->first == u) {
                            graph.adj[v].erase(it);
                            break;
                        }
                    }
                } else {
                    int weight = 1 + (u + v) % 50;
                    graph.adj[u].push_back({v, weight});
                    graph.adj[v].push_back({u, weight});
                }
            }
        }
    }
    
    int calculate_score(const string& answer) {
        // Simplified scoring - base score minus penalties
        int base_score = 1000;
        int penalties = 0;
        
        // Query efficiency penalty
        penalties += max(0, queries_used - (3 * graph.n));
        
        // Phase timing penalty
        if (graph.current_round > T3) penalties += 50;
        
        return max(0, base_score - penalties);
    }
    
    void run_interactive_session() {
        cout << graph.n << " " << R << " " << T1 << " " << T2 << " " << T3 << endl;
        
        string query;
        while (getline(cin, query) && !game_ended) {
            string response = handle_query(query);
            cout << response << endl;
            cout.flush();
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <test_case_file>" << endl;
        return 1;
    }
    
    InteractiveJudge judge;
    judge.load_test_case(argv[1]);
    judge.run_interactive_session();
    
    return 0;
}
