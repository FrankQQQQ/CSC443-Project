#ifndef KV_STORE_H
#define KV_STORE_H

#include <string>
#include <vector>
#include "KVPair.h"
using std::string, std::vector;

string get(string key);
void put(string key, string value);
vector<KVPair> scan(string key1, string key2);

#endif //KV_STORE_H
