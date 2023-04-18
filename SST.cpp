#include<iostream>
#include <vector>
#include <string>
#include <queue>
#include "./Memtable.h"
#include <chrono>
#include <ctime>    
#include <fstream>
#include <filesystem>
#include "SST.h"

using namespace std;



void SST::store(vector<KVPair> pairs, string dirname, int counter){
        string filename = "sst_" + to_string(counter) + ".txt";
        string path = "./" + dirname;
        if (!filesystem::exists(path) || !filesystem::is_directory(path)) {
            // Create the directory if it does not exist
            if (!filesystem::create_directory(path)) {
                cerr << "Failed to create directory " << path << endl;
                return;
            }
        }
        path += "/" + filename;
        ofstream file(path);
        if (file.is_open()) {
            file << pairs.size() << ";" << pairs[0].getKey() << ";" << pairs[pairs.size() - 1].getKey() << "\n";
            for (KVPair pair : pairs) {
                file << pair.getKey() << ";" << pair.getValue() << endl;
            }
            file.close();
            cout << "File created: " << path << endl;
        } else {
            cerr << "Failed to create file " << path << endl;
            return;
        }
}

string SST::getTime(){
        time_t now = time(nullptr);
        tm local_time = *localtime(&now);
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%m-%d-%H:%M:%S", &local_time);
        
        return string(time_str);
}

vector<KVPair> SST::scanFile(const string& filename, const string& small, const string& large, int lineCount) {
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

string SST::binarySearchFile(const string& filename, const string& targetKey, int lineCount) {
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