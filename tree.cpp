// Binary Tree
//   a data structure that is made of nodes, each node has one data memeber,
//   and two children in which one is referred to as the left child, the other
//   as the right child.
//
// Full Binary Tree
//     Has (pow(2, h) - 1) of nodes, h is the height of the tree,
//     every level is full
//
// Completed Binary Tree
//     The binary tree is filled from far left, every level is full except
//     the bottom level on which the nodes at the left.
//     there is no empty node in the array.
//
// Balanced Tree
//   the difference between the height of left subtree and right subtree is 
//   not more than 1.
//
// Binary Serach Tree
//   for each node, the values of the nodes in the left are less or equal, 
//   the values of the nodes in the right are greater.
//
// AVL Tree (Adelson, Velski & Landis)
//    a self-balancing binary search tree. the balance factor (the height difference 
//    between the left subtree and right subtree) is no more than 1. 
//    to balance a BST, the AVL does four kinds of rotations: left, right, left-right
//    and right-left, to make the BST balanced after insertion and deletion.
//    good for applications do not insert or delete frequently but read often.
//    + time complexity O(n)
//    + space complexity O(n)
//
// Heap Tree: see heap.cpp
// Trie Tree: see trie.cpp
//
// Binary Tree Traversal
//   visit each node exactly once in one of orders:
//   + Level Order:
//       - Breadth First Traversal (BFT) 
//       - root->left->right
//       - traverse from left to right at each level, 
//         then move to the next level from top to bottom.
//   + Pre-Order:
//       - Depth First Traversal (DFT)
//       - root->left->right, traverse top-down 
//       - traverse downward to the bottom, then move to the right
//   + In-Order:  
//       - Symmetric Traversal
//       - left->root->right
//       - traverse from the leftmost to the rightmost
//   + Post-Order:
//       - left->right->root
//       - traverse bottom-up
//   + time complexity:  O(n)
//   + space complexity: O(h), h = tree height, between log2(n) and n-1.
//   + applications: 
//       - traverse a tree using preorder is straight forward.
//       - binary search tree usually uses the inorder DFT.
//       - using postorder when free/delete tree nodes. 
//

#include <iostream>
#include <queue>
#include <stack>
#include <climits>

using namespace std;

struct TreeNode {
    long data;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : data(0), left(nullptr), right(nullptr) {}
    TreeNode(long x) : data(x), left(nullptr), right(nullptr) {}
    TreeNode(long x, TreeNode *left, TreeNode *right) : data(x), left(left), right(right) {}
};

/**********************************************************************
 * Binary Tree
 * 
 *********************************************************************/
class BinaryTree {
protected:
    TreeNode *tree_root;
    void free_tree(TreeNode *root);
public:
    BinaryTree() : tree_root(nullptr) {}
    BinaryTree(const vector<long>& v) : tree_root(nullptr) { for (long d : v) { insert(d); }; }
    ~BinaryTree() { free_tree(tree_root); tree_root = nullptr; }

    TreeNode* search(TreeNode* &root, long data);
    TreeNode* search(long data) { return search(tree_root, data); }
    TreeNode* insert(long data);
    TreeNode* remove(long data);
    TreeNode* get_root() const { return tree_root; }

    int  size(TreeNode *root);
    int  size() { return size(tree_root); }
    int  height(TreeNode *root);
    int  height() { return height(tree_root); }
    bool empty() { return tree_root == nullptr; }
    bool balanced(TreeNode *root);
    bool balanced() { return balanced(tree_root); }
    bool completed(TreeNode *root);
    bool completed() { return completed(tree_root); }
    bool is_binary_search_tree(TreeNode *root, long min_data, long max_data);
    bool is_binary_search_tree() { return is_binary_search_tree(tree_root, LONG_MIN, LONG_MAX); };

    virtual long min() { return 0; }
    virtual long max() { return 0; }
    virtual void sort(vector<long>& v) {}

    void print_preorder(TreeNode *root);
    void print_preorder() { print_preorder(tree_root); cout << endl; }
    void print_inorder(TreeNode *root);
    void print_inorder() { print_inorder(tree_root); cout << endl; }
    void print_postorder(TreeNode *root);
    void print_postorder() { print_postorder(tree_root); cout << endl; }
    void print_levelorder(TreeNode *root);
    void print_levelorder() { print_levelorder(tree_root); cout << endl; }
    void print_tree();

