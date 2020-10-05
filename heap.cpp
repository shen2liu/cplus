// k-Ary Heap Tree
//   it is a special completed binary tree. there are three types of heaps:
//   1) max heap, the node value is greater than the children, the root is the max.
//   2) min heap, the node value is less than the children, the root is the min.
//   3) min-max heap, the values are alternated on levels. // not implemented
//
// implementation:
//   the heap class implements a k-ary min-heap or max-heap for any data types.
//   the heap is stored in a vector which manages its memory automatically.
//   the heap is represented by vector, each node (i) has k children.
//   the index of a child is: i * k + m; 1 <= m <= k.
//   it is a binary heap when k is equal to 2.
//   the root is the first element, index is 0. 
//   the root is minimum when the heap is min-heap.
//   the root is maximum when the heap is max-heap.
//
// applications:
//   - priority queues
//   - k-way merge
//   - shortest path first algorithms
//   - etc.
// 

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// HeapTree::type
#define MIN_HEAP    0
#define MAX_HEAP    1   

template<class T>
class HeapTree {
    int size;           // number of elements in the heap
    int type;           // heap type: 0 = min; 1 = max; else = undefined
    int kAry;           // number of chilren
protected:
    vector<T> heap;
    void build_heap(const vector<T>& v);
    void adjust_down(int i);
    void adjust_up(int i);
public: 
    HeapTree(const vector<T>& v) : size(v.size()), type(MAX_HEAP), kAry(2) { build_heap(v); };
    HeapTree(const vector<T>& v, const int& t) : size(v.size()), type(t), kAry(2) { build_heap(v); }
    HeapTree(const vector<T>& v, const int& t, const int& k) : size(v.size()), type(t), kAry(k) { build_heap(v); }

    int  get_size() { return size; };
    int  get_type() { return type; };
    T pop();
    void push(T x);
    void sort();
    void display_heap_array(string title);
    void display_heap_tree();
};

template<class T>
struct MinHeap : public HeapTree<T> {
    MinHeap(const vector<T>& v) : HeapTree<T>(v, MIN_HEAP) {}   // binary min-heap
    MinHeap(const vector<T>& v, const int& k) : HeapTree<T>(v, MIN_HEAP, k) {}  // k-ary min-heap
};

template<class T>
struct MaxHeap : public HeapTree<T> {
    MaxHeap(const vector<T>& v) : HeapTree<T>(v, MAX_HEAP) {}   // binary max-heap
    MaxHeap(const vector<T>& v, const int& k) : HeapTree<T>(v, MAX_HEAP, k) {}  // k-ary max-heap
};

/* build_heap:
 *   copy a vector into the heap class, and
 *   turn it into a heap (tree).
 */
template<class T>
void HeapTree<T>::build_heap(const vector<T>& v)
{
    int sz = size;
    if (sz <= 1) {
        return;
    }
    heap = v;

    for (int i = sz/kAry - 1; i >= 0; --i) {
        adjust_down(i);
    }
}
/* adjust_down:
 *   adjust an array to a heap using recursion,
 *   starting from node i and going downward.
 */
template<class T>
void HeapTree<T>::adjust_down(int i) 
{ 
    if (i < 0 || size <= 1 || kAry < 1) {
        return;
    }

    int  mm_index = i;
    T mm_child = heap[i];

    for (int m = 1; m <= kAry; ++m) {  
        int child_index = kAry * i + m;
        if (child_index >= size) {
            break;
        }
        if ( ((type == MIN_HEAP) && (heap[child_index] < mm_child)) ||  
             ((type == MAX_HEAP) && (heap[child_index] > mm_child)) ) 
        {
            mm_index = child_index;
            mm_child = heap[child_index];
        }
    }

    if ( ((type == MIN_HEAP) && (heap[i] > heap[mm_index])) ||  
         ((type == MAX_HEAP) && (heap[i] < heap[mm_index])) ) 
    {
        swap(heap[i], heap[mm_index]);
        adjust_down(mm_index);
    }
} 
/* adjust_up:
 *   adjust an arry to be a heap recursively,
 *   starting from node i and going upward.
 */
template<class T>
void HeapTree<T>::adjust_up(int i) 
{ 
    if (i < 0 || size <= 1 || kAry < 1) {
        return;
    }
    int parent = (i - 1) / kAry;
    if ( ((type == MIN_HEAP) && (heap[i] < heap[parent])) ||  
         ((type == MAX_HEAP) && (heap[i] > heap[parent])) ) 
    {
        swap(heap[i], heap[parent]);
        adjust_up(parent);
    } 
}
/* pop:
 *   remove and return the root (max or min) element.
 * algorithm: 
 *   save the root;
 *   move the last element to the root;
 *   adjust the heap top-down;
 *   return the root;
 */
