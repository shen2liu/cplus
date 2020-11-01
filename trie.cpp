// Trie
//   it is a variant of an m-ary tree data structure, also called prefix tree.
//   it is often used to represent a set of words in which each node stores
//   a letter, a branch include one or more words. A completed word is indicated
//   by a (end-of-word) flag.
//   it has good compromise between running time and memory.
//   it was first recommended by René de la Briandais in 1959.
//

#include <iostream>
#include <string>

using namespace std;

const int   NUMBER_CHILDREN  = 26;  // number of English letters
const int   BASE_LETTER = 'a';      //
const char  ROOT = '@';             // 0x0D, indicate the root of the trie    
const char  END  = '\0';            // 0x20, not-in-use node or end of a word

__inline__ static 
char lower_letter(char c) { if (c <= 'Z') return c + 32; }


class TrieNode {

    typedef  TrieNode*  TrieNodePtr;

public:
    char  letter;
    bool  completed;
    short number_children;
    TrieNodePtr  *children;
 
    TrieNode() : TrieNode(END) { }
    TrieNode(char c) : TrieNode(c, NUMBER_CHILDREN) { }
    TrieNode(char c, short n) : letter(c), completed(false), number_children(n) { 
        children = new TrieNodePtr[n] { nullptr };
        // for (int i = 0; i < n; i++) { children[i] = nullptr; }
    }
};

class TrieTree {
    TrieNode *trie_root;
    void  free(TrieNode **node);
public:
    TrieTree() : trie_root(new TrieNode(ROOT))  { }
    ~TrieTree() { free(&trie_root); }

    void insert(TrieNode *node, string word);
    void insert(string word) { insert(trie_root, word); }
    bool remove(TrieNode **node, string word);
    bool remove(string word) { return remove(&trie_root, word); }
    bool search(TrieNode *node, string word);
    bool search(string word) { return search(trie_root, word); };

    int  children(TrieNode *node);
    bool empty();

    void print(TrieNode *root, int indent);
    void print() { print(trie_root, 0); }
};

/* free()
 * delete the node and all its children (every node below).
 */
void TrieTree::free(TrieNode **node)
{
    if (*node) {
        for (int i = 0; i < (*node)->number_children; ++i) {
            free(&(*node)->children[i]);
        }
        delete[] (*node)->children ;
        delete *node;
        *node = nullptr;
    }
}
/* insert()
 * - allocate the nodes for the word
 * - set the completed flag at the end.
 */ 
void TrieTree::insert(TrieNode *node, string word)
{
    TrieNode **walk = &node;

    for (int i = 0; i < word.size(); i++)
    { 
        int index = lower_letter(word[i]) - BASE_LETTER;
        walk = &(*walk)->children[index];
        if (*walk == nullptr) {
            *walk = new TrieNode(word[i]);
        }
    } 
    (*walk)->completed = true;
}
/* search()
 * - return true if the word is a completed word.
 */
bool TrieTree::search(TrieNode *node, string word)
{
    if (node == nullptr) {
        return false;
    }
    TrieNode *walk = node;

    for (int i = 0; i < word.size(); ++i) {
        int index = lower_letter(word[i]) - BASE_LETTER;
        walk = walk->children[index];
        if ((walk == nullptr) || walk->letter != word[i]) {
            return false;
        }
    }
    return (walk) && walk->completed;
}
/* remove()
 *
 */
bool TrieTree::remove(TrieNode **node, string word)
{
    if (*node == nullptr) {
        return false;
    }
    if ( word.empty() ) 
    {
        if ( children(*node) == 0) {
            delete *node;
            *node = nullptr;
            return true;
        }
        else if ((*node)->completed) {
            (*node)->completed = false;
            return true;
        }
        return false;
    }

    int index = lower_letter(word.front()) - BASE_LETTER;
    if ( node = &(*node)->children[index] ) {
        remove(node, &word[1]);
        if ( children(*node) == 0) {
            delete *node;
            *node = nullptr;
            return true;
        }
    }
    return false;
}
/* children()
 *   return return number of children, return 0 if no children
 */
