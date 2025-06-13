
#include <bits/stdc++.h>
using namespace std;

// Improved approach with adversary prediction
class ImprovedSolver {
private:
    int n, R, T1, T2, T3;
    int queries_used, current_round;
    vector<string> adversary_hints;
    
public:
    void solve() {
        cin >> n >> R >> T1 >> T2 >> T3;
        queries_used = 0;
        current_round = 0;
        
        // Phase 1: Discovery
        discovery_phase();
        
        // Phase 2: Adversarial
        adversarial_phase();
        
        // Phase 3: Optimization
        optimization_phase();
    }
    
    void discovery_phase() {
        for (int i = 0; i < n && current_round < T1 && queries_used < 4*n + 20; i++) {
            cout << "STRUCTURE " << i << endl;
            cout.flush();
            queries_used++;
            current_round++;
            
            string response;
            getline(cin, response);
        }
    }
    
    void adversarial_phase() {
        while (current_round < T2 && queries_used < 4*n + 20) {
            if (current_round % 3 == 0) {
                cout << "ADVERSARY_PREDICT" << endl;
                cout.flush();
                queries_used += 10;
                current_round++;
                
                string hint;
                getline(cin, hint);
                adversary_hints.push_back(hint);
            } else {
                // Strategic structure queries
                int target = current_round % n;
                cout << "STRUCTURE " << target << endl;
                cout.flush();
                queries_used++;
                current_round++;
                
                string response;
                getline(cin, response);
            }
        }
    }
    
    void optimization_phase() {
        // Try different k values
        int best_k = 3;
        for (int k = 3; k <= min(n, 6) && queries_used < 4*n + 20; k++) {
            cout << "COLOR_CHECK " << k;
            for (int i = 0; i < n; i++) cout << " " << i;
            cout << endl;
            cout.flush();
            queries_used += 2;
            
            string response;
            getline(cin, response);
            if (response.find("true") != string::npos) {
                best_k = k;
                break;
            }
        }
        
        // Greedy dominating set
        vector<int> domset;
        for (int i = 0; i < n; i += 2) {
            domset.push_back(i);
        }
        
        // Check domination weight
        if (queries_used < 4*n + 20) {
            cout << "DOMINATION_WEIGHT";
            for (int v : domset) cout << " " << v;
            cout << endl;
            cout.flush();
            queries_used += 3;
            
            string response;
            getline(cin, response);
        }
        
        // Resource allocation based on position
        vector<int> allocation(n, 0);
        int remaining = R;
        for (int i = 0; i < n && remaining > 0; i++) {
            int give = min(remaining, (R / n) + 1);
            allocation[i] = give;
            remaining -= give;
        }
        
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
};

int main() {
    ImprovedSolver solver;
    solver.solve();
    return 0;
}
