// Linked List in C/C++
//
// Pro:
// 1) dynamic size
// 2) ease of insertion and deletion
// Con:
// 1) random access is not allowed, no binary search.
// 2) extra space for the pointer in every element.
// 3) not cache friendly, memory is not continous.
//
// linked list types
//   1) singly linked list
//   2) doubly linked list
//   3) cirular linked list
//   *) only implements the singly linked list for now.
//
// linked list sorting
//   - merge sort
//   - insertion sort
//   - quick sort (doubly linked list, not implemented)
//
// linked list searching
//   - linear search only, cannot do binart search
//   - create and use the skip list for a linked list with length of sqrt(n)
//     
// applications:
//   1) implementations of stacks, queues, etc.
//   2) memory buffer managements
//   3) more ...
//
#include <iostream>

using namespace std;

struct SinglyNode {
    int data;
    SinglyNode *next; 
};

/**********************************************************************
 * create a singly linked list from an array.
 * Input:  (long *)a - standard C/C++ array
 *      :    (int)sz - the size (number of elements) of the array
 * Return: the head of the linked list
 */
SinglyNode* singly_linked_list_create(long *a, int sz)
{
    SinglyNode *head;
    SinglyNode **node = &head;
    for (int i = 0; i < sz; ++i) {
        *node = (SinglyNode *)malloc(sizeof(SinglyNode));
        (*node)->data = a[i];
        node = &(*node)->next;
    }
    *node = NULL;
    return head;
}
/**********************************************************************
 * free all the nodes on a singly linked list
 * input:   (SinglyNode **)slist - 
 *      :       the address of the linked list pointer.
 * output:  (SinglyNode *)slist = NULL
 * return:  none
 */
void singly_linked_list_free(SinglyNode **slist)
{
    SinglyNode *next;
    while (*slist) {
        next = (*slist)->next;
        free(*slist);
        *slist = next;
    }
}
/**********************************************************************
 * get the length (number of the nodes) of a sinlgy linked list
 *  Input:  (SinglyNode **)slist - the head pointer of the singly linked list
 * Return:  the length of the linked list 
 */
int singly_linked_list_length(SinglyNode *slist)
{
    // SinglyNode *walk = slist;
    int count = 0;
    while (slist) {
        ++count;
        slist = slist->next;
    }
    return count;
}
/*****************************************************************************
 * create and append a node to the singly linked list
 *  input:  (SinglyNode *)slist - the head pointer of the singly linked list
 *       :          (long)data  - the data of the node to be added
 * output:  (SinglyNode *)slist - will be set if it was NULL
 * return:  the pointer of the tail (or the node just added)
 */
SinglyNode* singly_linked_list_append(SinglyNode **slist, long data)
{
    SinglyNode *node = (SinglyNode *)malloc(sizeof(SinglyNode));
    node->data = data;
    node->next = NULL;

    if ( *slist == NULL ) {
        return *slist = node;
    }

    SinglyNode *walk = *slist;
    while (walk->next) {
        walk = walk->next;
    }
    walk->next = node;
    return node;
}
/*****************************************************************************
 * create and add (push) a node to at the head of the singly linked list
 *  input:  (SinglyNode *)slist - the head pointer of the singly linked list
 *       :        (long *)data  - the data to be added
 * output:  (SinglyNode *slist) - the new head pointer after push
 * return:  the pointer of the head (or the node just added)
 */
SinglyNode* singly_linked_list_push(SinglyNode** slist, long data)
{
    SinglyNode *node = (SinglyNode *)malloc(sizeof(SinglyNode));
    node->data = data;
    node->next = *slist;
    return *slist = node;
}
/*****************************************************************************
 * create and insert a node before the (target) node specified.
 *  input:  (SinglyNode **)slist  - the head pointer of the singly linked list
 *       :           (long)target - the data of the target node
 *       :           (long)data   - the data to be added
 * output:  (SinglyNode **)slist  - the new head pointer when the node is added 
 *                                  before the old head
 * return:  NULL if the targte node is not found
 *       :  the pointer of the node just inserted
 */
