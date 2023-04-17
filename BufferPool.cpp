#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <unordered_map>
#include "Memtable.cpp"
using namespace std;

class Frame {
public:
    string page_id;
    string data;
    // Add other necessary metadata here

    Frame(string page_id, string data) {
        this->page_id = page_id;
        this->data = data;
    }
};

class BufferPool {
private:
    int initial_size;
    int max_size;
    int current_size;
    int global_depth;
    vector<list<Frame>> directory;

    // Hashing function
    size_t hash_function(string key) {
        // Implement a suitable hashing function, e.g., MurmurHash or xxHash
    }

    // Method to split a bucket
    void split_bucket(int bucket_index) {
        // 1. Create a new bucket
        // 2. Update the global depth if necessary
        // 3. Rehash and redistribute the pages between the original and new buckets
    }

public:
    BufferPool(int initial_size, int max_size) {
        this->initial_size = initial_size;
        this->max_size = max_size;
        this->current_size = 0;
        this->global_depth = 1;
        this->directory.resize(initial_size);
    }



    // Insert a page into the buffer pool
void insert_page(string page_id, string data) {
    // 1. Calculate the bucket index using the hash function
    size_t bucket_index = hash_function(page_id) % directory.size();

    // 2. Check if the page already exists; if so, update its data
    for (Frame &frame : directory[bucket_index]) {
        if (frame.page_id == page_id) {
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
string search_page(string page_id) {
    // 1. Calculate the bucket index using the hash function
    size_t bucket_index = hash_function(page_id) % directory.size();

    // 2. Search the bucket for the target page
    for (const Frame &frame : directory[bucket_index]) {
        if (frame.page_id == page_id) {
            // 3. If found, return the data; otherwise, return an empty string or an error message
            return frame.data;
        }
    }

    return ""; // Page not found
}

// Delete a page from the buffer pool
void delete_page(string page_id) {
    // 1. Calculate the bucket index using the hash function
    size_t bucket_index = hash_function(page_id) % directory.size();

    // 2. Search the bucket for the target page
    for (auto it = directory[bucket_index].begin(); it != directory[bucket_index].end(); ++it) {
        if (it->page_id == page_id) {
            // 3. If found, remove the page from the bucket
            directory[bucket_index].erase(it);
            current_size--;
            break;
        }
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


int main() {
    // Create a BufferPool instance
    BufferPool bufferPool(4, 8);

    // Create a Memtable instance
    Memtable memtable(nullptr, 100);

    // Add key-value pairs to the Memtable
    memtable.root = memtable.putKV(memtable.root, "1", "a");
    memtable.root = memtable.putKV(memtable.root, "2", "b");
    memtable.root = memtable.putKV(memtable.root, "3", "c");
    memtable.root = memtable.putKV(memtable.root, "4", "d");
    memtable.root = memtable.putKV(memtable.root, "5", "e");
    memtable.root = memtable.putKV(memtable.root, "6", "f");
    memtable.root = memtable.putKV(memtable.root, "7", "g");
    memtable.root = memtable.putKV(memtable.root, "8", "h");

    // Insert pages from the Memtable into the BufferPool
    for (int i = 1; i <= 8; i++) {
        string key = to_string(i);
        string value = memtable.getKV(memtable.root, key);
        bufferPool.insert_page(key, value);
    }

    // Search for pages in the BufferPool
    for (int i = 1; i <= 8; i++) {
        string key = to_string(i);
        string value = bufferPool.search_page(key);
        cout << "Key: " << key << ", Value: " << value << endl;
    }

    // Delete a page from the BufferPool
    bufferPool.delete_page("3");

    // Try to search for the deleted page
    string deleted_page_data = bufferPool.search_page("3");
    cout << "Deleted page data: " << deleted_page_data << endl; // Should be an empty string

    // Grow the directory
    bufferPool.grow_directory();

    // Shrink the directory
    bufferPool.shrink_directory();

    return 0;
}