template<class T>
T HeapTree<T>::pop()
{
    T root = heap[0];
    --size;
    heap[0] = heap[size];
    heap.pop_back();
    adjust_down(0);
    return root;
}
/* push:
 *   add the new element at the end of the tree (heap array).
 * algorithm:
 *   add the new element at the end of the heap.
 *   adjust the heap from bottom up.
 */
template<class T>
void HeapTree<T>::push(T x)
{
    heap.push_back(x);
    ++size;
    adjust_up(size - 1);
}
/* sort:
 *   sorting the vector in asending order using max-heap or 
 *   desending order using min-heap.
 * algorithm:
 *   convert the vector into a heap (vector),
 *   pop the first element and move it to the end,
 *   reduce the size of the heap,
 *   repeat the previous steps until only one element left,
 *   restore the size of the heap.
 */
template<class T>
void HeapTree<T>::sort()
{
    int sz = size;
    for (int i = sz - 1; i >= 1; i--) {
        swap(heap[i], heap[0]);
        --size;
        adjust_down(0);
    }
    size = sz;
}
/* display_heap_array:
 *    display the heap data in array format.
 * note:
 *    only display the data in the beginning and at the end,
 *    when the size of heap greater than 32.
 */
template<class T>
void HeapTree<T>::display_heap_array(string title)
{
    const int words_line = min(16, size);
    const int word_width = log10(size) + 2;

    cout << "\e[1m" << title << "\e[0m" << ": " << endl;
    cout << "H[" << size << "] = "; 
    for(unsigned i = 0; i < words_line; ++i) {
        cout.width(word_width); cout << heap[i] << ", "; 
    } 
    cout << endl;
    if (size > words_line) {
        for(int i = 0; i < word_width + 5; ++i) { 
            cout << " "; 
        }
        for(int i = size - min(size - words_line, words_line); i < size; ++i) {
            cout.width(word_width); cout << heap[i] << ", "; 
        }
        cout << endl;
    }
}
/* display_heap_tree:
 *   display the heap data by tree levels.
 *   [p-c] shows the parent index and child index.
 */
template<class T>
void HeapTree<T>::display_heap_tree()
{
    if (size <= 0)  return;

    cout << " root:  " << "[--0] = " << heap[0] << endl;
    for (int i = 1, n = 1; n < size; ++i) {
        cout.width(5); cout << i << ":  ";
        int m = n + pow(kAry, i);
        for (int j = n; (j < m) && (n < size); ++j, ++n) {
            cout << "[" << (j - 1)/kAry << "-" << j << "] = " << heap[j] << "  ";
        }
        cout << endl;
    }
}
/* main:
 *   testing driver main.
 *   could take one argument as the size of the heap.
 */
int main(int argc, char *argv[])
{   
    int n = 32;
    if (argc > 1) {
        if ((n = atoi(argv[1])) < 0){
            return -1;
        }
    }
    else {
        n = rand() % n;
    }

    long A[n];

    for(int i = 0; i < n; ++i) { A[i] = rand() % (n * 10); }
    cout << "\e[1m" << "Original Array" << "\e[0m" << ": " << endl;
    cout << "A[" << n << "] = "; 
    for(int i = 0; i < min(16, n); ++i) { cout.width(log10(n) + 2); cout << A[i] << ", "; } cout << endl;
    cout << "        ";
    for(int i = n - min(n - 16, 16); i < n; ++i) { cout.width(log10(n) + 2); cout << A[i] << ", "; } cout << endl;

    vector<long> v(A, A + n);
    HeapTree<long> hv(v, 1, 3);
    hv.display_heap_array("Heap Vector");
    hv.display_heap_tree();

    long d = hv.pop();
    hv.display_heap_array("Heap Pop");
    // hv.display_heap_tree();
    hv.push(d);
    hv.display_heap_array("Heap Push");
    // hv.di splay_heap_tree();

    MinHeap<long> min_heap(v);
    min_heap.display_heap_array("Min-Heap");
    min_heap.sort();
    min_heap.display_heap_array("Min-Heap Sort");

    MaxHeap<long> max_heap(v);
    max_heap.display_heap_array("Max-Heap");
    max_heap.sort();
    max_heap.display_heap_array("Max-Heap Sort");

    return 0;
}