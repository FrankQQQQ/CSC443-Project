#include <iostream>
#include <fstream>
#include <chrono>
#include "SortedStringsTableDriver.h"
namespace fs = std::filesystem;

const auto static SST_FILE_HEAD = "SST_";
const auto static SST_FILE_EXTENSION = ".bin";
const auto static SST_FILE_DELIMITER = ",";

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

const fs::path &formatSstFilePath(const fs::path &dirPath) {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto millis_since_1970 = now_ms.time_since_epoch().count();

    const auto filename = SST_FILE_HEAD + std::to_string(millis_since_1970) + SST_FILE_EXTENSION;
    const auto sstPath = dirPath / filename;
    return sstPath;
}

void SortedStringsTableDriver::storeToSst(const vector<KVPair> &kvPairs) const {
    std::ofstream file;
    file.open(formatSstFilePath(dirPath));
    if (file.is_open()) {
        for (auto pair : kvPairs) {
            file << pair.getKey() << SST_FILE_DELIMITER << pair.getKey() << "\n";
        }
        file << std::endl;
        file.close();
    }
}

// create a binary search that returns pointer to a file

const string &SortedStringsTableDriver::get(const string &key) const {
    // iterate through sst files under the directory
    for (auto const &entry : fs::directory_iterator(dirPath)) {
        auto filePath = entry.path().filename();
        if (fs::is_regular_file(entry.status()) && filePath.extension() == SST_FILE_EXTENSION) {
            std::ifstream file;
            file.open(filePath);
            // TODO: binary search for a key
            if (file.is_open()) {
                string str;
                while (!file.eof()) {
                    getline(file, str);
                    int delimitIdx = str.find(SST_FILE_DELIMITER);
                    if (delimitIdx != std::string::npos) {
                        // if the key exists, close the file and return the value
                        string curKey = str.substr(0, delimitIdx);
                        if (curKey == key) {
                            string value = str.substr(delimitIdx + 1);
                            file.close();
                            return value;
                        }
                    }
                }
            }
            file.close();
        }
    }
    return nullptr;
}