    // friend ostream& operator<<(ostream& os, const TreeNode& tnode)  { os << tnode.data; return os; }
};

/* preorder (DFT): root->left->right
 */
void BinaryTree::print_preorder(TreeNode *root)
{
    if (root == nullptr) {
        return;
    }
    cout << root->data << ", ";
    print_preorder(root->left);
    print_preorder(root->right);
}
/* inorder: left->root->right
 */
void BinaryTree::print_inorder(TreeNode *root)
{
    if (root == nullptr) {
        return;
    }
    print_inorder(root->left);
    cout << root->data << ", ";
    print_inorder(root->right);
}
/* postorder: left->right->root
 */
void BinaryTree::print_postorder(TreeNode *root)
{
    if (root == nullptr) {
        return;
    }
    print_postorder(root->left);
    print_postorder(root->right);
    cout << root->data << ", ";
}
/* levelorder (BFT): left->right->next_level
 */
void BinaryTree::print_levelorder(TreeNode *root)
{
    if (root == nullptr) {
        cout << "{}" << endl;
        return;
    }
    queue<TreeNode *> Q;
    Q.push(root);
    while( !Q.empty() ) {
        TreeNode *tnode = Q.front();
        Q.pop();
        cout << tnode->data << ", ";
        if (tnode->left != nullptr) {
            Q.push(tnode->left);
        }
        if (tnode->right != nullptr) {
            Q.push(tnode->right);
        }
    }
}
/* print_tree()
 *   each row includes the nodes at that tree level (depth),
 *   "--" represents a null node.
 */
void BinaryTree::print_tree()
{
    if (tree_root == nullptr) {
        cout << "the tree is empty." << endl;
        return;
    }
    TreeNode null_node(LONG_MIN);
    int count = 0;
    int level = 0;
    int total = size();
    queue<TreeNode *> Q;
    Q.push(tree_root);
    while( !Q.empty() && total)
    {
        TreeNode *tnode = Q.front();
        Q.pop();

        if ((++count & (count - 1)) == 0)   // power of 2
        {
            if (level) {
                cout << endl; cout.width(4); cout << level << ":  ";
            }
            else {
                cout << "root:  ";
            }
            ++level;
        }

        if ( tnode->data == LONG_MIN) {
            cout << "--, ";
        }
        else {
            cout << tnode->data << ", ";
            --total;
        }

        if (tnode->left != nullptr) {
            Q.push(tnode->left);
        }
        else {
            Q.push(&null_node);
        }
        if (tnode->right != nullptr) {
            Q.push(tnode->right);
        }
        else {
            Q.push(&null_node);
        }
    }
    cout << endl;
}
/* free_tree()
 *   free the memory of every node in postorder
 */ 
void BinaryTree::free_tree(TreeNode *root)
{
    if (root == nullptr) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    delete root;
}
/* search()
 *   search the nodes in preorder. 
 *   return the node pointer if found, return nullptr if not found.
 */
TreeNode *BinaryTree::search(TreeNode* &root, long data)
{
    if (root == nullptr) {
        return nullptr;
    }
    if (root->data == data) {
        return root;
    }
    TreeNode *tnode = search(root->left, data);
    if (tnode) {
        return tnode;
    }
    return search(root->right, data);
}
/* insert()
 *   traverse the binary tree in level order (BFT), 
 *   create and add the node when find the an empty node.
 *   the BFT always makes a completed and balanced tree.
 */
TreeNode* BinaryTree::insert(long data)
{
    if (tree_root == nullptr) {
        tree_root = new TreeNode(data);
        return tree_root;
    }
 
    queue<TreeNode *> q;
    q.push(tree_root);

    while ( !q.empty() ) 
    {
        TreeNode* tnode = q.front();
        q.pop();
 
        if (tnode->left != nullptr) {
            q.push(tnode->left);
        }
        else {
            tnode->left = new TreeNode(data);
            return tnode->left;
        }
 
        if (tnode->right != nullptr) {
            q.push(tnode->right);
        }
        else {
            tnode->right = new TreeNode(data);
            return tnode->right;
        }
    }
    return tree_root;
}
/* remove()
 *   find the node, remove it and replace it with the last node
 *   (bottommost and rightmost node).
 */