int TrieTree::children(TrieNode *node)
{
    if (node == nullptr) {
        return 0;
    }
    int count = 0;
    for (int i = 0; i < node->number_children; ++i) {
        if (node->children[i]) {
            ++count;
        }
    }
    return count;
}
/* empty()
 *   return true if the trie is empty.
 */
bool TrieTree::empty()
{
    if (trie_root == nullptr) {
        return true;
    }
    for (int i = 0; i < trie_root->number_children; ++i) {
        if (trie_root->children[i]) {
            return false;
        }
    }
    return true;
}
/* print the Trie tree branches in rows
 * "\" indicates the end of a completed word.
 */
void TrieTree::print(TrieNode *node, int indent = 0)
{
    if (node == nullptr) {
        return;
    }

    cout << node->letter;
    node->completed ? cout << "\\" : cout << "-";

    int c = 0;
    for (int i = 0; i < node->number_children; i++) {
        if (node->children[i]) {
            if (++c > 1) {
                cout << endl;
                for (int j = 0; j <= indent; j++) { cout << "  "; }
            }
            print(node->children[i], indent + 1);
        }
    }
}

/********************************************************************** 
 * the following implementation of TrieWord/TrieSign simplifies 
 * the implementation of TrieTree/TrieNode.
 * - remove the variable 'letter' in TrieNode since: letter = index + 'a'.
 * - remove the variable 'completed' in TrieNode by using an extra
 *   child to indicate the end of a word.
 * - remove the variable 'number_children' in TrieNode and use the 
 *   fixed macro NUMBER_CHILDREN (26) for English only.
 */

const int   TOTAL_CHILDREN   = 27;  // NUMBER_CHILDREN + 1(END)
const int   END_INDEX  = 26;

#define GET_INDEX(c)    (lower_letter(c) - BASE_LETTER)

struct TrieSign {
    TrieSign *children[TOTAL_CHILDREN] = { nullptr };
};

class TrieWord {
    TrieSign *trie_root;
public:
    TrieWord() : trie_root(new TrieSign()) { }

    void insert(string word);
    bool search(string word);
    bool remove(TrieSign **node, string word);
    bool remove(string word) { return remove(&trie_root, word); }
    bool children(TrieSign *node);

    void print(TrieSign *node, int indent);
    void print() { print(trie_root, 0); }
};
//
void TrieWord::insert(string word)
{
    TrieSign **walk = &trie_root;
    for (char c : word)
    { 
        walk = &(*walk)->children[GET_INDEX(c)]; 
        if (!*walk) {
            *walk = new TrieSign();
        }
    } 
    (*walk)->children[END_INDEX] = new TrieSign();
}  
//
bool TrieWord::search(string word)
{
    TrieSign *walk = trie_root;
    for (char c : word) {
        walk = walk->children[GET_INDEX(c)];
        if (!walk) {
            return false;
        }
    }
    return (walk) && (walk->children[END_INDEX]);
}
//
bool TrieWord::remove(TrieSign **node, string word)
{
    if (*node == nullptr) {
        return false;
    }
    if ( word.empty() ) 
    {
        if ( !children(*node) ) {
            delete *node;
            *node = nullptr;
            return true;
        }
        if ((*node)->children[END_INDEX]) {
            delete (*node)->children[END_INDEX];
            (*node)->children[END_INDEX] = nullptr;
            return true;
        }
        return false;
    }

    if ( node = &(*node)->children[GET_INDEX(word[0])] ) {
        remove(node, &word[1]);
        if ( !children(*node) ) {
            delete *node;
            *node = nullptr;
            return true;
        }
    }
    return false;
}
//
bool TrieWord::children(TrieSign *node)
{
    if (node == nullptr) {
        return false;
    }
    for (int i = 0; i < NUMBER_CHILDREN; ++i) {
        if (node->children[i]) {
            return true;
        }
    }
    return false;
}
//
void TrieWord::print(TrieSign *node, int indent = 0)
{
    if (node == nullptr) {
        return;
    }
    if (indent == 0) { cout << '@'; }
    node->children[END_INDEX] ? cout << "\\" : cout << "-";

    int c = 0;
    for (int i = 0; i < NUMBER_CHILDREN; i++) {
        if (node->children[i]) {
            if (++c > 1) {
                cout << endl;
                for (int j = 0; j <= indent; j++) { cout << "  "; }
            }
            cout << char(i + BASE_LETTER);
            print(node->children[i], indent + 1);
        }
    }
}

