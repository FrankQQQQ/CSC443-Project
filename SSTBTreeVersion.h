#pragma once
#ifndef SSTBTREEVERSION_H
#define SSTBTREEVERSION_H

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <cmath>
#include "./Memtable.h"

using namespace std;

const int pageSize = 4096;

struct BTreeNode {
    vector<string> keys;
    vector<int> childOffsets;
    vector<BTreeNode*> children;
    bool isLeaf;
    int currentKeyCount;

    BTreeNode(bool isLeaf) : isLeaf(isLeaf), currentKeyCount(0) {}
};

class SSTBTreeVersion {
public:
    string path;

    int maxKeysPerNode();
    BTreeNode* buildBTree(vector<KVPair> sortedData, int start, int end);
    BTreeNode* loadChildNode(const string &filename, int offset);
    void storeBTree(BTreeNode* node, ofstream& file);
    void storeBTreeSST(vector<KVPair>& pairs, string dirname);
    vector<KVPair> scanFile(const string& filename, const string& small, const string& large, int lineCount);
    string binarySearchFile(const string& filename, const string& targetKey, int lineCount);
};

#endif // SSTBTREEVERSION_H