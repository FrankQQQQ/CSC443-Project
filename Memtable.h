#ifndef KV_STORE_MEMTABLE_H
#define KV_STORE_MEMTABLE_H

#include <string>
using std::string;

string get(string key);
void put(string key, string value);
string* scan(string key1, string key2);
// purge here

#endif //KV_STORE_MEMTABLE_H
