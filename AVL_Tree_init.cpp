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
    public:
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
    Node * insert(Node* newNode){
        if(this->root == NULL){
            this->root = newNode;
            return newNode;
        }
        else{
            if (newNode->val < this->root->val){
                return this->root->left.insert(newNode);
            }
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
    // printTree(root);

    Tree* my_tree = new Tree(root, 100);
    printTree(root);
    cout << "\n";
    cout << my_tree->getHeight(root->left);
}