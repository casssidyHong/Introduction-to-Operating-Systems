#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int numCacheLines, k, dataSize;
int set_num, address;


class SimpleCache {
public:
    SimpleCache(int cache_size) : cache(cache_size, make_pair(-1, -1)), cache_misses(0) {}

    void access_memory(int data) {

        bool hit = false;
        for (vector<pair<int, int> >::size_type i=0; i<cache.size(); i++) {
            if (data == cache[i].first) {
                hit = true;
                cache[i].second = -1;
                // cout << "Cache hit for data " << data << endl;
                break;
            }
        }
        if (hit == false) {
            cache_misses ++;
            // cout << "Cache miss for data " << data << endl;
            add_newdata(data);
        }

    }

    void add_newdata(int data) {

        bool full = true;
        int which_line = address%set_num;
        int first_cache = which_line*k;
        int last_cache = first_cache + k;
        for (int i=first_cache; i<last_cache; i++) {
            if (cache[i].first == -1) {
                cache[i].first = data;
                cache[i].second = -1;
                // cout << "cache " << i << " is add by " << data << " t=" << address << endl;
                full = false;
                break;
            }
        }
        if (full == true) {
            replace_lru(data);
        }
    }


    void replace_lru(int data) {

        int max = -1;
        // max = cache[0].second;
        int change = 0;
        int which_line = address%set_num;
        int first_cache = which_line*k;
        int last_cache = first_cache + k;

        // cout << "first_cache: "<< first_cache << " last: " << last_cache << endl;

        for (int i=first_cache; i<last_cache; i++) {
            if (cache[i].second >= max) {
                max = cache[i].second;
                change = i;
            }
        }
        // cout << "cache " << change  << " t=" << address << " is replace by " << data << endl;
        cache[change].first = data;
        cache[change].second = -1;
    }


    void update_accesstime() {
        for (vector<pair<int, int> >::size_type i=0; i<cache.size(); i++) {
            if (cache[i].first != -1)
                cache[i].second++;
            // cout << cache[i].second << " | ";
            // cout << cache[i].first << endl;
        }
    }

    int get_cache_misses() const {
        return cache_misses;
    }

private:
    // data, time
    vector<pair<int, int> > cache;
    int cache_misses;
};

int main() {
    // Read input
    cin >> numCacheLines >> k >> dataSize;

    set_num = numCacheLines/k;

    SimpleCache cache(numCacheLines);

    // Process reference data stream
    for (address = 0; address < dataSize; address++) {
        int data;
        cin >> data;
        cache.update_accesstime();
        cache.access_memory(data);
    }

    // Output total cache misses
    cout << "Total Cache Misses:" << cache.get_cache_misses() << endl;

    return 0;
}
