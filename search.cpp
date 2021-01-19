//   Search Algorithm
//     Linear Search
//     Partition Search
//     Binary Search
//     Interpolation Search
//     Fibonacci Search
//     Hash Search, see hash.cpp
//     Binary Tree Search, see tree.cpp
//

#include <iostream>

using namespace std;

/* Linear Search
 * algorithm: 
 *    search an array or linked list from the beginning, one by one
 *    until the end of the array or list.
 *    - pros: works on the unsorted list  
 *    - cons: must get to the end to know the element does not in.
 * implemetations:
 *    - return: index if found; -1 if not found, 
 * time complexity: worst O(n), average On/2), best O(1) 
 * space complexity: O(1)
 * applications: 
 *    - unsorted array or linked list
 *    - much slower than the algorithms based on sorted array
 */
int linear_search(long a[], int sz, long key)
{
    int i;
    for (i = 0; i < sz; ++i) {
        if (a[i] == key) {
            return i;
        }
    }
    return -1;
}
/* partition search:
 *   this is a linear search, partition the array into k segments, 
 *   then do the linear search in each segment in parallel.
 *   the searching starts from both ends towards to the middle.
 * applications: 
 *    - unsorted array or unsorted linked list
 *    - perform better than the basic linear search on average
 */ 
int partition_search(long a[], int sz, int k, long key)
{
    int i, j;
    int idx[k];
    long *arr[k];

    if (k <= 0) {
        return -1;
    }

    // save the beginning array index and address of each segment
    int sz1 = (sz + k - 1) / k ;
    for (i = 0, j = k - 1; i <= j; ++i, --j) {
        idx[i] = i * sz1;
        idx[j] = sz - 1 - i * sz1; 
        arr[i] = a + idx[i];
        arr[j] = a + idx[j];
    }

    for (int n = 0; n < sz1; ++n) {
        for (i = 0, j = k - 1; i <= j; ++i, --j) {
            if ( *(arr[i] + n) == key) {
                return idx[i] + n;
            }
            if ( *(arr[j] - n) == key) {
                return idx[j] - n;
            }
        }
    }
    return -1;
}
/* Binary Search
 *    cut the searching range by half after comparing the data at the middle.
 *    change the searching range to the first half or the back half.
 *    repeat the previous steps until find the key (first, last, middle are same)
 * implementations: iterative, recursive
 * time complexity: O(log(n))
 * space complexity: O(1) for iterative
 * applications:
 *    - sorted array or sorted linked list
 *    - easy to implement, good performance
 */
int my_binary_search(long a[], int sz, long key)
{
    int first = 0;
    int last  = sz - 1;
    int middle;

    if ((key < a[0]) || (key > a[last])) {
        return -1;
    }

    int count = 0;
    while (first <= last) 
    {
        middle = first + ((last - first) >> 1) + ((last - first) & 1);
        if (key == a[middle]) {
            return middle;
        }
        else if ( key > a[middle]) {
            first = middle + 1;
        }
        else {
            last = middle - 1;
        }
    }
    return -1;
}
/* Golden Search
 *    like the binary search, but spliting the array based on the golden 
 *    ratio 0.618 (1:1.618), use 0.625 in this implementation.
 *    the performance is better when the data is close to the ends. The average
 *    speed is slower than the binary search.
 * time complexity: O(log(n))
 * space complexity: O(1)
 * applications:
 *    - sorted array or sorted linked list
 *    - perform almost the same as binary search
 */
int golden_search(long a[], int sz, long key) 
{
    int first = 0;
    int last  = sz - 1;

    if ((key < a[0]) || (key > a[last])) {
        return -1;
    }

    int count = 0;
    while (first <= last) 
    {
        int golden = first + ((last - first) >> 1) + ((last - first) >> 3);
        if (key == a[golden]) {
            return golden;
        }
        else if ( key > a[golden]) {
            first = golden + 1;
        }
        else { // (key > a[middle])
            last = golden - 1;
        }
    }
    return -1;
}
/* Interpolation Search
 *   instead of using the middle index as in binary search, 
 *   find the index based on the value of Key as below:
 *     interpolation = (last - first) * (key - A[first]) / (A[last] - A[first])
 *   then split the array into two: (first, interpolation), (interpolation + 1, last)
 * implemetation: iterative
 * time complexity: O(log(n))
 * space compleexity: O(1)
 * applications:
 *    - sorted array or sorted linked list
 *    - best performance, better than binary search on average
 */