SinglyNode* singly_linked_list_insert(SinglyNode **slist, long target, long data)
{
    SinglyNode **walk = slist;
    while ((*walk) && (*walk)->data != target) {
        walk = &(*walk)->next;
    }
    if (*walk == NULL) {
        return NULL;
    }

    SinglyNode *node = (SinglyNode *)malloc(sizeof(SinglyNode));
    node->data = data;
    node->next = *walk;
    *walk = node;
    return node;
}
/*****************************************************************************
 * create and add a node after the (target) node specified.
 *  input:  (SinglyNode *)slist  - the head pointer of the singly linked list
 *       :          (long)target - the data of the target node
 *       :          (long)data   - the data to be added
 * return:  NULL if the target is not found
 *       :  or the pointer of the node just added
 *   note:  the target cannot be found if the slist is NULL.
 */
SinglyNode* singly_linked_list_add(SinglyNode* slist, long target, long data)
{
    SinglyNode *node = NULL;
    while (slist) {
        if (slist->data == target) {
            node = (SinglyNode *)malloc(sizeof(SinglyNode));
            node->data = data;
            node->next = slist->next;
            slist->next = node;
            break;
        }
        slist = slist->next;
    }
    return node;
}
/*****************************************************************************
 * pop out the node at the head from the singly lined list
 *  input:  (SinglyNode **)slist  - the head pointer of the singly linked list
 * output:  (SinglyNode **)slist  - the new head pointer after pop
 * return:  the node pointer that was at the head
 * memory:  the caller needs to free the memory of the node returned
 */
SinglyNode* singly_linked_list_pop(SinglyNode** slist)
{
    SinglyNode *node = *slist;
    if (*slist != NULL) {
        *slist = (*slist)->next;
    }
    return node;
}
/**********************************************************************
 * take out the node at the tail of the singly lined list
 *  input:  (SinglyNode *)slist - the head pointer of the singly linked list
 * return:  the node pointer that was at the tail
 * memory:  the caller needs to free the memory of the node returned
 */
SinglyNode* singly_linked_list_end(SinglyNode **slist)
{
    SinglyNode **tail = slist;
    if (*tail == NULL) {
        return NULL;
    }
    while((*tail)->next) {
        tail = &(*tail)->next;
    }
    SinglyNode *node = *tail;
    *tail = NULL;
    return node;
}
/**********************************************************************
 * remove the node in the singly linked list
 *  input:  (SinglyNode **)slist - the address of the linked list head
 *       :           (long)data  - the data of the node to be removed
 * output:  (SinglyNode **)slist - the new head if the head was removed
 * return:  NULL when the data is not found
 *       :  the pointer of the node that was removed
 * memory:  the caller needs to free the memory of the removed node
 */
SinglyNode *singly_linked_list_remove(SinglyNode **slist, long data)  
{
    SinglyNode *prev = NULL;
    SinglyNode *walk = *slist;

    while ((walk) && (walk->data != data)) {
        prev = walk;
        walk = walk->next;
    }

    if (walk == NULL) {
        return NULL;
    }
    else if (prev == NULL) {
        *slist = (*slist)->next;
    }
    else {
        prev->next = walk->next;
    }
    return walk;
}
/**********************************************************************
 * reverse a singly linked list using the iteration
 * - swap the next pointer with the previous pointer for every node.
 *  input:  (SinglyNode **)slist - the pointer of the linked list
 * return:  the reversed singly linked list
 *   note:  the input slist is not updated.
 */
SinglyNode* singly_linked_list_reverse(SinglyNode *slist)
{
    SinglyNode *prev = NULL;
    SinglyNode *next;
    while(slist) {
        next = slist->next;
        slist->next = prev;
        prev = slist;
        slist = next;
    }
    return prev;
}
/**********************************************************************
 * search a singlylinked list to find the first node that has the data
 *  input:  (SinglyNode **)slist - the address of the linked list head
 * return: NULL if not found
 *       : the pointer of the first node found (and the rest of the list)
 */
SinglyNode* singly_linked_list_search(SinglyNode *slist, int data)
{
    SinglyNode *walk = slist;
    while ((walk) && walk->data != data) {
        walk = walk->next;
    }
    return walk;
}
/**********************************************************************
 * search a singly linked list for all the nodes that have the data
 *  input:  (SinglyNode **)slist - the address of the linked list head
 * return: NULL if not found
 *       : the linked list that includes all the nodes matched
 *   note: not tested yet
 */
