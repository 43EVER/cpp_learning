#include <iostream>
#include <algorithm>
#include <deque>
using namespace std;

struct Node {
    int flag; // 0 2-node, 1 3-node, 2 4-node
    int a, b, c;
    Node* left;
    Node* middle;
    Node* right;

    Node(int a, Node* left = NULL, Node* right = NULL) : 
        flag(0), left(left), right(right), a(a) {}
    Node(int a, int b, Node* left = NULL, Node* right = NULL, Node* middle = NULL) :
        flag(1), left(left), right(right), middle(middle), a(a), b(b) {}

    bool operator== (const int& val)
    {
        if (flag == 0) return val == a;
        return (val == a) || (val == b);
    }
};

Node* search(Node* tree, const int& val)
{
    if (!tree) return tree;

    if (tree->flag == 0) {
        Node* res = NULL;
        if (val == tree->a) res = tree;
        else if (val < tree->a) res = search(tree->left, val);
        else res = search(tree->right, val);
        return res ? res : tree;
    }

    Node* res = NULL;
    if (val == tree->a || val == tree->b) res = tree;
    else if (val < tree->a) res = search(tree->left, val);
    else if (val > tree->b) res = search(tree->right, val);
    else res = search(tree->middle, val);
    return res;
}

Node* insert(Node* tree, const int& val)
{
    
}

int main()
{

}