int interpolation_search(long a[], int sz, long key)
{
    if ((key < a[0]) || (key > a[sz - 1])) {
        return -1;
    }

    int first = 0;
    int last  = sz - 1;
    int interpolation;

    // while (first <= last && key >= a[first] && key <= a[last]) {  // quickly find out if the key is in the list
    while (first <= last) 
    {
        if (last == first || a[last] == a[first]) {  // denominator will be 0
            interpolation = last;
        }
        else {
            // int interpolation = first + (last - first) * (key - a[first]) / (a[last] - a[first]);
            // unsigned long long interpolation = (unsigned long long)(last - first) * (key - a[first]) / (a[last] - a[first]);
            interpolation = first + (((double)(last - first) / (a[last] - a[first])) * (key - a[first]));
        }

        if (key == a[interpolation]) {
            return interpolation;
        }
        else if ( key > a[interpolation]) {
            first = interpolation + 1;
        }
        else { // (key > a[middle])
            last = interpolation - 1;
        }
    }
    return -1;
}
/* Fibonacci Search
 *   split the array into two parts by dividing its size into two
 *   Fibonacci numbers: fab1 + fab2 = size
 * applications:
 *   - works on sorted array or sorted linked list
 *   - good performance, but no better than binary search on average.
 */
int fibonacci_search(long a[], int sz, long key)
{
    int fib1 = 0, fib2 = 1;
    int fibM = fib1 + fib2;
  
    while (fibM < sz)   // find fab1, fab2 close to size
    { 
        fib1 = fib2, fib2 = fibM, fibM  = fib1 + fib2; 
    } 
  
    int offset = -1; 
    while (fibM > 1) 
    { 
        int i = min(offset + fib1, sz - 1); 
  
        if (a[i] < key) 
        { 
            fibM = fib2, fib2 = fib1, fib1 = fibM - fib2; 
            offset = i; 
        } 
        else if (a[i] > key) 
        { 
            fibM  = fib1, fib2 = fib2 - fib1, fib1 = fibM - fib2;
        } 
        else {
            return i; 
        }
    } 
    // there could be one element remained
    if(fib2 && a[offset+1] == key) {
        return offset+1; 
    }
    return -1; 
}

/* testing driver code
 */
#include <chrono>
#include <functional>
#include <algorithm>
#include <vector>

using namespace std::chrono;

int search_function(long a[], int n, int i, function<int (long*, int, long)> f) { return f(a, n, a[i]); }
int search_function(long a[], int n, int i, function<int (long*, int, int, long)> f) { return f(a, n, 22, a[i]); }

#define TESTING_SEARCH(s, f) { \
    cout << "\e[1m" << s << "\e[0m" << ": "; \
	auto start = chrono::high_resolution_clock::now(); \
    for (i = 0; i < n; ++i) { if ((result = search_function(A, n, i, f)) < 0) break; } \
	auto end = chrono::high_resolution_clock::now(); \
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count(); \
    cout << "Elapsed time: " << duration << " us, " << "Average time: " << double(duration) / n << " us" << endl; \
    if (result < 0) { cout << "i = " << i << ", A[i] = " << A[i] << " is not found" << endl; } \
}
/* testing main
 */
int main()
{
    int i, n = 100000;
    int result;
    long A[n];

    cout << "---- generate " << n << " random numbers ----" << endl;
    for(i = 0; i < n; ++i) { A[i] = rand() % (n * 10); }
    cout << "Original Array: " << endl;
    int k = n/2 - 1;
    cout << "A[" << k - min(8, n) << "] = ";
    for(i = k - min(8, n); i < min(n, k + 8); ++i) { cout.width(4); cout << A[i] << ", "; } cout << endl;

    long key = A[k];
    cout << "search key = " << key << " at " << k << " in array A[]" << endl;

    TESTING_SEARCH("Linear Search", linear_search);

    TESTING_SEARCH("Partition Search", partition_search);
    
    sort(A, A + n);
    cout << "Sorted Array: " << endl;
    cout << "A[" << k - min(8, n) << "] = ";
    for (i = k - min(8, n); i < min(n, k + 8); ++i) { cout.width(4); cout << A[i] << ", "; } cout << endl;

    TESTING_SEARCH("Binary Search", my_binary_search);

    TESTING_SEARCH("Golden Search", golden_search);

    TESTING_SEARCH("Interpolation Search", interpolation_search);

    TESTING_SEARCH("Fibonacci Search", fibonacci_search);

    cout << "\e[1m" << "C++ Binary Search" << "\e[0m" << ": "; \
    vector<long> V(A, A + n);
    sort(V.begin(), V.end());
	auto start = steady_clock::now();
    for (i = 0; i < n; ++i) {
        result = binary_search(V.begin(), V.end(), A[i]);
    }
	auto end = steady_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start).count();
    cout << "Elapsed time: " << duration << " us, " << "Average time: " << double(duration) / n << " us" << endl;
    if (result <= 0) { cout << A[i] << " is not found" << endl; }
}