TreeNode* BinaryTree::remove(long data)
{
    if (tree_root == nullptr) {
        return nullptr;
    }

    TreeNode *tnode = nullptr;
    TreeNode *fnode = nullptr;
    TreeNode *parent = nullptr;
    queue<TreeNode *> Q;
    Q.push(tree_root);

    while (!Q.empty()) 
    { 
        tnode = Q.front(); 
        Q.pop(); 
  
        if (tnode->data == data) {
            fnode = tnode; 
        }
        if (tnode->left) {
            Q.push(tnode->left);
            parent = tnode;
        }
        if (tnode->right) {
            Q.push(tnode->right);
            parent = tnode;
        }
    } 
  
    if (fnode != nullptr && tnode != nullptr) 
    { 
        fnode->data = tnode->data;
        if (parent) {
            parent->left == tnode ? parent->left = nullptr : parent->right = nullptr;
        }
        if (tree_root == tnode) {
            tree_root = nullptr;
        }
        delete tnode;
    }

    return tree_root;
}
/* size()
 *   return the number of the nodes in the binary tree.
 */
int BinaryTree::size(TreeNode *root)
{
    if (root == nullptr) {
        return 0;
    }
    return 1 + size(root->left) + size(root->right);
}
/* get the height (depth) of a node in a binary tree
 *   - it is the height of the tree when the node is the root.
 */
int BinaryTree::height(TreeNode *root) 
{
    if (root == nullptr) {
        return 0;
    }
    return std::max(height(root->left), height(root->right)) + 1;
}
/* check if the binary tree is balanced
 */
bool BinaryTree::balanced(TreeNode *root)
{
    if (root == nullptr) {
        return true;
    }
    int left_height  = height(root->left);  
    int right_height = height(root->right);

    return ((abs(left_height - right_height) <= 1) &&
             balanced(root->left) && balanced(root->right));
}
/* check if the binary tree is a completed tree
 */
bool BinaryTree::completed(TreeNode *root)
{
    if (root == nullptr) {
        return false;
    }

    bool not_full_node = false;
    queue<TreeNode *> Q;
    Q.push(root);

    while (!Q.empty())
    {
        TreeNode *tnode = Q.front();
        Q.pop();

        if (not_full_node && (tnode->left || tnode->right)){
            return false;
        }
        if (tnode->right && tnode->left == nullptr) {
           return false;
        }
        if (tnode->left) {
           Q.push(tnode->left);
        }
        if (tnode->right) {
           Q.push(tnode->right);
        }
        not_full_node = (tnode->left == nullptr || tnode->right == nullptr);
    }
    return true;
}
/* check if a tree is a binary_search_tree (BST)
 *   - an empty tree is a binary search tree.
 */
bool BinaryTree::is_binary_search_tree(TreeNode *root, long min_data, long max_data)
{
	if (root == nullptr) {
		return true;
    }
	if (root->data < min_data || root->data > max_data) {
		return false;
    }
	return is_binary_search_tree(root->left, min_data, root->data) &&
		   is_binary_search_tree(root->right, root->data, max_data);
}

/**********************************************************************
 * Binary Search Tree
 * 
 *********************************************************************/
class BinarySearchTree : public BinaryTree {
public:
    BinarySearchTree() : BinaryTree() {}
    BinarySearchTree(const vector<long>& v) : BinaryTree() { for (long d : v) { insert(tree_root, d); } }
    BinarySearchTree(BinaryTree &btree);

    TreeNode* insert(TreeNode* &root, long data);
    TreeNode* insert(long data) { return insert(tree_root, data); }
    TreeNode* remove(TreeNode* &root, long data);
    TreeNode* remove(long data) { return remove(tree_root, data); }
    TreeNode* search(TreeNode *root, long data);
    TreeNode* search(long data) { return search(tree_root, data); }

