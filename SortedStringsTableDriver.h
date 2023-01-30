#ifndef KV_STORE_SORTEDSTRINGSTABLEDRIVER_H
#define KV_STORE_SORTEDSTRINGSTABLEDRIVER_H

#include <string>
#include <vector>
#include "KVPair.h"

using std::string, std::vector;

class SortedStringsTableDriver {
public:

private:
    string directory;
public:
    explicit SortedStringsTableDriver(const string &directory);

    const string &getDirectory() const;
    void setDirectory(const string &directory);
    const string &getKey() const;
    vector<KVPair> scan(const string &key1, const string &key2) const;
    void storeToDirectory(const vector<KVPair> &kvPairs);

};


#endif //KV_STORE_SORTEDSTRINGSTABLEDRIVER_H
