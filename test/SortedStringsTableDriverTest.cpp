#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"
#include <string>
#include "../SortedStringsTableDriver.h"

const string TEST_DIR = "Testing";
namespace fs = std::filesystem;

void cleanupSSTFiles() {
    auto dirPath = fs::path{TEST_DIR};
    for (auto const &entry : fs::directory_iterator(dirPath)) {
        if (fs::is_regular_file(entry.status()) && entry.path().filename().extension() == SST_FILE_EXTENSION) {
            fs::remove(entry.path());
        }
    }
}

SCENARIO("storeToSST_then_get") {
    cleanupSSTFiles();
    GIVEN("a kvPair") {
        string key = "key";
        string value = "value";
        KVPair kvPair {key, value};
        WHEN("put into sst") {
            SortedStringsTableDriver sstDriver {TEST_DIR};
            std::vector<KVPair> kvPairs {kvPair};
            sstDriver.storeToSst(kvPairs);
            THEN("get should return the correct value of the kvPair") {
                REQUIRE(value == sstDriver.get(key));
            }
        }
    }
    cleanupSSTFiles();
}

SCENARIO("multiple_storeToSST_then_scan") {
    cleanupSSTFiles();
    SortedStringsTableDriver sstDriver {TEST_DIR};
    GIVEN("a 100 kvPairs stored into 10 sst") {
        string key = "key";
        string value = "value";
        for (auto i = 0; i < 10; i++) {
            vector<KVPair> kvPairs;
            for (auto j = i * 10; j < i * 10 + 10; j++) {
                kvPairs.emplace_back(key + std::to_string(j), value + std::to_string(j));
            }
            sstDriver.storeToSst(kvPairs);
        }
        WHEN("scan") {
            vector<KVPair> actual = sstDriver.scan("key0", "key99");
            THEN("should return the kvPairs stored one-to-one in natural order") {
                REQUIRE(actual.size() == 100);
                for (auto i = 0; i < 100; i++) {
                    KVPair expected {key + std::to_string(i), value + std::to_string(i)};
                    auto found = std::find(actual.begin(), actual.end(), expected);
                    REQUIRE(found != actual.end());
                }
            }
        }
    }
    cleanupSSTFiles();
}
