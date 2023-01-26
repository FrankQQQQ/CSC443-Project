#include<iostream>
using namespace std;


class Node{
    // Node Class
    public:
    int key;
    int val;
    int factor;
    Node * left;
    Node * right;
    Node(int key, int value){
        this->key = key;
        this->val = value;
        this->left = nullptr;
        this->right = nullptr;
        this->factor = -1;
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
    int getNodeNum(Node *root){
        if (root == NULL){
            return 0;
        }
        return (1 + getNodeNum(root->left) + getNodeNum(root->right));
    }

    Node* insert(Node *root, int key, int value){
        Node * node = new Node(key, value);
        if (root == NULL){
            return node;
        }
    }

    Node * l_rotate(Node* root){
        Node * new_root = root->right;
        Node * temp = root->right->left;
        root->right = NULL;
        new_root->left = root;
        new_root->left->right = temp;
        return new_root;

    }

    Node * r_rotate(Node * root){
        Node * new_root = root->left;
        Node * temp = root->left->right;
        root->left = NULL;
        new_root->right = root;
        new_root->right->left = temp;
        return new_root;
    }

    Node * lr_rotate(Node * root){
        Node* new_left = l_rotate(root->left);
        root->left = new_left;
        return r_rotate(root);

    }
    Node * rl_rotate(Node * root){
        Node * new_right = r_rotate(root->right);
        root->right = new_right;
        return l_rotate(root);
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
    Node* root = new Node(1,4);
    Node* a = new Node(2,6);
    Node* b = new Node(3,8);
    root->left = a;
    root->right = b;
    // printTree(root);

    Tree* my_tree = new Tree(root, 100);
    cout << my_tree->getNodeNum(root);
    cout << "\n";
    cout << my_tree->getHeight(root->left);
}