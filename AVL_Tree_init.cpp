#include<iostream>
using namespace std;


class Node{
    // Node Class
    public:
    int val;
    Node * left;
    Node * right;
    Node(int value){
        this->val = value;
        this->left = nullptr;
        this->right = nullptr;
    }
 };

class Tree{
    Node * root;
    int size;
    Tree(Node * my_root, int num){
        this->root = my_root;
        this->size = num;
    }
    
    int getHeight(Node * root){
        if (root == NULL){
            return 0;
        }
        else{
            return 1 + max(getHeight(root->left), getHeight(root->right));
        }
    }
    
};

void printTree(Node* root){
    // In order traversal
        if (root == NULL){
            return;
        }
        printTree(root->left);
        cout << root->val;
        printTree(root->right);
    }

int main(){
 
    Node* root = new Node(1);
    Node* a = new Node(2);
    Node* b = new Node(3);
    root->left = a;
    root->right = b;
    printTree(root);
}