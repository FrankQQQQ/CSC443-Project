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
