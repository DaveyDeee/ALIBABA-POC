
#include <bits/stdc++.h>
using namespace std;

// Basic greedy approach - first attempt
class BasicSolver {
private:
    int n, R, T1, T2, T3;
    int queries_used;
    
public:
    void solve() {
        cin >> n >> R >> T1 >> T2 >> T3;
        queries_used = 0;
        
        // Simple strategy: query all structures first
        for (int i = 0; i < n && queries_used < 4*n + 20; i++) {
            cout << "STRUCTURE " << i << endl;
            cout.flush();
            queries_used++;
            
            string response;
            getline(cin, response);
        }
        
        // Try 3-coloring
        cout << "COLOR_CHECK 3";
        for (int i = 0; i < n; i++) cout << " " << i;
        cout << endl;
        cout.flush();
        queries_used += 2;
        
        string color_response;
        getline(cin, color_response);
        
        // Simple dominating set (first half of vertices)
        vector<int> domset;
        for (int i = 0; i < (n+1)/2; i++) {
            domset.push_back(i);
        }
        
        // Equal resource distribution
        vector<int> allocation(n, R/n);
        int remainder = R % n;
        for (int i = 0; i < remainder; i++) {
            allocation[i]++;
        }
        
        cout << "ANSWER k=3 dominating_set=[";
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
    BasicSolver solver;
    solver.solve();
    return 0;
}
