#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int numCacheLines, k, dataSize;
int set_num, address;


class SimpleCache {
public:
    SimpleCache(int cache_size) : cache(cache_size, make_pair(-1, 0)), cache_misses(0) {}

    void access_memory(int data) {

        bool hit = false;
        for (vector<pair<int, int> >::size_type i=0; i<cache.size(); i++) {
            if (data == cache[i].first) {
                hit = true;
                cache[i].second ++;
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
                cache[i].second = 1;
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

        int min;
        // max = cache[0].second;
        int change;
        int which_line = address%set_num;
        int first_cache = which_line*k;
        min = cache[first_cache].second;
        change = first_cache;
        int last_cache = first_cache + k;

        // cout << "first_cache: "<< first_cache << " last: " << last_cache << endl;

        for (int i=first_cache+1; i<last_cache; i++) {
            if (cache[i].second < min) {
                min = cache[i].second;
                change = i; 
            }
            // else if (cache[i].second == min) {
            //     if (cache[i].first < cache[change].first) {
            //         min = cache[i].second;
            //         change = i;
            //     }
            // }
        }
        // cout << "cache " << change  << " t=" << address << " is replace by " << data << endl;
        cache[change].first = data;
        cache[change].second = 1;
    }

    int get_cache_misses() const {
        return cache_misses;
    }

private:
    // data, freq
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
        cache.access_memory(data);
    }

    // Output total cache misses
    cout << "Total Cache Misses:" << cache.get_cache_misses() << endl;

    return 0;
}
