/***
 **** this is about the heap binary tree, not the heap memory ****
 ***/
// Heap
//   it is a special completed binary tree. there are three types of heaps:
//      1) max heap, the node value is greater than the children, the root is the max.
//      2) min heap, the node value is less than the children, the root is the min.
//      3) min-max heap, the values are alternated on levels.
//   To insert an element to a max-heap, 
//      add the element to the end of the tree (or array), then check its parents
//      swap if needed to make the tree be a max-heap again.
//   To delete an element from a max-heap
//      we can only delete the root element from a heap tree
//      After remove the root node, bring the node at the tail to the root, then
//      swap with the child who has the greater value unti the tree become a heap tree.
//   To adjust a binary to be a heap
//      start from the last element, for each node, check its children to see if it
//      it is a heap, swap it and check its child if it is still a heap... 
//   To sort the heap
//      delete the root node and put it to the end, reduced the heap size by 1.
//      the heap will be sorted after delete every element in the heap.
//      the heap sort algorithm is first to insert elements to the heap, then delete
//      every element.
//   implemetations:
//      array: the root is the first element, index is 0. 
//             the left node of any node i must be at 2 * i + 1.
//             the right node of any node i must be at 2 * i + 2.
//      tree:  a complete binary tree, see TreeNode() in tree.cpp
//   applications:
//    - priority queues
//    - k-way merge
//    - shortest path first algorithms
// 

#include <iostream>
// #define NDEBUG
#include <assert.h>

using namespace std;

/* Heap Adjust
 * algorithm: for the node (element) that has the index i,
 *            its left child is at 2*i, its right child is at 2*i + 1. 
 *            compare these three values, put the largest at i.
 *            continue the adjust if the node at i has been changed.
 */

/* adjust a binary tree downward to be a min-heap or max-heap
 */
void heap_adjust_down(long a[], int sz, int i, bool is_max)
{
    int left = 2*i + 1;
    int right = 2*i + 2;
    int m = i;

    if (sz <= 1 || i < 0) {
        return;
    }
    else if (left >= sz) {
        return;
    }
    else if (right >= sz) {
        m = left;
    }
    else if (is_max) {
        m = a[left] > a[right] ? left : right;
    }
    else {
        m = a[left] < a[right] ? left : right;
    }

    bool adjust = is_max ? a[i] < a[m] : a[i] > a[m];
    if (adjust) {
        swap(a[i], a[m]);
        heap_adjust_down(a, sz, m, is_max);
    }
}
/* 
 * heapify (adjust to heap) bottom-up
 * best used for adding an element 
 */
void heap_adjust_up(long a[], int sz, int i, bool is_max)
{ 
    if (sz <= 1 || i <= 0) {
        return;
    }

    int parent = (i - 1) >> 1;  // (i - 1)/2
    if (parent >= 0) { 
        bool adjust = is_max ? a[i] > a[parent] : a[i] < a[parent];
        if (adjust) { 
            swap(a[i], a[parent]); 
            heap_adjust_up(a, sz, parent, is_max); 
        } 
    } 
}
/* make a binary tree to be a heap
 * must starting from bottom 
 * do not need to do anything for the bottom level
 */
void max_heap_build(long a[], int sz)
{
    if ( sz <= 1) {
        return;
    }
    for (int i = (sz >> 1) - 1; i >= 0; --i) {
        heap_adjust_down(a, sz, i, true);
    }
}
/*
 * build a min-heap from an array
 */
void min_heap_build(long a[], int sz)
{
    if ( sz <= 1) {
        return;
    }
    for (int i = (sz >> 1) - 1; i >= 0; --i) {
        heap_adjust_down(a, sz, i, false);
    }
}
/*
 * check if an array is a min-heap
 */
bool is_min_heap_array(long hp[], int sz)
{
    if (!hp || !sz) {
        return false;
    }
    if (sz == 1) {
        return true;
    }

    int left, right;
    for (int i = 0; i < (sz >> 1); ++i) {
        left = 2*i + 1;
        if (left >= sz ) {
            break;
        }
        if (hp[i] > hp[left]) {
            return false;
        }
        right = 2*i + 2;
        if (right >= sz) {
            break;
        }
        if (hp[i] > hp[right]) {
            return false;
        }
    }
    return true;    
}
/*
 * check if an array is a max-heap
 */
bool is_max_heap_array(long hp[], int sz)
{
    if (!hp || !sz) {
        return false;
    }
    if (sz == 1) {
        return true;
    }

    int left, right;
    for (int i = 0; i < (sz >> 1); ++i) {
        left = 2*i + 1;
        if (left >= sz ) {
            break;
        }
        if (hp[i] < hp[left]) {
            return false;
        }
        right = 2*i + 2;
        if (right >= sz) {
            break;
        }
        if (hp[i] < hp[right]) {
            return false;
        }
    }
    return true;    
}
/*
 * check if an array is a heap array
 */
