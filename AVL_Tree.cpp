#include<iostream>
using namespace std;
#include <vector>
#include <string>
#include <queue>
#include "KVPair.h"

class Node{
    // Node Class
    public:
    string key;
    string val;
    int height;
    Node * left;
    Node * right;
    Node(string key, string value){
        this->key = key;
        this->val = value;
        this->left = NULL;
        this->right = NULL;
        this->height = 1;
    }
 };

class Memtable{
    public:
    Node * root;
    int size;
    Memtable(Node * my_root, int num){
        this->root = my_root;
        this->size = num;
    }
    int treeHeight(Node* root){
        if (! root){
            return 0;
        }
        else{
            return 1 + max(this->treeHeight(root->left), this->treeHeight(root->right));
        }
    }
    
    int getHeight(Node * p){
        if(p->left && p->right){
            if (p->left->height < p->right->height)
                return p->right->height + 1;
            else return  p->left->height + 1;
            }
            else if(p->left && p->right == NULL){
               return p->left->height + 1;
            }
            else if(p->left ==NULL && p->right){
               return p->right->height + 1;
            }
            return 0;
    }

    int getBalance(Node * root){
        // if(!root || (!root->left && !root->right)){
        //     return 0;
        // }
        if (root->left && root->right){
            return root->left->height - root->right->height;
        }
        else if (root->left && root->right == NULL){
            return root->left->height;
        }
        else if (root->right && root->left == NULL){
            return -root->right->height;
        }
        
    }

    int getNodeNum(Node *root){
        if (root == NULL){
            return 0;
        }
        return (1 + getNodeNum(root->left) + getNodeNum(root->right));
    }



    Node* putKV(Node *root, string key, string value){
        if (getNodeNum(root) >= size){
            // transfer to sst table.
        }
        Node * node = new Node(key, value);
        if (root == NULL){
            root = node;
            return root;
        }
        else{
            if(key.compare(root->key) < 0){
                //insert left
                root->left = putKV(root->left, key, value);
            }
            else if (key.compare(root->key) > 0){
                root->right = putKV(root->right, key, value);
            }
            else{
                cout << "Not support duplicate key at this stage\n";
            }
        }

        root->height = getHeight(root);

        //perform rotation operation
        int bf = getBalance(root);
        if(bf > 1 && key < root->left->key){
            return r_rotate(root);
        }
        else if(bf < -1 && key > root->right->key){
            return l_rotate(root);
        }
        else if(bf > 1 && key > root->left->key){
            root->left = l_rotate(root->left);
            return r_rotate(root);
        }
        else if(bf < -1 && key < root->right->key){
            root->right = r_rotate(root->right);
            return l_rotate(root);
        }        

        return root;
    }

    Node * l_rotate(Node* root){
        Node * x = root->right;
        Node * temp = x->left;
        x->left = root;
        root->right = temp;
        return x;


    }

    Node * r_rotate(Node * root){;
        Node *x = root->left;
        Node * temp = x->right;
        x->right = root;
        root->left = temp;
        return x;
    }

    string getKV(Node* root, string key){
        if (!root){
            return "not found";
        }
        if (root->key.compare(key) == 0){
            // found
            return root->val;
        }
        else if (root->key.compare(key) < 0){
            //to right subtree
            return getKV(root->right, key);
        }
        else{
            return getKV(root->left, key);
        }
    }


    void helperScan(Node *root, string small, string large, vector<string>& result){
        if(!root){
            return;
        }
        helperScan(root->left, small, large, result);
        if (small.compare(root->key) <= 0 && large.compare(root->key) >= 0){
            // cout << "[" + root->key + "," + root->val + "]";
            result.push_back(root->key);
        }
        
        helperScan(root->right, small, large, result); 
    }


    vector<string> scanKV(Node *root, string small, string large){
        vector<string> result;
        this->helperScan(root, small, large, result);
        return result;
    }

   void printTree(Node *v){
        queue < Node *> q;
        Node *cur;
        q.push(v);
        q.push(NULL);      

        while(!q.empty()){
            cur = q.front();
            q.pop();
            if(cur == NULL && q.size()!=0){
                cout<<"\n";
                
                q.push(NULL);
                continue;
            }
            if(cur!=NULL){
                cout<<" "<<cur->key;

                if (cur->left!=NULL){
                q.push(cur->left);
                }
                if (cur->right!=NULL){
                    q.push(cur->right);
                }
            }
            
            
        }
    }
    
    
    
};



    



int main(){
    
    //create memtable, set maximum capacity
    Memtable* my_tree = new Memtable(NULL, 100);

    //putKV
    my_tree->root = my_tree->putKV(my_tree->root, "1", "a");
    my_tree->root = my_tree->putKV(my_tree->root, "2", "b");
    my_tree->root = my_tree->putKV(my_tree->root, "3", "c");
    my_tree->root = my_tree->putKV(my_tree->root, "4", "d");
    my_tree->root = my_tree->putKV(my_tree->root, "5", "e");
    my_tree->root = my_tree->putKV(my_tree->root, "6", "f");
    my_tree->root = my_tree->putKV(my_tree->root, "7", "g");
    my_tree->root = my_tree->putKV(my_tree->root, "8", "h");


    // printTree(root);
    my_tree->printTree(my_tree->root);
    cout << "\n\n";

    //getKV
    cout << my_tree->getKV(my_tree->root, "3");
    cout << "\n\n";


    //scanKV
    vector<string> result = my_tree->scanKV(my_tree->root, "2", "6");
    for (int i = 0; i < result.size(); i++){
        cout << result[i];
    }
    cout << "\n\n";

}