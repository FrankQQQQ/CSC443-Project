#include<iostream>
#include <vector>
#include <string>
#include <queue>
#include "./Memtable.h"
#include <chrono>
#include <ctime>    
#include <fstream>
#include <filesystem>
#include <cmath>
#include "SSTBTreeVersion.h"

int SSTBTreeVersion::maxKeysPerNode() {
    return floor((pageSize - sizeof(bool) - sizeof(int)) / (sizeof(string) + sizeof(int)));
}

BTreeNode* SSTBTreeVersion::buildBTree(vector<KVPair> sortedData, int start, int end) {
    bool isLeaf = (start == end);
    BTreeNode* node = new BTreeNode(isLeaf);

    if (isLeaf) {
        // Create a leaf node with key-value pairs
        for (int i = start; i <= end; i++) {
            node->keys.push_back(sortedData[i].getKey());
            node->childOffsets.push_back(i); // Store the index of the key-value pair in the SST
            node->currentKeyCount++;
        }
    } else {
        int maxKeys = maxKeysPerNode();
        int step = ceil((end - start + 1) / float(maxKeys));
        for (int i = start; i <= end; i += step) {
            BTreeNode* child = buildBTree(sortedData, i, min(i + step - 1, end));
            node->children.push_back(child);
            if (i < end) {
                node->keys.push_back(sortedData[i + step].getKey());
            }
            node->childOffsets.push_back(-1); // We will update this with file offsets later
            node->currentKeyCount++;
        }
    }

    return node;
}

BTreeNode* SSTBTreeVersion::loadChildNode(const string &filename, int offset) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return nullptr;
    }

    // Seek to the correct position in the file
    file.seekg(offset, ios::beg);

    // Read the 'is_leaf' flag
    bool is_leaf;
    file.read(reinterpret_cast<char*>(&is_leaf), sizeof(is_leaf));

    // Create a new Node object and set its 'is_leaf' flag
    BTreeNode *childNode = new BTreeNode(is_leaf);

    // Read the number of keys stored in the node
    int num_keys;
    file.read(reinterpret_cast<char*>(&num_keys), sizeof(num_keys));

    // Read keys
    for (int i = 0; i < num_keys; ++i) {
        int key_size;
        file.read(reinterpret_cast<char*>(&key_size), sizeof(key_size));
        string key(key_size, '\0');
        file.read(&key[0], key_size);
        childNode->keys.push_back(key);
    }

    if (!is_leaf) {
        // Read child offsets for internal nodes
        for (int i = 0; i <= num_keys; ++i) {
            int child_offset;
            file.read(reinterpret_cast<char*>(&child_offset), sizeof(child_offset));
            childNode->childOffsets.push_back(child_offset);
        }
    }

    // Close the file
    file.close();

    return childNode;
}

void SSTBTreeVersion::storeBTree(BTreeNode* node, ofstream& file) {
    if (node == nullptr) {
            return;
    }

    int currentNodeOffset = file.tellp();
    file.write((char*)&(node->isLeaf), sizeof(bool));
    file.write((char*)&(node->currentKeyCount), sizeof(int));

    for (int i = 0; i < node->currentKeyCount; i++) {
        int keySize = node->keys[i].size();
        file.write((char*)&keySize, sizeof(int));
        file.write(node->keys[i].c_str(), keySize);
    }

    if (!node->isLeaf) {
        for (int i = 0; i < node->currentKeyCount + 1; i++) {
            int childOffsetPlaceholder = -1;
            file.write((char*)&childOffsetPlaceholder, sizeof(int));
        }

        for (int i = 0; i < node->currentKeyCount + 1; i++) {
            int childOffset = file.tellp();
            node->childOffsets[i] = childOffset;
            storeBTree(node->children[i], file); // Assuming you have added a vector<BTreeNode*> children to the BTreeNode struct
        }

        // Update the childOffsets in the file
        int currentPosition = file.tellp();
        file.seekp(currentNodeOffset + sizeof(bool) + sizeof(int) + (node->currentKeyCount * (sizeof(int) + sizeof(string))));
        for (int i = 0; i < node->currentKeyCount + 1; i++) {
            file.write((char*)&(node->childOffsets[i]), sizeof(int));
        }
        file.seekp(currentPosition);
    }
}

void SSTBTreeVersion::storeBTreeSST(vector<KVPair>& pairs, string dirname) {
    string filename = "btree.txt";
    string path = "./" + dirname;
    if (!filesystem::exists(path) || !filesystem::is_directory(path)) {
        if (!filesystem::create_directory(path)) {
            cerr << "Failed to create directory " << path << endl;
            return;
        }
    }
    path += "/" + filename;
    ofstream file(path);
    if (file.is_open()) {
        BTreeNode* root = buildBTree(pairs, 0, pairs.size() - 1);
        storeBTree(root, file);
        file.close();
        cout << "B-tree SST created: " << path << endl;
    } else {
        cerr << "Failed to create B-tree SST " << path << endl;
        return;
    }
}

vector<KVPair> SSTBTreeVersion::scanFile(const string& filename, const string& small, const string& large, int lineCount) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return {};
    }
    vector<KVPair> results;
    string line;
    getline(file, line); // skip the header line

    while (getline(file, line)) {
        string key = line.substr(0, line.find(';'));
        string value = line.substr(line.find(';') + 1);

        if (key.compare(small) >= 0 && key.compare(large) <= 0) {
            results.push_back(KVPair(key, value));
        }

        if (key.compare(large) > 0) {
            break;
        }
    }

    file.close();
    return results;
}

string SSTBTreeVersion::binarySearchFile(const string& filename, const string& targetKey, int lineCount) {
    ifstream file(filename);
    if (!file.is_open()) {
        return "File not found!";
    }

    int left = 1;
    int right = lineCount;
    while (left <= right) {
        int middle = left + (right - left) / 2;
        string line;
        getline(file, line);
        file.seekg(0, ios::beg);
        for (int i = 1; i < middle; i++) {
            getline(file, line);
            cout << line;
        }
        string key = line.substr(0, line.find(';'));
        if (key == targetKey) {
            return line.substr(line.find(';') + 1);
        } else if (key.compare(targetKey) < 0) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    return "Key not found!";
}


// int main(){
//     SST my_sst = SST();


        
//     //create memtable, set maximum capacity
//     Memtable* my_tree = new Memtable(NULL, 100);

//     //putKV
//     my_tree->root = my_tree->putKV(my_tree->root, "111111111111111111", "aaaaaaaaaaaaaaaaaa");
//     my_tree->root = my_tree->putKV(my_tree->root, "2", "b");
//     my_tree->root = my_tree->putKV(my_tree->root, "3", "c");
//     my_tree->root = my_tree->putKV(my_tree->root, "4", "d");
//     my_tree->root = my_tree->putKV(my_tree->root, "5", "e");
//     my_tree->root = my_tree->putKV(my_tree->root, "6", "f");
//     my_tree->root = my_tree->putKV(my_tree->root, "7", "g");
//     my_tree->root = my_tree->putKV(my_tree->root, "8", "h");

//     vector<KVPair> pairs = my_tree->purge(my_tree->root);

//     my_sst.store(pairs, "aaa");
//     cout << "\n\n";
//     // cout << my_sst.binarySearchFile("a.txt", "7", 8);
//     // cout << "\n\n";
    


// }