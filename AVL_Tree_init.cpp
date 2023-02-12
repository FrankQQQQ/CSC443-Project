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
            else if (key.compare(root->key) > 0){
                root->right = insert(root->right, key, value);
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

    Node * lr_rotate(Node * root){
        Node *p;
        Node *tp;
        Node *tp2;
        p = root;
        tp = p->left;
        tp2 =p->left->right;

        p -> left = tp2->right;
        tp ->right = tp2->left;
        tp2 ->right = p;
        tp2->left = tp; 
        
        return tp2; 


    }
    Node * rl_rotate(Node * root){
        Node *p;
        Node *tp;
        Node *tp2;
        p = root;
        tp = p->right;
        tp2 =p->right->left;

        p -> right = tp2->left;
        tp ->left = tp2->right;
        tp2 ->left = p;
        tp2->right = tp; 
        
        return tp2; 

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


    void levelOrder(struct Node *v){
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
    my_tree->root = my_tree->insert(my_tree->root, "2", "2");
    my_tree->root = my_tree->insert(my_tree->root, "3", "2");
    my_tree->root = my_tree->insert(my_tree->root, "4", "2");
    my_tree->root = my_tree->insert(my_tree->root, "5", "2");
    my_tree->root = my_tree->insert(my_tree->root, "6", "2");
    my_tree->root = my_tree->insert(my_tree->root, "7", "2");
    // my_tree->root = my_tree->insert(my_tree->root, "8", "2");

    


    // printTree(root);
    levelOrder(my_tree->root);

    cout << "\n";
    // cout << my_tree->getHeight(my_tree->root);
    // cout << my_tree->treeHeight(my_tree->root);
    cout << "\n";
    
    // cout << my_tree->getNodeNum(root);
    
}