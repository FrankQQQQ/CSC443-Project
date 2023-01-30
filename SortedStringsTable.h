#ifndef KV_STORE_SORTEDSTRINGSTABLE_H
#define KV_STORE_SORTEDSTRINGSTABLE_H

#include <string>
#include <vector>
#include "KVPair.h"

using std::string, std::vector;

class SortedStringsTable {
public:

private:
    string directory;
public:
    explicit SortedStringsTable(const string &directory);

    const string &getDirectory() const;
    void setDirectory(const string &directory);
    const string &getKey() const;
    vector<KVPair> scan(const string &key1, const string &key2);
    void storeToDirectory(const vector<KVPair> &kvPairs);

};


#endif //KV_STORE_SORTEDSTRINGSTABLE_H
