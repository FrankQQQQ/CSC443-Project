#pragma once
#ifndef SST_H
#define SST_H

#include "Memtable.h"

#include<iostream>
#include <vector>
#include <string>
#include <queue>
using namespace std;
using std::string, std::vector;

class SST {
    public:
    string path;
    void store(vector<KVPair> pairs, string dirname, int counter);
    string getTime();
    string binarySearchFile(const string& filename, const string& targetKey, int lineCount);
};


#endif