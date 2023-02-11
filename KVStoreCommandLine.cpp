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
    while (true) {
        cout << "Please type a command:" << endl;
        string command;
        getline(cin, command);
        cout << "You typed: " << command << endl;
    }
    return 0;
}
