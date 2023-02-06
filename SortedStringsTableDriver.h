#ifndef KV_STORE_SORTEDSTRINGSTABLEDRIVER_H
#define KV_STORE_SORTEDSTRINGSTABLEDRIVER_H

#include <string>
#include <vector>
#include "KVPair.h"

using std::string, std::vector;


const auto static SST_FILE_HEAD = "SST_";
const auto static SST_FILE_EXTENSION = ".bin";
const auto static SST_FILE_DELIMITER = ',';

class SortedStringsTableDriver {
public:
    explicit SortedStringsTableDriver(const string &directory);

    const std::filesystem::path &getDirPath() const;
    void setDirPath(const std::filesystem::path &directory);
    void storeToSst(const vector<KVPair> &kvPairs) const;
    const string get(const string &key) const;
    const vector<KVPair> scan(const string &key1, const string &key2) const;
private:
    std::filesystem::path dirPath;
};


#endif //KV_STORE_SORTEDSTRINGSTABLEDRIVER_H