/* testing main
 */
int main()
{
    // cout << "TrieNode " << sizeof(TrieNode) << ",  TrieTree " << sizeof(TrieTree) << endl;
    // cout << "TrieSign " << sizeof(TrieSign) << ",  TrieWord " << sizeof(TrieWord) << endl;

    TrieTree tt;
    tt.insert("word");    cout << "Add Word: word" << endl;    tt.print(); cout << endl;
    tt.insert("abcd");    cout << "Add Word: abcd" << endl;    tt.print(); cout << endl;
    tt.insert("abef");    cout << "Add Word: abef" << endl;    tt.print(); cout << endl;
    tt.insert("abcdefg"); cout << "Add Word: abcdefg" << endl; tt.print(); cout << endl;
    tt.insert("abcdhij"); cout << "Add Word: abcdhij" << endl; tt.print(); cout << endl;

    // cout << "Free Trie Tree" << endl; tt.~TrieTree(); tt.print(); cout << endl;

    cout << "Search Word: abcd ";    tt.search("abcd") ?    cout << "Yes" : cout << "No"; cout << endl;
    cout << "Search Word: abcdefg "; tt.search("abcdefg") ? cout << "Yes" : cout << "No"; cout << endl;
    cout << "Search Word: abcdef ";  tt.search("abcdef") ?  cout << "Yes" : cout << "No"; cout << endl;

    tt.remove("word");    cout << "Remove Word: word" << endl;;   tt.print(); cout << endl;
    tt.remove("abef");    cout << "Remove Word: abef" << endl;    tt.print(); cout << endl;
    tt.remove("abcd");    cout << "Remove Word: abcd" << endl;    tt.print(); cout << endl;
    tt.remove("abcdef");  cout << "Remove Word: abcdef" << endl;  tt.print(); cout << endl;
    tt.remove("abcdefg"); cout << "Remove Word: abcdefg" << endl; tt.print(); cout << endl;
    tt.remove("abcdhij"); cout << "Remove Word: abcdefg" << endl; tt.print(); cout << endl;

    TrieWord tw;
    tw.insert("word");    cout << "Add Word: word" << endl;    tw.print(); cout << endl;
    tw.insert("abcd");    cout << "Add Word: abcd" << endl;    tw.print(); cout << endl;
    tw.insert("abef");    cout << "Add Word: abef" << endl;    tw.print(); cout << endl;
    tw.insert("abcdefg"); cout << "Add Word: abcdefg" << endl; tw.print(); cout << endl;
    tw.insert("abcdhij"); cout << "Add Word: abcdhij" << endl; tw.print(); cout << endl;

    cout << "Search Word: abcd ";    tw.search("abcd") ?    cout << "Yes" : cout << "No"; cout << endl;
    cout << "Search Word: abcdefg "; tw.search("abcdefg") ? cout << "Yes" : cout << "No"; cout << endl;
    cout << "Search Word: abcdef ";  tw.search("abcdef") ?  cout << "Yes" : cout << "No"; cout << endl;

    tw.remove("word");    cout << "Remove Word: word" << endl;;   tw.print(); cout << endl;
    tw.remove("abef");    cout << "Remove Word: abef" << endl;    tw.print(); cout << endl;
    tw.remove("abcdef");  cout << "Remove Word: abcdef" << endl;  tw.print(); cout << endl;
    tw.remove("abcdefg"); cout << "Remove Word: abcdefg" << endl; tw.print(); cout << endl;
    tw.remove("abcd");    cout << "Remove Word: abcd" << endl;    tw.print(); cout << endl;
    tw.remove("abcdhij"); cout << "Remove Word: abcdhij" << endl; tw.print(); cout << endl;
}