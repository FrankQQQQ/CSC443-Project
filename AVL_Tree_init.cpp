#include<iostream>
using namespace std;
#include <vector>
#include <string>
#include <queue>

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

    int getBalance(Node * root){
        if(!root || (!root->left && !root->right)){
            return 0;
        }
        else if (root->left && root->right == NULL){
            return root->left->height;
        }
        else if (root->right && root->left == NULL){
            return -root->right->height;
        }
        else{
            return root->left - root->right;
        }

    }

    int getNodeNum(Node *root){
        if (root == NULL){
            return 0;
        }
        return (1 + getNodeNum(root->left) + getNodeNum(root->right));
    }

    Node* insert(Node *root, string key, string value){
        Node * node = new Node(key, value);
        if (root == NULL){
            root = node;
            return root;
        }
        else{
            if(key.compare(root->key) < 0){
                //insert left
                root->left = insert(root->left, key, value);
            }
            else{
                root->right = insert(root->right, key, value);
            }
        }

        root->height = getHeight(root);

        //perform rotation operation
        // if(getBalance(root)==2 && getBalance(root->left)==1){
        //     root = l_rotate(root);
        // }
        // else if(getBalance(root)==-2 && getBalance(root->right)==-1){
        //     root = r_rotate(root);
        // }
        // else if(getBalance(root)==-2 && getBalance(root->right)==1){
        //     root = rl_rotate(root);
        // }
        // else if(getBalance(root)==2 && getBalance(root->left)==-1){
        //     root = lr_rotate(root);
        // }        

        return root;
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
        cout << ("[" + root->key +": "+ root->val + "]");
        printTree(root->right);
    }


vector<vector<Node*>> levelOrder(Node * root){
    // cout << "start";
    vector<Node*> q;
    q.push_back(root);
    vector<vector<Node*>> result;
    while (! q.empty()){
        vector<Node*> k;
        result.push_back(k);
        for (int i = 0; i < q.size(); i += 1){
            Node * cur = q[0];
            q.erase(q.begin());
            result[result.size() - 1].push_back(cur);
            if (cur->left != NULL){
                q.push_back(cur->left);
            }
            if (cur->right != NULL){
                q.push_back(cur->right);
            }
            
        }
    }
    for (int i = 0; i < result.size(); i += 1){
        
        for (int j = 0; j < result[i].size(); j += 1){
            cout << result[i][j]->key;
        }
        cout << "\n";

    }
    return result;
}

    void levelorder_newline(struct Node *v){
        queue <struct Node *> q;
        struct Node *cur;
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



int main(){
    Node* root = new Node("1","4");

    

    Tree* my_tree = new Tree(root, 100);
    my_tree->insert(root, "2", "2");
    my_tree->insert(root, "1", "2");
    my_tree->insert(root, "7", "2");
    my_tree->insert(root, "4", "2");
    my_tree->insert(root, "0", "2");
    my_tree->insert(root, "8", "2");
    printTree(root);
    // levelorder_newline(root);
    // cout << my_tree->getNodeNum(root);
    
}