#include <iostream>
using std::cin, std::cout, std::string, std::getline, std::endl;


/**
 * Print a list of commands available
 */
void printHelp() {

}

/**
 * Open the KVStore
 */
void open() {

}

/**
 * Close the KVStore
 */
void close() {

}

int main() {
//    while (true) {
//        cout << "Please type a command:" << endl;
//        string command;
//        getline(cin, command);
//        cout << "You typed: " << command << endl;
//    }
    for (auto i = 0; i < 10; i++) {
        for (auto j = i * 10; j < i * 10 + 10; j++) {
            cout << std::to_string(j) << endl;
        }
    }
    std::string str1 = "key9";
    std::string str2 = "key90";
    if (str1 < str2) {
        cout << "True" << endl;
    } else {
        cout << "False" << endl;
    }
    return 0;
}
