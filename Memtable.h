#ifndef KV_STORE_MEMTABLE_H
#define KV_STORE_MEMTABLE_H

#include <string>
#include <vector>
#include "KVPair.h"
using std::string, std::vector;

class Memtable {
public:
    explicit Memtable(int size);

    string get(string key);
    void put(string key, string value);
    vector<KVPair> scan(string key1, string key2);
    vector<KVPair> purge();
private:
    int size;
};

#endif //KV_STORE_MEMTABLE_H
