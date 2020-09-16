// Sorting Algorithms:
//   Primary Key: an uniform key is used to compare the data objects for sorting.
//   Sorting techniques: key comparative sort, radix (digital) sort
//   sorting categorise: internal sorting (main memory), external sorting (disk)
// 
//   Simple Sort Methods:   worst           best
//     Bubble Sort          O(n*n)          O(n)
//     Insertion Sort       O(n*n)          O(n)
//     Selection Sort       O(n*n)          O(n*n)
//   Advanced Sort Methods
//     Quick Sort           O(n*n)          O(n*log(n))
//     Merge Sort           O(n*logn)       O(n*log(n))
//     Shell Sort           O(n*(logn)^2)
//     Heap Sort            O(n*log(n))     O(n*log(n))
//     Radix Sort           O(m*(n+r))
//     Binary Tree Sort (see tree.cpp)
//     Counting Sort
//     Bucket Sort
//
//  the following programs sort an arry in asending order.
//
//  note: using data type "long" as the data type array to
//        distinguish from other arguments.
//
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

/* Bubble Sort
 * algorithm: compare the item with the next one in the array (or list)
 *            swap the two items if they are not in desired order
 *            loop the list a few of times until no item needs to be swapped.
 * implementation:  loop, recursive,
 * time complexity: O(n*n)
 * space complexity: O(1)
 */
void algorithm_bubble_sort(long *a, int sz)
{   
    bool swapped;
    int  k = 0;
    do {
        swapped = false; 
        for (int i = 0; i < sz - k - 1; ++i) {
            if (a[i] > a[i + 1]) {
                swap(a[i], a[i + 1]);
                swapped = true;
            }
        }
        ++k;    // the k-th element from the end is done
    } while (swapped);
}
/* Insertion Sort
 * algorithm: start at the second from back, save the value
 *            compare the value with the next item:
 *            if the value is less (asending), move to the previous
 *            if the value is greater, insert the value by moving other items ahead
 *            repeat the compare-and-insert until get to the front
 * implementation: can be started from back or front.
 *            when sort linked list, must take care of the head and tail.     
 * time complexity: worst O(n*n)
 * space complexity: O(1)
 * applications:
 *    - number of elements in the array is small
 *    - the array is almost sorted, only a few of elements need to be sorted.
 */
void algorithm_insertion_sort(long *a, int sz)
{
    for (int k = sz - 2; k >= 0; --k) {
        int i = k + 1;
        long key = a[k];
        while ((i < sz) && (key > a[i])) {
            a[i - 1] = a[i];
            ++i;
        }
        a[i - 1] = key;
    }
}
/* Slection Sort
 * algorithm: start from the beginning (index 0),
 *            find the index of the least number, swap it to the front,
 *            work on the next item, find the least and swap, until the end.
 * implemetation: the simplest, could use binary search to find the least.
 * time complexity: O(n*n)
 * space complexity: O(1)
 * applications:
 */
void algorithm_selection_sort(long *a, int sz)
{
    for (int j = 0; j < sz; ++j) {
        int low = j;
        // find the index of the least number
        for (int i = j + 1; i < sz; ++i) {
            if (a[i] < a[low]) {
                low = i;
            }
        }
        swap(a[j], a[low]);
    }
}
/* Quick Sort (Patition Exchange Sort) with single pivot:
 *    invented by C.A.R. Hoare to improve slection sort.
 *    the most efficient internal sorting methods.
 *    uses the hardware cache most effectively.
 * algorithm: one of divide-and-conquer methods
 *    choose the first (left) as the pivot and start from both sides
 *    move the front pointer until find the item greater than the pivot
 *    move the back pointer until find the item less than the pivot
 *    swap the items in the two group (front and back). so
 *    all the items that less than the pivot to the front
 *    all the items that greater than the pivot to the back
 *    swap the first in the front with the first in the back
 *    recursively to quick-sort on the two groups.
 * implementations: recursive and iterative
 * time complexity: best O(n*log(n)), worst O(n*n)
 * space complexity: O(1)
 */
void algorithm_quick_sort(long *a, int left, int right)
{
    if (left >= right) {
        return;
    }
    long pivot = a[left];
    int i = left;
    int j = right;
    while(i < j) {
        while ((i < right) && (a[i] <= pivot)) { ++i; }
        while ((j > left) && (a[j] >= pivot)) { --j; }
        if (i < j) { 
            swap(a[i], a[j]);
        }
    }
    swap(a[j], a[left]);
    algorithm_quick_sort(a, left, j - 1);
    algorithm_quick_sort(a, j + 1, right);
}
/* Dual Pivot Qick Sort
 *   Invented by Vladimir Yaroslavskiy in 2009.
 *   it is faster than the single pivot quick sort,
 *   now it is the default sort algorithm of Java.
 * Algorithm:
 *   it uses two pivots, one in the left, the other in the right.
 *   partition the arry into three parts:
 *   elements in the left part are less than the left pivot.
 *   elements in the right part are greater than the right pivot.
 *   elements in the middle part are greater than the left pivot
 *   but less than the right pivot.
 * Implementations:
 *   if the array is already sorted, will bring to the worse case.
 *   may choose the median to be the pivots.
 * Time Complexity:
 *   the average number of comparisons is 2*n*ln(n),
 *   the average number of swaps is 0.8*n*ln(n).
 *   the single pivot Quicksort has 2*n*ln(n) and 1*n*ln(n) respectively.
 * Space Complexity: O(1)
 */