    TreeNode* min_node(TreeNode *root);
    TreeNode* min_node() { return min_node(tree_root); }
    TreeNode* max_node(TreeNode *root);
    TreeNode* max_node() { return max_node(tree_root); }
    long min() { return min_node()->data; }
    long max() { return max_node()->data; }
    void sort(TreeNode *root, vector<long>& v);
    void sort(vector<long>& v) { sort(tree_root, v); }
};

/* constructor:
 *   traverse the binary tree inorder iteratively using stack,
 *   insert nodes into the binary search tree using the data 
 *   from the binary tree.
 */
BinarySearchTree::BinarySearchTree(BinaryTree &btree)
    : BinaryTree()
{ 
    if (btree.empty()) {
        return;
    }
    
    stack<TreeNode*> S;
	TreeNode *tnode = btree.get_root();

	while (!S.empty() || tnode != nullptr)
	{
		if (tnode != nullptr)
		{
			S.push(tnode);
			tnode = tnode->left;
		}
		else
		{
			tnode = S.top(); 
			S.pop();
            insert(tnode->data);
			tnode = tnode->right; 
		}
	}
}
/* insert()
 *   - add the node at where a null node is found.
 *   - add to the left when the data is less than the current node.
 *   - add to the right when the data is greater than the current node.
 *   the tree may be not balanced, not completed.
 */
TreeNode *BinarySearchTree::insert(TreeNode* &root, long data)
{
    if(root == nullptr) {
        root = new TreeNode(data);
    }
    else if (data < root->data) {
        insert(root->left, data);
    }
    else {
        insert(root->right, data);
    }
    return root;
}
/* remove()
 *   delete a node and keep the tree as binary search tree.
 *   - delete the node when it has no children.
 *   - swap with the right child when has no left child.
 *   - swap with the left child when has no right child.
 *   - swap with the min node on its right when has both children,
 *     then remove the min node which should be a leaf node.
 */ 
TreeNode* BinarySearchTree::remove(TreeNode* &root, long data)
{
    if (root == nullptr) {
        return nullptr;
    }
    else if (data < root->data) {
        remove(root->left, data);
    }
    else if (data > root->data) {
        remove(root->right, data);
    }
    else {  // found the node
        if (root->left == nullptr) {
            TreeNode *tnode = root;
            root = root->right;
            delete tnode;
        }
        else if (root->right == nullptr) {
            TreeNode *tnode = root;
            root = root->left;
            delete tnode;
        }
        else {
            TreeNode *tnode = min_node(root->right);
            root->data = tnode->data;
            remove(root->right, tnode->data);
        }
    }
    return root;
}
/* search()
 *   - search the node in preorder, return the node when found.
 *   - go to the left when data is less than current node.
 *   - go to right when data is greater than the current node.
 *   time complexity: O(height)
 */
TreeNode* BinarySearchTree::search(TreeNode *root, long data)
{
    if (root == nullptr) {
        return NULL;
    }
    if (root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    }
    else {
        return search(root->right, data);
    }
}
/* sort()
 *   traverse the binary search tree in inorder,
 *   and save its nodes into a vector.
 */
void BinarySearchTree::sort(TreeNode *root, vector<long>& v)
{
    if (root) {
        sort(root->left, v);
        v.push_back(root->data);
        sort(root->right, v);
    }
}
/* min_node()
 *   going all the way to the left to get the node 
 *   which has the minimum value in the binary search tree.
 */
TreeNode* BinarySearchTree::min_node(TreeNode *root)
{
    TreeNode *tnode = root;
    if (tnode == nullptr) {
        return nullptr;
    }
    while (tnode->left) {
        tnode = tnode->left;
    }
    return tnode;
}
/* max_node()
 *   going all the way to the right to get the node 
 *   which has the maximum value in the binary search tree. 
 */
TreeNode* BinarySearchTree::max_node(TreeNode *root)
{
    TreeNode *tnode = root;
    if (tnode == nullptr) {
        return nullptr;
    }
    while (tnode->right) {
        tnode = tnode->right;
    }
    return tnode;
}

/**********************************************************************
 * AVL Tree
 * 
 *********************************************************************/
class AVLTree : public BinarySearchTree {
    TreeNode* build_avl_tree(TreeNode* &root, vector<long>& v, int start, int end);
    TreeNode* left_rotate(TreeNode* &X);
    TreeNode* right_rotate(TreeNode* &X);
public:
    AVLTree() : BinarySearchTree() {}
    AVLTree(vector<long>& v) : BinarySearchTree() { for(long d : v) { insert(tree_root, d); } }
    AVLTree(BinarySearchTree& bst);

