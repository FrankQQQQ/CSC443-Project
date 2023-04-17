#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include <ctime>
#include <chrono>
#include "Memtable.cpp"
#include "MurHash.h"
using namespace std;
using namespace std::chrono;

enum class EvictionPolicy {
    LRU,
    Clock
};


class Frame {
public:
    string page_id;
    string data;
    system_clock::time_point last_access_time;
    bool clock_bit;

    Frame(string page_id, string data)
        : page_id(move(page_id)), data(move(data)), last_access_time(system_clock::now()), clock_bit(false) {}
};


class BufferPool {
private:
    int initial_size;
    int max_size;
    int current_size;
    int global_depth;
    EvictionPolicy eviction_policy;
    vector<list<Frame>> directory;
    int clock_hand;

    // Hashing function
    uint32_t simple_hash(const void* data, int len) {
        const char* key = reinterpret_cast<const char*>(data);
        uint32_t hash = 5381;
        int c;

        while (len--) {
            c = *key++;
            hash = ((hash << 5) + hash) ^ c; // hash * 33 ^ c
        }

        return hash;
    }


    size_t hash_function(const string& key) {
        uint32_t hash = simple_hash(key.data(), static_cast<int>(key.size()));
        return static_cast<size_t>(hash);
    }

    // The rest of the BufferPool implementation remains the same as in the original code, except for the following methods:

public:
    BufferPool(int initial_size, int max_size, EvictionPolicy eviction_policy)
        : initial_size(initial_size),
          max_size(max_size),
          current_size(0),
          global_depth(1),
          eviction_policy(eviction_policy),
          directory(initial_size),
          clock_hand(0) {}

    // The insert_page and delete_page methods remain the same as in the original code.

    // Change the maximum size of the directory
    void set_max_size(int new_max_size) {
        if (new_max_size < current_size) {
            evict_pages(current_size - new_max_size);
        }
        max_size = new_max_size;
        shrink_directory();
    }

    // Evict pages from the buffer pool
    void evict_pages(int num_pages) {
        if (eviction_policy == EvictionPolicy::LRU) {
            evict_pages_lru(num_pages);
        } else if (eviction_policy == EvictionPolicy::Clock) {
            evict_pages_clock(num_pages);
        }
    }

    // Evict pages using the LRU policy
void evict_pages_lru(int num_pages) {
    for (int i = 0; i < num_pages; i++) {
        system_clock::time_point least_recent_time = system_clock::now();
        int least_recent_bucket_index = -1;
        list<Frame>::iterator least_recent_frame_it;

        for (int j = 0; j < directory.size(); j++) {
            for (auto it = directory[j].begin(); it != directory[j].end(); ++it) {
                if (it->last_access_time < least_recent_time) {
                    least_recent_time = it->last_access_time;
                    least_recent_bucket_index = j;
                    least_recent_frame_it = it;
                }
            }
        }

        if (least_recent_bucket_index != -1) {
            directory[least_recent_bucket_index].erase(least_recent_frame_it);
            current_size--;
        }
    }
}


    void split_bucket(int bucket_index) {
        // 1. Create a new bucket
        list<Frame> new_bucket;

        // 2. Update the global depth if necessary
        if (bucket_index >= (1 << global_depth)) {
            global_depth++;
            directory.resize(1 << global_depth);
        }

        // 3. Rehash and redistribute the pages between the original and new buckets
        for (auto it = directory[bucket_index].begin(); it != directory[bucket_index].end();) {
            size_t new_bucket_index = hash_function(it->page_id) % directory.size();
            if (new_bucket_index != static_cast<size_t>(bucket_index)) {
                new_bucket.push_back(*it);
                it = directory[bucket_index].erase(it);
            } else {
                ++it;
            }
        }

        // Insert the new bucket into the directory
        directory.push_back(new_bucket);
    }


// Evict pages using the Clock policy
void evict_pages_clock(int num_pages) {
    int evicted_pages = 0;
    while (evicted_pages < num_pages) {
        for (int i = 0; i < directory.size() && evicted_pages < num_pages; i++) {
            for (auto it = directory[i].begin(); it != directory[i].end();) {
                if (clock_hand == i) {
                    if (it->clock_bit) {
                        it->clock_bit = false;
                    } else {
                        it = directory[i].erase(it);
                        current_size--;
                        evicted_pages++;
                        continue;
                    }
                }
                ++it;
            }
            clock_hand = (clock_hand + 1) % directory.size();
        }
    }
}

// Get a page from the buffer pool; if not found, load it from the storage and insert it
string get_page(const string& page_id, Memtable& memtable) {
    size_t bucket_index = hash_function(page_id) % directory.size();
    auto frame_it = search_page(page_id);
    
    if (frame_it == directory[bucket_index].end()) {
        string data = memtable.getKV(memtable.root, page_id);
        if (current_size == max_size) {
            evict_pages(1);
        }
        insert_page(page_id, data);
        return data;
    } else {
        frame_it->last_access_time = system_clock::now();
        return frame_it->data;
    }
}


