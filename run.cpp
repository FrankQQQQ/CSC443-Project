#include <iostream>
using namespace std;
#include <string>
#include <filesystem>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include <algorithm>
#include "run.h"
#include "Memtable.h"
#include "SST.h"

string openFolder(string name) {
    string path = "./" + name;
    if (filesystem::exists(path) && filesystem::is_directory(path)) {
        // Directory already exists, return its path
        return path;
    } else {
        // Create the directory and return its path
        if (filesystem::create_directory(path)) {
            return path;
        } else {
            // Failed to create directory
            return "";
        }
    }
}


string helperStrip(string str){
    str.erase(0, str.find_first_not_of(" \t\n\r\f\v"));  // remove leading spaces
    str.erase(str.find_last_not_of(" \t\n\r\f\v") + 1);  // remove trailing spaces
    str.erase(0, 1);  // remove leading double quote
    str.erase(str.size() - 1);  // remove trailing double quote
    return str;
}

extern string DIR_NAME = "cur";
extern vector<string> sstFiles = vector<string>();

void updateDirName(string dir_name){
    DIR_NAME = dir_name;
    return;

}

void getStartEndKeys(const string& filename, string& startKey, string& endKey) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line);
    size_t firstSeparator = line.find(';');
    size_t secondSeparator = line.find(';', firstSeparator + 1);
    startKey = line.substr(firstSeparator + 1, secondSeparator - firstSeparator - 1);
    endKey = line.substr(secondSeparator + 1);

    file.close();
}

int getLineCount(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file " << filename << endl;
        return 0;
    }

    int count = 0;
    string line;
    while (getline(file, line)) {
        count++;
    }

    file.close();
    return count - 1; // exclude the header line
}