    TreeNode* insert(TreeNode* &root, long data);
    TreeNode* insert(long data) { return insert(tree_root, data); }
    TreeNode* remove(TreeNode* &root, long data); 
    TreeNode* remove(long data) { return remove(tree_root, data); }
};

/* constructor:
 *   create an AVL tree from a binary search tree.
 *   - generate the sorted vector from the binary search tree.
 *   - build the AVL tree starting at the middle of the vector.
 */
AVLTree::AVLTree(BinarySearchTree& bst)
    : BinarySearchTree()
{
    vector<long> v;
    bst.sort(v);
    build_avl_tree(tree_root, v, 0, v.size() - 1);
}
/* build an avl tree from an sorted AVL tree.
 *   - find the middle index of the sorted vector.
 *   - create the (root) node using the data at the middle.
 *   - make the left tree using the partial vector before the middle.
 *   - make the right tree using the partial vector after the middle.
 */
TreeNode* AVLTree::build_avl_tree(TreeNode* &root, vector<long>& v, int start, int end)
{
    if (start > end) {
        return nullptr;
    }
    int mid = (start + end) >> 1;
    if (root == nullptr) {
        root = new TreeNode(v[mid]);
    }
    build_avl_tree(root->left, v, start, mid - 1);
    build_avl_tree(root->right, v, mid + 1, end);
    return root;
}
/* left_rotate()
 *       X             Y
 *      / \           / \
 *     A   Y   ==>   X   B
 *        / \       / \
 *       Z   B     A   Z
 */
TreeNode* AVLTree::left_rotate(TreeNode* &X)
{
    TreeNode* Y = X->right;
    TreeNode* Z = Y->left;
    Y->left = X;
    X->right = Z;
    return Y;
}
/* right_rotate()
 *        X           Y
 *       / \         / \
 *      Y   B  ==>  A   X
 *     / \             / \
 *    A   Z           Z   B
 */
TreeNode* AVLTree::right_rotate(TreeNode* &X)
{
    TreeNode* Y = X->left;
    TreeNode* Z = Y->right;
    Y->right = X;
    X->left = Z;
    return Y;  
}
/* insert()
 *   + insert the node in the same way as the binary search tree.
 *   + rebalance the nodes when the absolute value of the balance factor
 *     is greater than 1 by doing left and right rotations.
 *     - left.left:  right rotate
 *     - left.right:  left rotate + right rotate
 *     - right.right: left rotate
 *     - right.left: right rotate + left rotate
 */
TreeNode* AVLTree::insert(TreeNode* &root, long data)
{
    if (root == nullptr) {
        root = new TreeNode(data);
        return root;  
    }
    if (data < root->data) {
        insert(root->left, data);
    }
    else if (data > root->data) {
        insert(root->right, data);
    }
    else {
        return root;
    }

    int balance_factor = height(root->left) - height(root->right);
    if (balance_factor > 1) {
        if (data < root->left->data) {  // left-left
            return root = right_rotate(root);
        } 
        else if (data > root->left->data) {  // left-right
            root->left = left_rotate(root->left);
            return root = right_rotate(root);
        }
    }
    else 
    if (balance_factor < -1) {
        if (data > root->right->data) {  // right-right
            return root = left_rotate(root);
        } 
        else if (data < root->right->data) {  // right-left
            root->right = right_rotate(root->right);
            return root = left_rotate(root);
        }
    }

    return root;
}
/* remove()
 *   traverse the tree in postorder, when the node is found:
 *   + remove the node the same way as the binary search tree.
 *   + rebalance the tree when the 
 */