void algorithm_dual_pivot_quick_sort(long *a, int left, int right)
{
    if (left >= right)
        return;

    // the left pivot should be less than the right pivot
    if (a[left] > a[right]) {
        swap(a[left], a[right]);
    }
    int p = a[left];    // the left pivot
    int q = a[right];   // the right pivot
    int j = left + 1;   // iterator of the left partition
    int g = right - 1;  // iterator of the right partition

    for (int k = j; k <= g; ++k)
    {
        // if elements are less than the left pivot 
        if (a[k] < p) { 
            swap(a[k], a[j]); 
            j++;
        } 
        // if elements are greater than or equal to the right pivot 
        else if (a[k] >= q) { 
            while (a[g] > q && k < g) {
                g--; 
            }
            swap(a[k], a[g]); 
            g--; 
            if (a[k] < p) { 
                swap(a[k], a[j]); 
                j++;
            }
        } 
    } 
    // bring pivots to their appropriate positions. 
    swap(a[left], a[--j]); 
    swap(a[right], a[++g]); 
  
    algorithm_dual_pivot_quick_sort(a, left,  j - 1);
    algorithm_dual_pivot_quick_sort(a, j + 1, g - 1);
    algorithm_dual_pivot_quick_sort(a, g + 1, right);
}
/* Merge Sort
 *    efficient for external sorting, such as, data in a file.
 *    doesn't take advantage when the data is already in order.
 *    good for sorting linked list because no extra sub-arrays.
 * algorithm: one of the divide-and-conquer methods
 *    split the array into sub-array recursively until the arrays cannot be splited.
 *    merge sub-arrays, starting from the shortest array (one element).
 *    select the smaller (less) data and insert/replace it back to the orginal array.
 * implementations: iterative, recursive and linked merge sort
 * time complexity: always O(nlogn)
 * space complexity: O(n)
 * applications: 
 *    - sort the (huge) files on the disks.
 *    - sort the linked list, no extra memory needed.
 */
void algorithm_merge(long a[], int left, int middle, int right)
{
    int i, j, k; 
    int n1 = middle - left + 1; 
    int n2 = right - middle;
  
    int L[n1], R[n2];
  
    // split the array at the middle into two sub-arrays
    for (i = 0; i < n1; i++) {
        L[i] = a[left + i]; 
    }
    for (j = 0; j < n2; j++) {
        R[j] = a[middle + 1 + j]; 
    }
  
    i = 0;      // Initial index of left subarray 
    j = 0;      // Initial index of second subarray 
    k = left;   // Initial index of merged subarray 

    // merge the two sub-arrays back into the orginal array by
    // inserting the smaller data at the heads of the two arrays.
    while (i < n1 && j < n2) { 
        if (L[i] <= R[j]) { 
            a[k] = L[i]; 
            i++; 
        } 
        else { 
            a[k] = R[j]; 
            j++; 
        } 
        k++; 
    } 
    // copy the remaining elements of L[], if any 
    while (i < n1) { 
        a[k++] = L[i++]; 
    } 
    // Copy the remaining elements of R[], if any
    while (j < n2) { 
        a[k++] = R[j++]; 
    }
} 
void algorithm_merge_sort(long a[], int left, int right)
{ 
    if (left >= right) { 
        return;
    }
    // middle = (l+r)/2
    int middle = left + ((right - left) >> 1);
  
    algorithm_merge_sort(a, left, middle); 
    algorithm_merge_sort(a, middle + 1, right); 
    algorithm_merge(a, left, middle, right); 
} 
/* Shell Sort
 */
void algorithm_shell_half_sort(long a[], int sz)
{
    // Start with a big gap, then reduce the gap 
    for (int gap = sz/2; gap > 0; gap /= 2) 
    { 
        // cout << "gap=" << gap << endl;
        // Do a gapped insertion sort for this gap size. 
        // The first gap elements a[0..gap-1] are already in gapped order 
        // keep adding one more element until the entire array is gap sorted  
        for (int i = gap; i < sz; ++i) 
        { 
            // add a[i] to the elements that have been gap sorted 
            // save a[i] in temp and make a hole at position i 
            int temp = a[i]; 
            
            // cout << "gap=" << gap << " i=" << i << " " << temp << " a[i]=" << a[i];
  
            // shift earlier gap-sorted elements up until the correct  
            // location for a[i] is found 
            int j;             
            for (j = i; j >= gap && a[j - gap] > temp; j -= gap) {
                a[j] = a[j - gap]; 
                // cout << " j=" << j << " " << a[j] << " j-gap=" << j - gap << " " << a[j-gap];
            }
            //  put temp (the original a[i]) in its correct location 
            a[j] = temp; 
            // cout << "  # j=" << j << " a[j]=" << a[j] << endl;
        } 
        // for (int k = 0; k < sz; k++) { cout << a[k] << " "; } cout << endl;
    }   
}
/* Shell Sort
 *    invented by D. L. Shell to inprove the insertion and bubble sort.
 *    efficiently select the gap (shell) sizes, eg. 40, 13, 4, 1, etc.
 * algorithm:
 */
