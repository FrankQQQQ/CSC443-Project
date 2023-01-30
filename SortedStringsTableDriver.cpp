#include "SortedStringsTableDriver.h"

const string &SortedStringsTableDriver::getDirectory() const {
    return directory;
}

void SortedStringsTableDriver::setDirectory(const string &directory) {
    SortedStringsTableDriver::directory = directory;
}

SortedStringsTableDriver::SortedStringsTableDriver(const string &directory) {
    setDirectory(directory);
}
