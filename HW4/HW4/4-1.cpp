#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
int cache_size, stream_size;

class SimpleCache {
public:
    SimpleCache(int cache_size) : cache(cache_size, -1), cache_misses(0) {}

    void access_memory(int data, int address) {
        // Check if the data is in the cache
        vector<int>::iterator it = find(cache.begin(), cache.end(), data);

        if (it != cache.end()) {
            // cout << "Cache hit for data " << data << endl;
        } else {
            // cout << "Cache miss for data " << data << endl;
            cache_misses++;

            // If cache is not full, add the data to the cache
            int which_line;
            which_line = address%cache_size;
            cache[which_line] = data;
        }
    }

    int get_cache_misses() const {
        return cache_misses;
    }

private:
    vector<int> cache;
    int cache_misses;
};

int main() {

    cin >> cache_size >> stream_size;

    SimpleCache cache(cache_size);
    // vector<int> datas(stream_size);

    // Process memory accesses
    for (int i = 0; i < stream_size; i++) {
        int data;
        cin >> data;
        // datas.push_back(data);
        cache.access_memory(data, i);
    }

    // Output total cache misses
    cout << "Total Cache Misses:" << cache.get_cache_misses() << endl;

    return 0;
}
