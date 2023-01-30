#include "SortedStringsTable.h"

const string &SortedStringsTable::getDirectory() const {
    return directory;
}

void SortedStringsTable::setDirectory(const string &directory) {
    SortedStringsTable::directory = directory;
}

SortedStringsTable::SortedStringsTable(const string &directory) {
    setDirectory(directory);
}