TreeNode* AVLTree::remove(TreeNode* &root, long data)
{
    if (root == nullptr) {
        return nullptr;
    }
    if (data < root->data) {
        root->left = remove(root->left, data);
    }
    else if (data > root->data) {
        root->right = remove(root->right, data);
    }
    else {
        if (root->left == nullptr) {
            TreeNode *tnode = root->right;
            delete root;
            root = tnode;
        }
        else if (root->right == nullptr) {
            TreeNode *tnode = root->left;
            delete root;
            root = tnode;
        } 
        else {
            TreeNode *tnode = min_node(root->right);
            root->data = tnode->data;
            root->right = remove(root->right, tnode->data);
        }
    }

    if (root == nullptr) {
        return root;
    }
    
    int balance_factor = height(root->left) - height(root->right);
    if (balance_factor > 1) {
        if ((root->left) && height(root->left->left) > height(root->left->right)) {
            return root = right_rotate(root);
        } else {
            root->left = left_rotate(root->left);
            return root = right_rotate(root);
        }
    }
    else
    if (balance_factor < -1) {
        if ((root->right) && height(root->right->left) < height(root->right->right)) {
            return root = left_rotate(root);
        } else {
            root->right = right_rotate(root->right);
            return root = left_rotate(root);
        }
    }
    return root;
}

/* testing driver code
 */
#define TESTING_TREE(tclass, t, v) \
{ \
    cout << "\e[1m" << #tclass << "\e[0m" << ": " << endl; \
    cout << "Vector[" << n << "] = "; for (long d : V) { cout << d << ", "; } cout << endl; \
    tclass t(v); \
    cout << "Preorder:    ";  t.print_preorder(); \
    cout << "Inorder:     ";  t.print_inorder(); \
    cout << "Postorder:   ";  t.print_postorder(); \
    cout << "Levelorder:  ";  t.print_levelorder(); \
    cout << "Tree Size: " << t.size() << ", "; \
    cout << "Tree Heigth: " << t.height() << endl; \
    t.print_tree(); \
    cout << #tclass << " Empty ? ";  t.empty() ? cout << "Yes" : cout << "No"; cout << endl; \
    cout << #tclass << " Balanced ? ";  t.balanced() ? cout << "Yes" : cout << "No"; cout << endl; \
    cout << #tclass << " Completed ? ";  t.completed() ? cout << "Yes" : cout << "No"; cout << endl; \
    cout << #tclass << " Searchable ? "; t.is_binary_search_tree() ? cout << "Yes" : cout << "No"; cout << endl; \
    if (t.is_binary_search_tree()) { \
        cout << "Minimum Data: " << t.min(); cout << endl; \
        cout << "Maximum Data: " << t.max(); cout << endl; \
        cout << "Sort: "; vector<long> s; t.sort(s); for (long d : s) { cout << d << ", "; } cout << endl; } \
    long x = -1; \
    cout << "Search Node: " << x; t.search(x) ? cout << " is found." : cout << " is not found."; \
    cout << "  Node: " << v[n/2]; t.search(v[n/2]) ? cout << " is found." << endl : cout << " is not found." << endl; \
    cout << "Remove Node: " << x << ", "; t.remove(x); \
    for (long d : v) { t.remove(d); cout << d << ", "; }  cout << endl; \
    cout << #tclass << " Empty ? ";  t.empty() ? cout << "Yes" : cout << "No"; cout << endl; \
}
/* main()
 *   may take one argument as the number of nodes in the tree, 
 *   or the number of elements in the vector.
 */
int main(int argc, char *argv[])
{   
    int n = 8;
    if (argc > 1) {
        if ((n = atoi(argv[1])) < 0){
            return -1;
        }
    }

    vector<long> V(n);
    for(int i = 0; i < n; ++i) { V[i] = i + 1; }
    TESTING_TREE(BinaryTree, bt, V);

    for(int i = 0; i < n; ++i) { V[i] = rand() % (n * 10); }
    TESTING_TREE(BinarySearchTree, bst, V);
    BinaryTree bt2(V);
    cout << "BinaryTree: "; bt2.print_inorder();
    // bt2.print_tree();
    BinarySearchTree bst2(bt2);
    cout << "BinarySearchTree: "; bst2.print_inorder();
    // bst2.print_tree();

    TESTING_TREE(AVLTree, avl, V);
    BinarySearchTree bst3(V);
    cout << "BinarySearchTree: "; bst3.print_levelorder();
    // bst3.print_tree();
    AVLTree avl3(bst3);
    cout << "AVLTree: "; avl3.print_levelorder();
    // avl3.print_tree();

    return 0;
}