int main() {
    string input;
    // Memtable cur_memtable = 
    Memtable* cur_memtable = new Memtable(NULL, 100);
    
    while (true) {
        cout << "Type a command. \n";
        cout << "Current Dir name is " + DIR_NAME + ".\n";
        getline(cin, input);
        
        if (input.find("open(") == 0 && input[input.length() - 1] == ')') {
            if (input.size() <= 8){
                cout << "invalid\n";
                continue;
                

            }
            string dbname = input.substr(input.find_first_of("\"") + 1, input.find_last_of("\"") - input.find_first_of("\"") - 1);
            cout << "OPEN" << endl;
            cout  << dbname << endl;
            updateDirName(dbname);



        }
        else if (input.find("put(") == 0 && input[input.length() - 1] == ')') {
            if (input.size() <= 10){
                cout << "invalid\n";
                continue;
            }
            string result = input.substr(4, input.size() - 5);
            stringstream ss(result);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() != 2){
                cout << "Put invalid template\n";
                continue;
            }
            cout << "PUT" << endl;
            string key = helperStrip(tokens[0]);
            string value = helperStrip(tokens[1]);
            cout <<"first: " << helperStrip(tokens[0]) << endl;
            cout << "second: " <<helperStrip(tokens[1]) << endl;
            try
            {
                cur_memtable->root = cur_memtable->putKV(cur_memtable->root, key, value);
                cur_memtable->printTree(cur_memtable->root);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
            

        }

        else if (input.find("get(") == 0 && input[input.length() - 1] == ')') {
            if (input.size() <= 7){
                cout << "invalid\n";
                continue;
            }
            string cur_input = input.substr(4, input.size() - 5);
            cout << "GET" << endl;
            string key = helperStrip(cur_input);
            cout << key << endl;
            try
            {
                // Search the memtable
                string result = cur_memtable->getKV(cur_memtable->root, key);
                if (result != "not found") {
                    cout << "The value is " + result + "\n";
                }
                SST sst = SST();

                // Binary search all the SST files
                int left = 0;
                int right = sstFiles.size() - 1;
                while (left <= right) {
                    int middle = left + (right - left) / 2;
                    string startKey, endKey;
                    getStartEndKeys(sstFiles[middle], startKey, endKey);

                    if (key.compare(startKey) >= 0 && key.compare(endKey) <= 0) {
                        // Found the correct SST file
                        int lineCount = getLineCount(sstFiles[middle]);
                        string res = sst.binarySearchFile(sstFiles[middle], key, lineCount);
                        if (res != "Key not found!") {
                            cout << "The value is " + res + "\n";
                        }
                    } else if (key.compare(endKey) > 0) {
                        left = middle + 1;
                    } else {
                        right = middle - 1;
                    }
                }
                cout << "not found \n";
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            


        }
        else if (input.find("scan(") == 0 && input[input.length() - 1] == ')') {
            if (input.size() <= 12){
                cout << "invalid\n";
                continue;
            }
            string result = input.substr(5, input.size() - 6);
            stringstream ss(result);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            cout << "SCAN" << endl;
            string first = helperStrip(tokens[0]);
            string second = helperStrip(tokens[1]);
            cout <<"first: " << first << endl;
            cout << "second: " <<second << endl;

            try
            {   //scanKV
                vector<KVPair> result = cur_memtable->scanKV(cur_memtable->root, first, second);
                for (int i = 0; i < result.size(); i++){
                    cout << result[i].printKVPair();
                }
                // Initialize the SST object to use scanFile function
                SST sst = SST();

                // Step 1: Search the memtable
                vector<KVPair> memtableResults = cur_memtable->scanKV(cur_memtable->root, first, second);

                // Step 2: Perform a binary search to find the starting SST file
                int left = 0;
                int right = sstFiles.size() - 1;
                int startSSTIndex = -1;

                while (left <= right) {
                    int middle = left + (right - left) / 2;
                    string startKey, endKey;
                    getStartEndKeys(sstFiles[middle], startKey, endKey);

                    if (second.compare(startKey) < 0) {
                        right = middle - 1;
                    } else if (first.compare(endKey) > 0) {
                        left = middle + 1;
                    } else {
                        startSSTIndex = middle;
                        break;
                    }
                }

                // Step 3: Scan each SST file in the range
                vector<KVPair> sstResults;
                if (startSSTIndex != -1) {
                    for (int i = startSSTIndex; i < sstFiles.size(); i++) {
                        string startKey, endKey;
                        getStartEndKeys(sstFiles[i], startKey, endKey);
                        if (first.compare(endKey) > 0) {
                            continue;
                        }
                        if (first.compare(startKey) < 0) {
                            break;
                        }
                        int lineCount = getLineCount(sstFiles[i]);
                        vector<KVPair> fileResults = sst.scanFile(sstFiles[i], first, second, lineCount);
                        sstResults.insert(sstResults.end(), fileResults.begin(), fileResults.end());
                    }
                }

                // Step 4: Merge the results from the memtable and SST files
                vector<KVPair> mergedResults;
                size_t memIdx = 0, sstIdx = 0;

                while (memIdx < memtableResults.size() && sstIdx < sstResults.size()) {
                    if (memtableResults[memIdx].getKey().compare(sstResults[sstIdx].getKey()) <= 0) {
                        mergedResults.push_back(memtableResults[memIdx]);
                        memIdx++;
                    } else {
                        mergedResults.push_back(sstResults[sstIdx]);
                        sstIdx++;
                    }
                }

                while (memIdx < memtableResults.size()) {
                    mergedResults.push_back(memtableResults[memIdx]);
                    memIdx++;
                }

                while (sstIdx < sstResults.size()) {
                    mergedResults.push_back(sstResults[sstIdx]);
                    sstIdx++;
                }

                for (int i = 0; i < mergedResults.size(); i++){
                    cout << mergedResults[i].printKVPair();
                }
                cout << "\n\n";

            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            }
            
        }
        else if (input.find("close(") == 0 && input[input.length() - 1] == ')') {
            if (input.size() <= 9){
                cout << "invalid\n";
                continue;
            }
            string result = input.substr(6, input.size() - 7);
            cout << result << endl;
            cout << helperStrip(result) << endl;
            Memtable* cur_tree = new Memtable(NULL, 100);
            if (helperStrip(result).compare(DIR_NAME) == 0){
                updateDirName("");
                cout << "Successfully closed Directory\n";
                continue;
            }
            else{
                cout << "Database is not opened. \n";
                continue;
            }
            
            
        }
        else if(input == "help"){
            cout << "helper" << endl;
        }
        else if (input == "quit"){
            break;
        }
        else {
            cout << "Invalid input" << endl;
            cout << "Type \"help\" to see instructions" << endl;
        }
    }
    
    return 0;
}