    // Insert a page into the buffer pool
void insert_page(string page_id, string data) {
    // 1. Calculate the bucket index using the hash function
    size_t bucket_index = hash_function(page_id) % directory.size();

    // 2. Check if the page already exists; if so, update its data
    for (Frame &frame : directory[bucket_index]) {
        auto frame_it = search_page(page_id);
        if (frame_it == directory[bucket_index].end())
         {
            frame.data = data;
            return;
        }
    }

    // 3. If the bucket is full, split the bucket and rehash
    if (directory[bucket_index].size() >= max_size) {
        split_bucket(bucket_index);
        bucket_index = hash_function(page_id) % directory.size();
    }

    // 4. Insert the page into the appropriate bucket
    directory[bucket_index].push_back(Frame(page_id, data));
    current_size++;
}


// Search for a page in the buffer pool
list<Frame>::iterator search_page(const string& page_id) {
    size_t bucket_index = hash_function(page_id) % directory.size();
    for (auto it = directory[bucket_index].begin(); it != directory[bucket_index].end(); ++it) {
        if (it->page_id == page_id) {
            return it;
        }
    }
    return directory[bucket_index].end();
}


// Delete a page from the buffer pool
void delete_page(const string& page_id) {
    size_t bucket_index = hash_function(page_id) % directory.size();
    auto frame_it = search_page(page_id);
    
    if (frame_it != directory[bucket_index].end()) {
        directory[bucket_index].erase(frame_it);
        current_size--;
    }
}

// Grow the directory
void grow_directory() {
    // 1. Double the directory size
    int new_size = directory.size() * 2;
    directory.resize(new_size);

    // 2. Update the global depth
    global_depth++;

    // 3. Rehash and redistribute the pages between the original and new buckets
    for (int i = 0; i < new_size / 2; i++) {
        list<Frame> temp;

        for (auto it = directory[i].begin(); it != directory[i].end();) {
            size_t new_bucket_index = hash_function(it->page_id) % new_size;

            if (new_bucket_index != i) {
                directory[new_bucket_index].push_back(*it);
                it = directory[i].erase(it);
            } else {
                ++it;
            }
        }
    }
}

// Shrink the directory
void shrink_directory() {
    // 1. Check if the directory can be safely shrunk (e.g., no pages will be lost)
    bool can_shrink = true;
    for (int i = directory.size() / 2; i < directory.size(); i++) {
        if (!directory[i].empty()) {
            can_shrink = false;
            break;
        }
    }

    if (!can_shrink) {
        return;
    }

    // 2. If so, halve the directory size and update the global depth
    int new_size = directory.size() / 2;
    directory.resize(new_size);
    global_depth--;

    // 3. Rehash and redistribute the pages between the original and new buckets
    for (int i = 0; i < new_size; i++) {
        list<Frame> temp;

        for (auto it = directory[i].begin(); it != directory[i].end();) {
            size_t new_bucket_index = hash_function(it->page_id) % new_size;

            if (new_bucket_index != i) {
                directory[new_bucket_index].push_back(*it);
                it = directory[i].erase(it);
            } else {
                ++it;
            }
        }
    }
}
};

