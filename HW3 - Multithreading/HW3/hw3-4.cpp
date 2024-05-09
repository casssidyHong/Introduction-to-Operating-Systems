#include <iostream>
#include <cstdint>
#include <vector>
#include <thread>

using namespace std;

int n, m;
vector<uint64_t> subsets;
uint64_t one = static_cast<uint64_t>(1), global_count = 0;
int count[9] = {0};
int i;

void solve(int index, uint64_t current, int i) {
    if (index == m) {
        if (current == (one << n) - 1) count[i]++;
        //cout << "current: " << current << " ";
        //cout << "index: " << index << endl;
    } else {
        solve(index + 1, current, i);
        solve(index + 1, current | subsets[index], i);
    }
}

int main(int argc, char* argv[]) {
    cin >> n >> m;

    subsets.resize(m);
    for (int i = 0; i < m; i++) {
        int p, temp;
        cin >> p;
        for (int j = 0; j < p; j++) {
            cin >> temp;
            subsets[i] |= (one << temp);
        }
    }

    int num_threads = -1;
    // Check if the "-t" flag is provided
    if (argc == 3 && string(argv[1]) == "-t") {
        num_threads = stoi(argv[2]);
        if (num_threads != 1 && num_threads != 2 && num_threads != 4 && num_threads != 8) {
            cerr << "Invalid number of threads." << endl;
            return 1;
        }
    }

    vector<thread> threads;

    if (num_threads == 1 || m==1) //1 thread or 1 subset
        solve(0, 0, 0);
    else if (num_threads == 2 || m==2) { //2 threads and at least 2 subsets
        threads.emplace_back(thread(solve, 1, 0, 0));
        threads.emplace_back(thread(solve, 1, subsets[0], 1));
    }
    else if (num_threads == 4 || m==3) { //4 threads and at least 3 subsets
        threads.emplace_back(thread(solve, 2, 0, 0));
        threads.emplace_back(thread(solve, 2, subsets[0], 1));
        threads.emplace_back(thread(solve, 2, subsets[1], 2));
        threads.emplace_back(thread(solve, 2, subsets[0] | subsets[1], 3));
    }
    //else if (num_threads == 8 && m > 3) { //8 threads and at least 4 subsets
    else {
        threads.emplace_back(thread(solve, 3, 0, 0));
        threads.emplace_back(thread(solve, 3, subsets[0], 1));
        threads.emplace_back(thread(solve, 3, subsets[1], 2));
        threads.emplace_back(thread(solve, 3, subsets[2], 3));
        threads.emplace_back(thread(solve, 3, subsets[0] | subsets[1], 4));
        threads.emplace_back(thread(solve, 3, subsets[0] | subsets[2], 5));
        threads.emplace_back(thread(solve, 3, subsets[1] | subsets[2], 6));
        threads.emplace_back(thread(solve, 3, subsets[1] | subsets[2] | subsets[0], 7));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    for (int i=0; i<9; i++)
        global_count += count[i];

    cout << global_count << endl;
    return 0;
}
