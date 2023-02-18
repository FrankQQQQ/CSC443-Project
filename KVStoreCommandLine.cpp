#include <iostream>
using std::cin, std::cout, std::string, std::getline, std::endl;
using namespace std;
#include <string>
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
        int len = command.length();
        if (len < 6){
            cout << "not valid";
        }
        if (command.substr(0,5) == "open(" && command[5] == '"' && command.back() == ')' && command[len - 2] == '"'){
            // open a new database here
            string database = command.substr(6, len - 8);
            cout << "Here open a database ";
            //TODO open
        }
        else if (command == "new"){
            
        }
        
        
    }
    return 0;
}
