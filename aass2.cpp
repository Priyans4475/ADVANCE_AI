#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>

using namespace std;

struct Assignment {
    int id;
    int prompt_count;
    vector<int> dependencies;
};

// Global storage for the assignment graph
map<int, Assignment> assignments;

// Function to simulate the schedule and find earliest completion time
int calculateEarliestTime(int N, int K) {
    int day = 0;
    set<int> completed;
    map<int, int> finish_day; // Track when each task was finished

    // Check if any single task exceeds the daily prompt limit K
    for (auto const& [id, assign] : assignments) {
        if (assign.prompt_count > K) return 1e9; // Impossible to complete [cite: 38]
    }

    while (completed.size() < assignments.size()) {
        day++;
        int students_available = N;
        vector<int> started_today;

        for (auto const& [id, assign] : assignments) {
            if (completed.find(id) != completed.end()) continue;

            // Check dependencies and the 6 AM next-day exchange rule 
            bool can_start = true;
            for (int dep : assign.dependencies) {
                // Dependency must be finished at least one day prior to be available at 6 AM today 
                if (finish_day.find(dep) == finish_day.end() || finish_day[dep] >= day) {
                    can_start = false;
                    break;
                }
            }

            if (can_start && students_available > 0) {
                // Assignment 1/2 rule: Must finish in one day (no partial work) 
                // Student must have sufficient prompts [cite: 11]
                if (assign.prompt_count <= K) {
                    started_today.push_back(id);
                    students_available--;
                }
            }
        }

        if (started_today.empty() && completed.size() < assignments.size()) {
            // Check if we are stuck (deadlock or K too small)
            if(day > 1000) return 1e9; 
            continue; 
        }

        for (int id : started_today) {
            completed.insert(id);
            finish_day[id] = day;
        }
    }
    return day;
}

void parseInput(string filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (line.empty() || line[0] == '%' || line[0] == '$') continue; 
        
        stringstream ss(line);
        char type;
        ss >> type;
        if (type == 'A') {
            int id, prompts, dep;
            ss >> id >> prompts;
            Assignment as;
            as.id = id;
            as.prompt_count = prompts;
            while (ss >> dep && dep != 0) { 
                as.dependencies.push_back(dep);
            }
            assignments[id] = as;
        }
    }
}

int main(int argc, char* argv[]) {
    // We expect: executable, input_file, N, value (K or M), and mode ("time" or "sub")
    if (argc < 5) {
        cout << "Usage for Earliest Time: " << argv[0] << " <input_file> <N> <K> time" << endl;
        cout << "Usage for Min Subscription: " << argv[0] << " <input_file> <N> <M_days> sub" << endl;
        return 1;
    }

    string filename = argv[1];
    int N = stoi(argv[2]);
    int third_arg = stoi(argv[3]);
    string mode = argv[4];

    parseInput(filename); // Load assignments and dependencies [cite: 6, 29]

    if (mode == "time") {
        // Part 1: Given N and K, find earliest completion time [cite: 7, 12]
        int K = third_arg;
        int result = calculateEarliestTime(N, K);
        
        if (result >= 1e9) {
            cout << "Result: Impossible. Some tasks exceed prompt limit K." << endl;
        } else {
            cout << "Earliest completion time: " << result << " days." << endl;
        }
    } 
    else if (mode == "sub") {
        // Part 2: Given N and M days, find minimum subscription K [cite: 8, 12]
        int M_deadline = third_arg;
        int low = 1, high = 1000; // Search range for prompts
        int min_K = -1;

        while (low <= high) {
            int mid_K = low + (high - low) / 2;
            // Test mid_K using the 6 AM exchange simulation 
            if (calculateEarliestTime(N, mid_K) <= M_deadline) {
                min_K = mid_K;
                high = mid_K - 1; // Try to find a smaller K
            } else {
                low = mid_K + 1; // Need more prompts to meet deadline
            }
        }

        if (min_K != -1) {
            cout << "Minimum prompts per day (K) needed to finish within " << M_deadline << " days: " << min_K << endl;
        } else {
            cout << "Result: Impossible to finish within " << M_deadline << " days even with max prompts." << endl;
        }
    } else {
        cout << "Invalid mode. Use 'time' or 'sub'." << endl;
    }

    return 0;
}