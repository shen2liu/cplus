//  the algorithms that count the set-bits in an integer
//      for-loop
//      while-loop
//      recursive
//      kernighan
//      lookup_table
//      pop_count
//      cpp-bitset
//      cpp-built-in
//
//  the following programs are:
//  couting how many bits that are set to 1 in a 32-bit integer.
//
#include <iostream>

using namespace std;

/* algorithm: iterate the bits one by one, 
*  the total steps is 32.
 */
int bits_count_for_loop(unsigned int number) 
{
    int count = 0;
    for (int i = 0; i < (sizeof(number) * 8); ++i) {
        if ( (number & (1 << i)) ) {
            ++count;
        }
    }
    return count;
}
/* algorithm: check the bits one by one until the "bytes" is 0
 * the total steps is less than 32.
 */
int bits_count_while_loop(unsigned int number)
{
    int count = 0;
    while (number) {
        count += number & 1;
        number = number >> 1;
    }
    return count;
}
/* algorithm: check the rightmost bit recursively
 * the total steps is less than 32.
 */
int bits_count_recursive(unsigned int number)
{
    if (number == 0) {
        return 0;
    }
    return (number & 1) + bits_count_recursive(number >> 1);
}
/* algorithm: Brian Kernighan's
 */
int bits_count_kernighan(unsigned int number)
{
    int count = 0;
    while (number) {
        number &= number - 1;
        ++count;
    }
    return count;
}

/* algorithm: lookup table
 * only takes 3 steps without building the table.
 * the fastest method if the table is built in advance.
 */
int bits_count_lookup_table(unsigned int number)
{
    // the table can be generated statically before this call 
    int bits_set_table[256];

    bits_set_table[0] = 0;  
    for (int i = 0; i < 256; i++) 
    {  
        bits_set_table[i] = (i & 1) +  bits_set_table[i >> 1];  
    }

    // int count = bits_set_table[number & 0xff] +  
    //             bits_set_table[(number >> 8) & 0xff] +  
    //             bits_set_table[(number >> 16) & 0xff] +  
    //             bits_set_table[(number >> 24)];

    char *bytes = (char *)&number; // don't use loop, don't care the endian
    int count = bits_set_table[ bytes[0] ] +  
                bits_set_table[ bytes[1] ] +  
                bits_set_table[ bytes[2] ] +  
                bits_set_table[ bytes[3] ];  

    return count;
}
/* algorithm: pop count
 * very fast and no extra memory
 */
int bits_count_pop_count(unsigned int n)
{   
    n = n - ((n >> 1) & 0x55555555);
    n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
    n = ((n + (n >> 4) & 0x0F0F0F0F) * 0x1010101) >> 24;
    // the follow code can replace the above line
    // n = (n + (n >> 4)) & 0x0F0F0F0F;
    // n = n + (n >> 8);
    // n = n + (n >> 16);
    // n = n & 0x3F;
    return n;
}

/* C++ bitset<>::count()
 * need to include the libary <bitset>
 */
#include <bitset>

int bits_count_cpp_bitset(unsigned int number)
{
    bitset<32> bs(number);
    return bs.count();
}

/* testing driver code
 */
int main() 
{
    int bits_count = 0;
    unsigned int bytes = 0x5555;

    cout << "Count Bits, Input an Hex Integer: ";
    // cin >> hex >> bytes;
    // check signed integer, or only take unsigned integer

    cout.flags(ios::right | ios::hex | ios::showbase);
    cout << "number: " << bytes << endl;
    cout.setf(ios::dec, ios::basefield);

    bits_count = bits_count_for_loop(bytes);
    cout << "for_loop:  \t count = " << bits_count << endl;

    bits_count = bits_count_while_loop(bytes);
    cout << "while_loop: \t count = " << bits_count << endl;

    bits_count = bits_count_kernighan(bytes);
    cout << "kernighan: \t count = " << bits_count << endl;

    bits_count = bits_count_recursive(bytes);
    cout << "recursive: \t count = " << bits_count << endl;

    bits_count = bits_count_lookup_table(bytes);
    cout << "lookup_table: \t count = " << bits_count << endl;

    bits_count = bits_count_pop_count(bytes);
    cout << "pop_count:  \t count = " << bits_count << endl;

    bits_count = bits_count_cpp_bitset(bytes);
    cout << "cpp_bitset: \t count = " << bits_count << endl;

    cout << "cpp_builtin: \t count = " << __builtin_popcount(bytes) << endl;

    return 0;
} 