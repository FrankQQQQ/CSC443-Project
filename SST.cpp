#include<iostream>
using namespace std;
#include <vector>
#include <string>
#include <queue>
#include "./Memtable.cpp"
#include <chrono>
#include <ctime>    
#include <fstream>

// SortedStringsTableDriver::SortedStringsTableDriver(const string &directory) {
//     auto dirPath = fs::path{directory};
//     if (!fs::exists(dirPath) || !fs::is_directory(dirPath)) {
//         std::cerr << "Invalid directory!" << std::endl;
//         std::exit(-1);
//     }
//     setDirPath(dirPath);
// }

class SST{
    public:
    string path;
    void store(vector<KVPair> vec, string dirname){
        ofstream file("myfile.txt");
        if (file.is_open()) {
            file << vec.size() << "\n";
            for (KVPair pair : vec) {
                file << pair.getKey() <<";"<< pair.getValue() << endl;
        }
        file.close();
        }
    }
    
    string getTime(){
        time_t now = time(nullptr);
        tm local_time = *localtime(&now);
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%m-%d-%H:%M:%S", &local_time);
        return time_str;
    }













};

// void writeVectorToFile(vector<string>& strings, string filename) {
//   ofstream file(filename);
//   if (file.is_open()) {
//     for (auto& str : strings) {
//       file << str << endl; // Write each string followed by a newline character
//     }
//     file.close();
//   }
// }





// }


int main(){
    SST my_sst = SST();
    cout << my_sst.getTime();


        
    //create memtable, set maximum capacity
    Memtable* my_tree = new Memtable(NULL, 100);

    //putKV
    my_tree->root = my_tree->putKV(my_tree->root, "111111111111111111", "aaaaaaaaaaaaaaaaaa");
    my_tree->root = my_tree->putKV(my_tree->root, "2", "b");
    my_tree->root = my_tree->putKV(my_tree->root, "3", "c");
    my_tree->root = my_tree->putKV(my_tree->root, "4", "d");
    my_tree->root = my_tree->putKV(my_tree->root, "5", "e");
    my_tree->root = my_tree->putKV(my_tree->root, "6", "f");
    my_tree->root = my_tree->putKV(my_tree->root, "7", "g");
    my_tree->root = my_tree->putKV(my_tree->root, "8", "h");

    vector<KVPair> pairs = my_tree->purge(my_tree->root);

    my_sst.store(pairs, "aaa");


}