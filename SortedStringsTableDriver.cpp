#include <iostream>
#include <fstream>
#include <chrono>
#include "SortedStringsTableDriver.h"
namespace fs = std::filesystem;

SortedStringsTableDriver::SortedStringsTableDriver(const string &directory) {
    auto dirPath = fs::path{directory};
    if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
        std::cerr << "Invalid directory!" << std::endl;
        std::exit(-1);
    }
    setDirPath(dirPath);
}

const fs::path &SortedStringsTableDriver::getDirPath() const {
    return dirPath;
}

void SortedStringsTableDriver::setDirPath(const fs::path &dirPath) {
    this->dirPath = dirPath;
}

const fs::path formatSstFilePath(const fs::path &dirPath) {
    auto now = std::chrono::system_clock::now();
    auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
    auto nanos_since_1970 = now_ns.time_since_epoch().count();

    const auto filename = SST_FILE_HEAD + std::to_string(nanos_since_1970) + SST_FILE_EXTENSION;
    const auto sstPath = dirPath / filename;
    return sstPath;
}

void SortedStringsTableDriver::storeToSst(const vector<KVPair> &kvPairs) const {
    std::ofstream file;
    file.open(formatSstFilePath(dirPath));
    if (file.is_open()) {
        for (auto pair : kvPairs) {
            file << pair.getKey() << SST_FILE_DELIMITER << pair.getValue() << "\n";
        }
        file.flush();
        file.close();
    }
}

// create a binary search that returns pointer to a file

const string SortedStringsTableDriver::get(const string &key) const {
    // iterate through sst files under the directory
    for (auto const &entry : fs::directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.status())&& entry.path().filename().extension() == SST_FILE_EXTENSION) {
            std::ifstream file;
            file.open(entry.path());
            // TODO: binary search for a key
            string str;
            while (file.is_open() && !file.eof()) {
                string curKey;
                string curValue;
                getline(file, curKey, SST_FILE_DELIMITER);
                if (curKey.empty()) {
                    continue;
                }
                if (curKey == key) {
                    getline(file, curValue);
                    // if the key exists, close the file and return the value
                    file.close();
                    return curValue;
                }
            }
            file.close();
        }
    }
    return "";
}

const vector<KVPair> SortedStringsTableDriver::scan(const string &key1, const string &key2) const {
    // TODO: Optimization, initialize according to memtable size for
    vector<KVPair> kvPairsInRange;

    // iterate through sst files under the directory
    for (auto const &entry : fs::directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.status())&& entry.path().filename().extension() == SST_FILE_EXTENSION) {
            std::ifstream file;
            file.open(entry.path());
            // TODO: binary search for a key
            string str;
            while (file.is_open() && !file.eof()) {
                string curKey;
                string curValue;
                getline(file, curKey, SST_FILE_DELIMITER);
                if (curKey.empty()) {
                    continue;
                }
                if (key1 <= curKey && curKey <= key2) {
                    getline(file, curValue);
                    // if the key exists, close the file and return the value
                    KVPair kvPair {curKey, curValue};
                    kvPairsInRange.push_back(kvPair);
                }
            }
            file.close();
        }
    }
    // TODO: optimization, use merge sort k-lists
    std::sort(kvPairsInRange.begin(), kvPairsInRange.end());
    return kvPairsInRange;
}