void algorithm_shell_sort(long a[], int sz)
{
    int k2, k1, k;
    int shell_size = sz;
    int change = 0;

    while (shell_size > 1) {
        k2 = 0;
        k1 = 1;
        // cout << "shell=" << shell_size << " k=" << k1 * 4 - k2 * 3;
        while ((k = k1 * 4 - k2 * 3) < shell_size) {
            k2 = k1;
            k1 = k;
        }
        k = k1;
        // cout << " k2=" << k2 << " k1=" << k1 << " k=" << k << endl;
        for (int i = k; i < sz; i++) {
            long tmp = a[i];
            int s = i - k;
            while (s >= 0 && tmp < a[s]) {
                a[s + k] = a[s];
                ++change;
                // cout << "swap " << change << ": " << i << "-" << s << "-" << s + k << " " << tmp << " " << a[s] << endl;
                s -= k;
            }
            a[s + k] = tmp;
        }
        // for (int j = 0; j < sz; j++) { cout << a[j] << " "; } cout << endl;
        shell_size = k;
    }
}
/* Radix (Exchange) Sort
 * algorithm: similar to the quick sort, instead of comparing the data,
 *    radix compares the bits (from msb to lsb).
 * implementation: recursive, iterative
 * time complexity: 
 * application: 
 *    more suitable for hardware implementation.
 */
void algorithm_radix_sort(long a[], int first, int last, int bitnum)
{
    if (first >= last || bitnum < 0 ) {
        return;
    }
    int i = first;
    int j = last;
    while (i != j) {
        while (i < j && !((a[i] >> bitnum) & 1)) { ++i; }
        while (j > i && ((a[j] >> bitnum) & 1)) { --j; }
        swap(a[i], a[j]);
    }
    if ( !((a[last] >> bitnum) & 1) ) { ++j; }
    // cout << "bit=" << bitnum << " i=" << i << " j=" << j << " " << a[i] << " " << a[j] << endl;
    algorithm_radix_sort(a, first, j - 1, bitnum - 1);
    algorithm_radix_sort(a, j, last, bitnum - 1);
}

/* testing driver code
 */
// void test_sort(long B[], int n, (void *)f())
// {
// }

int main()
{
    // cout << "Input an integer: ";
    // cin >> n;
    int n = 18;
    long A[n];
    long B[n];        
    
    cout << "---- generate " << n << " random numbers ----" << endl;
    for(int i = 0; i < n; ++i) { B[i] = rand() % (n * 10); }
    cout << "Original Array: " << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << B[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Bubble Sort: ";
    copy(B, B + n, A);
	auto start = steady_clock::now();
    algorithm_bubble_sort(A, n);
	auto end = steady_clock::now();
    cout << "Elapsed time: " << duration_cast<microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Insertion Sort: ";
    copy(B, B + n, A);
	start = chrono::steady_clock::now();
    algorithm_insertion_sort(A, n);
	end = chrono::steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Selection Sort: ";
    copy(B, B + n, A);
	start = chrono::steady_clock::now();
    algorithm_selection_sort(A, n);
	end = chrono::steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Quick Sort: ";
    copy(B, B + n, A);
    // long C[] = {33, 60, 5, 15, 25, 12, 45, 70, 35, 7};
	start = chrono::steady_clock::now();
    algorithm_quick_sort(A, 0, n - 1);
	end = chrono::steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Dual Pivot Quick Sort: ";
    copy(B, B + n, A);
    // long C[] = {33, 60, 5, 15, 25, 12, 45, 70, 35, 7};
	start = chrono::steady_clock::now();
    algorithm_dual_pivot_quick_sort(A, 0, n - 1);
	end = chrono::steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Merge Sort: ";
    copy(B, B + n, A);
	start = chrono::steady_clock::now();
    algorithm_merge_sort(A, 0, n - 1);
	end = chrono::steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Shell Half: ";
    copy(B, B + n, A);
	start = chrono::steady_clock::now();
    algorithm_shell_half_sort(A, n);
	end = chrono::steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Shell Sort: ";
    copy(B, B + n, A);
	start = steady_clock::now();
    algorithm_shell_sort(A, n);
	end = steady_clock::now();
    microseconds duration(duration_cast<microseconds>(end - start));
    cout << "Elapsed time: " << duration.count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Radix Sort: ";
    copy(B, B + n, A);
	start = steady_clock::now();
    algorithm_radix_sort(A, 0, n - 1, sizeof(A[0]) * 4 - 1);    // max_bits = 31 (sizeof(long) * 4 - 1))
	end = steady_clock::now();
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " us" << endl;
    for(int i = 0; i < n; ++i) { cout.width(4); cout << A[i] << ", ";  if ((i + 1) % 20 == 0) { cout << endl; } } cout << endl;

    cout << "Heap Sort: see \"heap.cpp\"";
} 

