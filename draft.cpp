#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class KVPair {
public:
    string key;
    string val;
};

// Comparator function for sorting KVPair vector by key in ascending order
bool cmp(KVPair a, KVPair b) {
    return a.key < b.key;
}

// Function to write a vector of KVPair to a file
void writeKVPairToFile(vector<KVPair> kvpair, string filename) {
    ofstream out(filename);
    for (auto p : kvpair) {
        out << p.key << ";" << p.val << endl;
    }
    out.close();
}

// Function to perform binary search for a key in a file
string binarySearchFile(string filename, string targetKey) {
    ifstream file(filename);
    string line;
    int low = 0, high, middle;
    file.seekg(0, ios::end);
    high = file.tellg();
    while (low < high) {
        middle = (low + high) / 2;
        file.seekg(middle, ios::beg);
        getline(file, line); // Read until the end of the line
        cout << line;
        if (line.empty()) { // End of file reached
            break;
        }
        size_t pos = line.find(';');
        string key = line.substr(0, pos);
        if (key == targetKey) { // Key found
            file.close();
            return line.substr(pos + 1);
        } else if (key < targetKey) { // Key in upper half of file
            low = middle + 1;
        } else { // Key in lower half of file
            high = middle - 1;
        }
    }
    file.close();
    return ""; // Key not found
}

int main() {
    // Example usage
    vector<KVPair> kvpair = {{"1", "ads"}, {"3", "dsad"}, {"4", "wwww"}, {"51", "aaa"}, {"6", "213"}};
    string filename = "kvpair.txt";
    sort(kvpair.begin(), kvpair.end(), cmp);
    writeKVPairToFile(kvpair, filename);
    string targetKey = "4";
    string result = binarySearchFile(filename, targetKey);
    if (result.empty()) {
        cout << "Key not found." << endl;
    } else {
        cout << "Value for key " << targetKey << " is " << result << endl;
    }
    return 0;
}