
#include <bits/stdc++.h>
using namespace std;

class TestCaseGenerator {
private:
    mt19937 rng;
    
public:
    TestCaseGenerator() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}
    
    struct TestCase {
        int n, R, T1, T2, T3;
        vector<vector<pair<int, int>>> adj;
        vector<int> vertex_weights;
        vector<int> capacities;
        vector<tuple<int, int, int>> adversarial_moves;
        string graph_type;
        string adversarial_pattern;
    };
    
    TestCase generate_test_case(int n, string graph_type, string adversarial_pattern) {
        TestCase tc;
        tc.n = n;
        tc.R = 2 * n;
        tc.T1 = n;
        tc.T2 = 2 * n;
        tc.T3 = 3 * n;
        tc.graph_type = graph_type;
        tc.adversarial_pattern = adversarial_pattern;
        
        tc.adj.assign(n, vector<pair<int, int>>());
        tc.vertex_weights.resize(n);
        tc.capacities.resize(n);
        
        // Generate vertex properties
        for (int i = 0; i < n; i++) {
            tc.vertex_weights[i] = uniform_int_distribution<int>(1, 100)(rng);
            tc.capacities[i] = uniform_int_distribution<int>(1, 20)(rng);
        }
        
        // Generate graph structure based on type
        if (graph_type == "complete") {
            generate_complete_graph(tc);
        } else if (graph_type == "tree") {
            generate_tree_graph(tc);
        } else if (graph_type == "cycle") {
            generate_cycle_graph(tc);
        } else if (graph_type == "sparse") {
            generate_sparse_graph(tc);
        } else if (graph_type == "bipartite") {
            generate_bipartite_graph(tc);
        } else {
            generate_random_graph(tc);
        }
        
        // Generate adversarial moves
        generate_adversarial_moves(tc);
        
        return tc;
    }
    
    void generate_complete_graph(TestCase& tc) {
        for (int i = 0; i < tc.n; i++) {
            for (int j = i + 1; j < tc.n; j++) {
                int weight = uniform_int_distribution<int>(1, 50)(rng);
                tc.adj[i].push_back({j, weight});
                tc.adj[j].push_back({i, weight});
            }
        }
    }
    
    void generate_tree_graph(TestCase& tc) {
        for (int i = 1; i < tc.n; i++) {
            int parent = uniform_int_distribution<int>(0, i - 1)(rng);
            int weight = uniform_int_distribution<int>(1, 50)(rng);
            tc.adj[i].push_back({parent, weight});
            tc.adj[parent].push_back({i, weight});
        }
    }
    
    void generate_cycle_graph(TestCase& tc) {
        for (int i = 0; i < tc.n; i++) {
            int next = (i + 1) % tc.n;
            int weight = uniform_int_distribution<int>(1, 50)(rng);
            tc.adj[i].push_back({next, weight});
            tc.adj[next].push_back({i, weight});
        }
    }
    
    void generate_sparse_graph(TestCase& tc) {
        int edges = min(tc.n + 5, tc.n * (tc.n - 1) / 2);
        set<pair<int, int>> added;
        
        for (int e = 0; e < edges; e++) {
            int u, v;
            do {
                u = uniform_int_distribution<int>(0, tc.n - 1)(rng);
                v = uniform_int_distribution<int>(0, tc.n - 1)(rng);
            } while (u == v || added.count({min(u, v), max(u, v)}));
            
            added.insert({min(u, v), max(u, v)});
            int weight = uniform_int_distribution<int>(1, 50)(rng);
            tc.adj[u].push_back({v, weight});
            tc.adj[v].push_back({u, weight});
        }
    }
    
    void generate_bipartite_graph(TestCase& tc) {
        int n1 = tc.n / 2;
        int n2 = tc.n - n1;
        
        for (int i = 0; i < n1; i++) {
            for (int j = n1; j < tc.n; j++) {
                if (uniform_real_distribution<double>(0, 1)(rng) < 0.4) {
                    int weight = uniform_int_distribution<int>(1, 50)(rng);
                    tc.adj[i].push_back({j, weight});
                    tc.adj[j].push_back({i, weight});
                }
            }
        }
    }
    
    void generate_random_graph(TestCase& tc) {
        double edge_prob = min(0.5, 2.0 * log(tc.n) / tc.n);
        
        for (int i = 0; i < tc.n; i++) {
            for (int j = i + 1; j < tc.n; j++) {
                if (uniform_real_distribution<double>(0, 1)(rng) < edge_prob) {
                    int weight = uniform_int_distribution<int>(1, 50)(rng);
                    tc.adj[i].push_back({j, weight});
                    tc.adj[j].push_back({i, weight});
                }
            }
        }
    }
    
    void generate_adversarial_moves(TestCase& tc) {
        int num_moves = uniform_int_distribution<int>(3, 8)(rng);
        
        for (int m = 0; m < num_moves; m++) {
            int round = uniform_int_distribution<int>(tc.T1 + 1, tc.T2)(rng);
            int u = uniform_int_distribution<int>(0, tc.n - 1)(rng);
            int v = uniform_int_distribution<int>(0, tc.n - 1)(rng);
            
            if (u != v) {
                tc.adversarial_moves.push_back({round, u, v});
            }
        }
    }
    
    void write_test_case(const TestCase& tc, const string& filename) {
        ofstream out(filename);
        out << tc.n << " " << tc.R << " " << tc.T1 << " " << tc.T2 << " " << tc.T3 << "\n";
        
        // Write graph structure
        out << "GRAPH\n";
        for (int i = 0; i < tc.n; i++) {
            out << "VERTEX " << i << " " << tc.vertex_weights[i] << " " << tc.capacities[i] << "\n";
            for (auto& edge : tc.adj[i]) {
                if (edge.first > i) { // Avoid duplicate edges
                    out << "EDGE " << i << " " << edge.first << " " << edge.second << "\n";
                }
            }
        }
        
        // Write adversarial moves
        out << "ADVERSARIAL\n";
        for (auto& move : tc.adversarial_moves) {
            out << "MOVE " << get<0>(move) << " " << get<1>(move) << " " << get<2>(move) << "\n";
        }
        
        out << "END\n";
        out.close();
    }
    
    void generate_all_test_cases() {
        vector<string> graph_types = {"complete", "tree", "cycle", "sparse", "bipartite", "random"};
        vector<string> adversarial_patterns = {"predictable", "random", "adaptive"};
        
        int test_num = 1;
        
        // Small graphs (n=5-8)
        for (int n = 5; n <= 8; n++) {
            for (const string& graph_type : graph_types) {
                for (const string& adversarial_pattern : adversarial_patterns) {
                    TestCase tc = generate_test_case(n, graph_type, adversarial_pattern);
                    write_test_case(tc, "tests/test_" + to_string(test_num) + ".in");
                    test_num++;
                    if (test_num > 15) goto medium_graphs;
                }
            }
        }
        
        medium_graphs:
        // Medium graphs (n=9-18)
        for (int n = 9; n <= 18; n += 3) {
            for (const string& graph_type : graph_types) {
                TestCase tc = generate_test_case(n, graph_type, "adaptive");
                write_test_case(tc, "tests/test_" + to_string(test_num) + ".in");
                test_num++;
                if (test_num > 35) goto large_graphs;
            }
        }
        
        large_graphs:
        // Large graphs (n=19-25)
        for (int n = 19; n <= 25; n += 2) {
            for (const string& graph_type : {"random", "sparse", "bipartite"}) {
                TestCase tc = generate_test_case(n, graph_type, "adaptive");
                write_test_case(tc, "tests/test_" + to_string(test_num) + ".in");
                test_num++;
                if (test_num > 45) break;
            }
        }
        
        cout << "Generated " << test_num - 1 << " test cases." << endl;
    }
};

int main(int argc, char* argv[]) {
    TestCaseGenerator generator;
    
    if (argc > 1) {
        // Generate specific test case
        int n = stoi(argv[1]);
        string graph_type = argc > 2 ? argv[2] : "random";
        string adversarial_pattern = argc > 3 ? argv[3] : "adaptive";
        
        TestCaseGenerator::TestCase tc = generator.generate_test_case(n, graph_type, adversarial_pattern);
        generator.write_test_case(tc, "test_custom.in");
        cout << "Generated custom test case with n=" << n << endl;
    } else {
        // Generate all test cases
        generator.generate_all_test_cases();
    }
    
    return 0;
}