SinglyNode* singly_linked_list_search_all(SinglyNode *slist, int data)
{
    SinglyNode *walk = slist;
    SinglyNode *flist = NULL;
    while (walk) {
        if (walk->data == data) {
            SinglyNode *node = (SinglyNode *)malloc(sizeof(SinglyNode));
            node->data = data;
            node->next = NULL;
            flist = node;
            flist = flist->next;
        }
        walk = walk->next;
    }
    return flist;
}
/**********************************************************************
 * merge two sorted singly linked list into one sorted singly linked list
 * - the sort is in ascending order.
 *  Input: (SinglyNode *)list1 - the head pointer of a sorted singly linked list
 *       : (SinglyNode *)list2 - the head pointer of a sorted singly linked list
 * Return: the head pointer of the merged singly linked list
 */
SinglyNode* singly_linked_list_merge(SinglyNode *list1, SinglyNode *list2)
{
    if ( !(list1) ) {
        return list2;
    }
    else if ( !(list2) ) {
        return list1;
    }

    SinglyNode *mlist = NULL;

    if ( list1->data < list2->data ) {
        mlist = list1;
        mlist->next = singly_linked_list_merge(list1->next, list2);
    }
    else {
        mlist = list2;
        mlist->next = singly_linked_list_merge(list1, list2->next);
    }

    return mlist;
}
/**********************************************************************
 * implements the merge sort algorithm to sort a singly linked list
 * - first split the list into two halfs.
 * - sort the two half lists recursively.
 * - merge the two half list back to one list.
 *  input:  (SinglyNode **)slist - the address of the list
 * output:  (SinglyNode **)slist - the new address of the list
 * return:  the head pointer of the list
 */
SinglyNode* singly_linked_list_merge_sort(SinglyNode **slist)
{
    if ( *slist == NULL || (*slist)->next == NULL ) {
        return *slist;
    }

    SinglyNode *list1 = *slist;
    SinglyNode *list2 = (*slist)->next;

    while ((list2) && (list2 = list2->next) ) {
        list2 = list2->next;
        list1 = list1->next;
    }
    list2 = list1->next;
    list1->next = NULL;
    list1 = *slist;

    singly_linked_list_merge_sort( &list1 );
    singly_linked_list_merge_sort( &list2 );
    return *slist = singly_linked_list_merge(list1, list2);
}
/**********************************************************************
 * implements the insertion sort algorithm to sort a singly linked list
 * - take a node from the input singly lined list (slist)
 * - put the node into the sorted singly linked list (head)
 *  input:  (SinglyNode **)slist - the address of the list
 * output:  (SinglyNode **)slist - the new address of the list
 * return:  the head pointer of the list
 */
SinglyNode* singly_linked_list_insertion_sort(SinglyNode **slist)
{
    SinglyNode *head = NULL; 
    SinglyNode *prev = NULL;
    SinglyNode *node = *slist;
    while (node != NULL) 
    {
        SinglyNode *next = node->next;
        SinglyNode *walk = head;

        while (walk && walk->data <= node->data) {
            prev = walk;
            walk = walk->next;
        }

        if (walk == head) {
            node->next = head;
            head = node;
        }
        else {
            prev->next = node;
            node->next = walk;
        }
        prev = node;
        node = next;
    } 
    return *slist = head;
}
/**********************************************************************
 * detect if there is a loop in the singly linked list using 
 * Floyd's cycle detection algorithm.
 * - define two pointers, slow and fast, both start from the beginning.
 * - the slow pointer forwards one node at a time: slow = slow->next.
 *   the fast pointer forwards two nodes at a time: fast = fast->next->next.
 * - the two pointers will meet (equal) if there is a loop (proved by math).
 * 
 *  input:  (SinglyNode *)slist - the head pointer of the singly linked list
 * return:  true  - if the loop is detected
 *          flase - if no loop
 */
bool singly_linked_list_detect_loop(SinglyNode *slist)
{
    if ( !(slist) || !(slist->next) ) {
        return false;
    }
    SinglyNode *slow = slist;
    SinglyNode *fast = slist;

    while (slow && fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (fast == slow ) {
            return true;
        }
    }
    return false;
}
/**********************************************************************
 * print the singly linked list
 * - try detect a loop using c++ set
 * - a pointer is found already in the set when there is a loop
 *  input:  (SinglyuNode *)slist - the head pointer of the list
 * return:  none
 */