bool is_heap_array(long hp[], int sz)
{
    if (hp[0] < hp[sz - 1] )  // min_heap
    {
        return is_max_heap_array(hp, sz);
    }
    else  // min-heap
    {
        return is_min_heap_array(hp, sz);
    }
}
/* 
 * remove and return the root (max or min) element.
 * algorithm: 
 *    save the root, 
 *    move the last element to the root,
 *    adjust the heap top-down.
 * parameters: 
 *    (long)hp[] - heap array
 *    (int)sz    - array size, must > 0
 * Note:
 *    this implementation does not change the array size,
 *    clear the (last) element that is not in use.
 *    the caller needs to rmember the heap size.
 */            
long heap_pop(long hp[], int sz)
{
    // cannot return anything, just abort
    assert(sz > 0);

    bool is_max = (sz > 1) && (hp[0] > hp[sz - 1]);
    long root = hp[0];
    --sz;
    hp[0] = hp[sz], hp[sz] = 0;
    heap_adjust_down(hp, sz, 0, is_max);
    return root;
}
/* 
 * add the new element at the end of the tree (heap array).
 * algorithm:
 *   add the new element at the end of the heap.
 *   adjust the heap from bottom up.
 * note:
 *   make sure the array has an extra space for the new element.
 *   caller needs to remember the heap size.
 */
int heap_push(long hp[], int sz, int data)
{
    bool is_max = (sz > 1) && (hp[0] > hp[sz - 1]);
    hp[sz - 1] = data;
    heap_adjust_up(hp, sz, sz - 1, is_max);     // 
    return hp[0];
}
/* Heap Sort
 * algorithm: to easily calculate the heap tree index, we need index stat from 1.
 *            so make a new array its size 1 bigger than the orignal array.
 *            convert (adjust) the array to a heap tree, start at the middle.
 *            copy the root node to the far right, reduce the array size by 1.
 *            adjust the heap tree make sure the element at 1 is the root (max).
 *            contine the last two steps until array size is 0.
 * applications: priority queues
 */
void heap_sort(long hp[], int sz)
{
    if (sz <= 1)  return;

    bool is_max = hp[0] > hp[sz - 1];
    for (int i = sz - 1; i >= 1; i--) {
        swap(hp[i], hp[0]);
        heap_adjust_down(hp, i, 0, is_max);
    }
}

/* tesing driver code
 */
#include <cmath>

/* display heap data in array format
 */
void heap_array_display(string title, long hp[], int sz)
{
    cout << "\e[1m" << title << "\e[0m" << ": " << "A[" << sz << "] = "; 
    for (int i = 0; i < sz; ++i) { cout << hp[i] << ", "; } cout << endl;
}
/* display heap data by tree levels
 */
void heap_tree_display(long hp[], int sz)
{
    if (sz <= 0)  return;

    // const int w = log10(sz * 10) + 1;
    
    cout << " root:  " << "[--0] = " << hp[0] << endl;
    for (int i = 1, n = 1; n < sz; ++i) {
        cout.width(5); cout << i << ":  ";
        int m = n + pow(2, i);
        for (int j = n; (j < m) && (n < sz); ++j, ++n) {
            cout << "[" << (j - 1)/2 << "-" << j << "] = " << hp[j] << "  ";
        }
        cout << endl;
    }
}
/* testing main: could take one argument as the size of the heap array
 */
int main(int argc, char *argv[])
{   
    int n = 32;
    if (argc > 1) {
        if ((n = atoi(argv[1])) < 0)
            return -1;
    }
    else {
        n = rand() % n;
    }

    int heap_size = n;
    long A[n];  // testing heap array
    long B[n]; 

    // create an array with n of random data
    for(int i = 0; i < n; ++i) { B[i] = rand() % (n * 10); }
    heap_array_display("Heap Array", B, n);

    copy(B, B + n, A);
    max_heap_build(A, n);
    heap_array_display("Max-Heap Build", A, n);
    heap_tree_display(A, n);

    long d = heap_pop(A, n);
    heap_size -= 1;
    heap_array_display("Heap Pop", A, n);
    heap_tree_display(A, n);
    
    heap_push(A, n, d);
    heap_size += 1;
    heap_array_display("Heap Push", A, n);
    heap_tree_display(A, n);

    heap_sort(A, n);
    heap_array_display("Heap Sort", A, n);
    heap_sort(A, n);
    heap_array_display("Heap Sort", A, n);

    copy(B, B + n, A);
    min_heap_build(A, n);
    heap_array_display("Min-Heap Build", A, n);
    heap_tree_display(A, n);

    A[n - 1] = 64;
    heap_array_display("Is Heap Array", A, n);
    bool b = is_heap_array(A, n);
    if (b) { cout << "Is a Heap"; }  else { cout << "Not a Heap"; }  cout << endl;

    // heap_adjust_down(A, n, (n >> 1) - 1, false);
    heap_adjust_up(A, n, n - 1, false);
    heap_array_display("Heap Adjust", A, n);

    return 0;
}
