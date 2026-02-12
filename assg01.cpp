#include <bits/stdc++.h>
using namespace std;

struct assign {
    int cost;
    vector<int> depend;
};

int N, K, M;
map<int, assign> tasks;
vector<vector<string>> res;

// if all dependcy are done, then only proceed
bool check(int id, const set<int>& visited) {
    for (int p : tasks[id].depend)
        if (!visited.count(p)) 
        return false;

    return true;
}

void dfs(int day, int std, int prmpt,
         set<int>& visited, vector<string>& sche) {

    if (visited.size() == tasks.size()) {
        res.push_back(sche);
        return;
    }
    if (day > M) return;

    bool poss = false;
    
for (auto it = tasks.begin(); it != tasks.end(); ++it) {
    int id = it->first;
    assign &t = it->second;

    if (!visited.count(id) && check(id, visited) && prmpt >= t.cost) {
        poss = true;

        visited.insert(id);
        sche.push_back("Day " + to_string(day) +
                       ", stud " + to_string(std) +
                       " solve A" + to_string(id));

        dfs(day, std, prmpt - t.cost, visited, sche);
// backtrack_here_for_other_possibilities
        sche.pop_back();
        visited.erase(id);
    }
}

// if not poss above, then move to next stud or next day
    if (!poss) {
        if (std < N)
            dfs(day, std + 1, K, visited, sche);
        else
            dfs(day + 1, 1, K, visited, sche);
    }
}
int main(int argc, char* argv[]) {
    

    M = stoi(argv[2]);
    ifstream in(argv[1]);
    string line;

    while (getline(in, line)) {
        if (line.empty() || line[0] == '%') continue;

        stringstream ss(line);
        char c; ss >> c;

        if (c == 'N') ss >> N;
        else if (c == 'K') ss >> K;
        else if (c == 'A') {
            int id, cost, x;
            ss >> id >> cost;
            vector<int> dep;
            while (ss >> x && x) dep.push_back(x);
            tasks[id] = {cost, dep};
        }
    }

    set<int> visited;
    vector<string> sche;
    dfs(1, 1, K, visited, sche);

    if (res.empty()) {
        cout << "No schedules found.\n";
    } else {
        for (int i = 0; i < res.size(); i++) {
            cout << "Schedule #" << i + 1 << ":\n";
            for (auto &s : res[i]) cout << "  " << s << "\n";
            cout << "\n";
        }
    }
}