#include <set>
void singly_linked_list_print(SinglyNode *slist)
{
    set<SinglyNode *> S;
    SinglyNode *walk = slist;

    long found = 0;
    cout << "[";
    while (walk) {
        cout << walk->data << ", ";
        if ( S.find(walk) != S.end() ) {
            if (found == 0) {
                found = walk->data;
            }
            else {  // print one more
                break;
            }
        }
        S.insert(walk);
        walk = walk->next;
    }
    if (found) {
        cout << ">  this list includes loop at " << found << " !!" << endl;
    }
    else {
        cout << "\\0]" << endl;
    }
}
/**********************************************************************
 * testing display utitilities
 */
void  print_return(SinglyNode *rv)
{
    cout << "return ";
    (rv) ? cout << rv->data : cout << "NULL";
    cout << ", ";
}
/* display and free the node
 */
void  print_free(SinglyNode *rv)
{
    print_return(rv);
    free(rv);
}
/**********************************************************************
 * main()
 * take one argument as the number of the nodes in the list
 */
int main(int argc, char* argv[])
{
    int n = 10;
    if (argc > 1 && (n = atoi(argv[1])) < 0) {
        return -1;
    }
    long a[n];
    for (int i = 0; i < n; ++i) { a[i] = i + 1; }
    long x = a[n/2];
    SinglyNode *head, *tail, *entry;

    cout << "create: ";   head = singly_linked_list_create(a, n);  singly_linked_list_print(head);
    cout << "length: " << singly_linked_list_length(head) << endl;
    cout << "append: ";  entry = singly_linked_list_append(&head, n + 1);   print_return(entry);    singly_linked_list_print(head);
    cout << "end:    ";  entry = singly_linked_list_end(&head);             print_free(entry);      singly_linked_list_print(head);
    cout << "push:   ";  entry = singly_linked_list_push(&head, -1);        print_return(entry);    singly_linked_list_print(head);
    cout << "pop:    ";  entry = singly_linked_list_pop(&head);             print_free(entry);      singly_linked_list_print(head);
    cout << "insert: ";  entry = singly_linked_list_insert(&head, x, -x);   print_return(entry);    singly_linked_list_print(head);
    cout << "insert: ";  entry = singly_linked_list_insert(&head, 0, -1);   print_return(entry);    singly_linked_list_print(head);
    cout << "remove: ";  entry = singly_linked_list_remove(&head, -x);      print_free(entry);      singly_linked_list_print(head);
    cout << "remove: ";  entry = singly_linked_list_remove(&head, 0);       print_free(entry);      singly_linked_list_print(head);
    cout << "add:    ";  entry = singly_linked_list_add(head, x, -x);       print_return(entry);    singly_linked_list_print(head);
    cout << "add:    ";  entry = singly_linked_list_add(head, 0, -1);       print_return(entry);    singly_linked_list_print(head);
    cout << "free:   "; singly_linked_list_free(&head);  singly_linked_list_print(head);

    for (int i = 0; i < n; ++i) { a[i] = rand() % (n * 10); }
    cout << "create:  ";  head = singly_linked_list_create(a, n);           singly_linked_list_print(head);
    cout << "search:  "; entry = singly_linked_list_search(head, a[n/2]);   print_return(entry); cout << endl;
    cout << "search:  "; entry = singly_linked_list_search(head, -99);      print_return(entry); cout << endl;
    cout << "reverse: ";  head = singly_linked_list_reverse(head);          singly_linked_list_print(head);
    cout << "merge sort:     ";  head = singly_linked_list_merge_sort(&head);  singly_linked_list_print(head);

    cout << "create list:    ";  head = singly_linked_list_create(a, n);    singly_linked_list_print(head);
    cout << "insertion sort: ";  head = singly_linked_list_insertion_sort(&head);  singly_linked_list_print(head);

    cout << "create list: "; head = singly_linked_list_create(a, n);        singly_linked_list_print(head);
    cout << "detect loop: "; singly_linked_list_detect_loop(head) ? cout << "Yes" : cout << "No"; cout << endl;
    tail = head; while (tail->next) { tail = tail->next; }
    tail->next = singly_linked_list_search(head, a[n/2]);
    cout << "create loop: "; singly_linked_list_print( head );
    cout << "detect loop: "; singly_linked_list_detect_loop(head) ? cout << "Yes" : cout << "No"; cout << endl;
 
    return 0;
}