#include <iostream>
#include <thread>
#include <vector>

using namespace std;

int n, global_count = 0;
const int MAX_THREADS = 8;

bool is_prime(int num) {
    if (num == 1) return false;

    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

void count_primes_in_range(int start, int end, int& local_count) {
    local_count = 0;
    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            local_count++;
        }
    }
}

int main(int argc, char* argv[]) {
    cin >> n;
    int num_threads = 1; // Default to 1 thread

    // Check if the "-t" flag is provided
    if (argc == 3 && string(argv[1]) == "-t") {
        num_threads = stoi(argv[2]);
        if (num_threads < 1 || num_threads > MAX_THREADS) {
            cerr << "Invalid number of threads. Must be between 1 and 8." << endl;
            return 1;
        }
    }

    vector<thread> threads;
    vector<int> local_counts(num_threads, 0);

    // Divide the range of numbers among threads
    int numbers_per_thread = n / num_threads;
    int start = 1;
    int end = numbers_per_thread;

    // Create and launch threads
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(count_primes_in_range, start, end, ref(local_counts[i]));
        start = end + 1;
        end = (i == num_threads - 2) ? n : end + numbers_per_thread;
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Sum up local counts to get the global count
    for (int i = 0; i < num_threads; i++) {
        global_count += local_counts[i];
    }

    cout << global_count << endl;
    return 0;
}
 