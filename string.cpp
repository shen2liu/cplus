// 
// C/C++ String
// 
//  String Matching Algorithms
//  - Naive Pattern
//  - Knuth-Morris-Pratt 
//  - Robin-Karp
//  - Boyer-Moore
//  - Boyer-Moore-Horspool  (TBD)
//  - Finite Automate
//  - Aho-Corasick (TBD)
//  - Z Array (TBD)
//
//
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**********************************************************************
 * Naive Pattern (Brute Force)
 * - scan the string one letter by one letter to match the pattern.
 * - can be improved when every letter in pattern is unique.
 * - time complexity: O(n*m)
 */
bool string_match_naive(string str_text, string str_pattern, vector<int>& results)
{
    int n = str_text.size();
    int m = str_pattern.size();
    int i, j;
    for (i = 0; i <= n - m; ++i ) {
        for (j = 0; j < m; ++j) {
            if (str_text[i + j] != str_pattern[j]) {
                break;
            }
        }
        if (j == m) {
            results.push_back(i);
        }
    }
    return results.size();
}
/**********************************************************************
 * Knuth-Morris-Pratt Algorithm
 * + preprocessing: generate the longest prefix suffix (LPS) for the 
 *   pattern which is also proper suffix (not whole string). 
 *   - set 0 in lps[] if the letter is shown first time, then go next
 *   - increase the value if the letters (prefix) are shown again.
 *     for example: "AAACAAAAAC", lps[] = [0, 1, 2, 0, 1, 2, 3, 3, 3, 4]
 *   - lps[] has the same size as the string str_pattern.
 * + searching pattern: 
 *   - increase both iteration index (i, j) if the letter matches.
 *   - go back on the pattern string based on the lps[]
 *   - save the index of the first matching letter when match is found.
 * + compare to the naive algorithn, the algorithm does not come back to 
 *   the next letter, goes back based on the lps[] values.
 * preprocessing time complexity: O(m)
 * matching time complexity: O(n*m)
 */
bool string_match_kmp(string str_text, string str_pattern, vector<int>& results)
{
    int n = str_text.size();
    int m = str_pattern.size();
    int lps[m];

    if (m <= 0 || n < m) {
        return false;
    }

    // calculate the longest prefix suffix
    lps[0] = 0;
    int j = 0;
    int i = 1;
    while (i < m) // one for double loops
    {
        if (str_pattern[i] == str_pattern[j]) {
            lps[i] = ++j;
            ++i;
        }
        else if (j == 0) {
            lps[i] = 0;
            ++i;
        }
        else {
            j = lps[j - 1]; // going backward
        }
    }

    // matching the pattern
    i = 0, j = 0;
    while (i < n) 
    {
        if (str_text[i] == str_pattern[j]) {
            ++i; ++j;
        }
        if (j == m) {
            results.push_back(i - j);
            j = lps[j - 1];
        }
        else if (i < n && str_pattern[j] != str_text[i]) {
            j == 0 ? ++i : j = lps[j - 1];
        }
    }
    return (results.size() != 0);
}
/**********************************************************************
 * Rabin-Karp Algorithm
 * + define the pattern string as p[m], the text string as t[n]
 *   - the search (matching) window has the same size of the pattern
 * + choose a hash function for string x[k]:
 *   - hash(x[k]) = (x[0]*d^(k-1) + x[1]*d^(k-2) + ... + x[k-1]*d^0) % q
 *   - d is the base, better <= 10, in case overflow.
 *   - q is a large (prime) number, try to reduce the hash collisions
 * + calculate the hash value of the pattern p = hash(p[m])
 *   calculate the hash value of the first window of the text t = hash(t[m])
 * + start the search (matching pattern) of the text, 
 *   - if the hash value of the text windoe is equal to the hush value
 *     of the pattern, start comparing the letter one by one.
 *   - contine to the next window if the hash values are not equal,
 *     re-calculate the hash value (rehash) the new window of the text.
 *     . rehash: remove the hash value of the first letter and add the 
 *               the hash value of the last letter.
 *       rehash(t[m]) = (t - t[i]*h) * d + t[i+m] // h = d^(m-1), i = first
 * + repeat the previous step to find all the matches, save to the vector.
 * + expected time complexity: O(m+n)
 */
const int d = 3;    // the base value
const int q = 97;   // a prime number

bool string_match_rabin_karp(string str_text, string str_pattern, vector<int>& results)
{
    int i, j;
    int n = str_text.size();
    int m = str_pattern.size();
    if (n < m) {
        return false;
    }

    // h = pow(d, m - 1)
    int h = 1;
    for (j = 0; j < m - 1; ++j) {
        h = (h * d) % q;
    }

    int p = 0;
    int t = 0;
    for (i = 0; i < m; ++i) {
        p = (d * p + str_pattern[i]) % q;
        t = (d * t + str_text[i]) % q;
    }

    for (i = 0; i <= n - m; ++i) {
        if (p == t) {
            for (j = 0; j < m; ++j) {
                if (str_text[i + j] != str_pattern[j]) {
                    break;
                }
            }
            if (j == m) {
                results.push_back(i);
            }
        }
        if (i < n - m) {
            t = (d*(t - str_text[i]*h) + str_text[i + m]) % q;
            if ( t < 0) {
                t = t + q;
            } 
        }
    }
    return (results.size() != 0);
}
/**********************************************************************
 * Boyer Moore Algorithm
 * + scans the pattern string P from the rightmost letter to left.
 * + uses two precomputed function to shift the text T window to right.
 *   - bad-character shift:
 *     upon mismatch, let b be the mismatched character in T (text). 
 *     skip alignments until b matches its opposite in P (pattern),
 *     or P moves past b.
 *   - good-suffix shift:
 *     let t be the substring of T that matched a suffix of P.
 *     skip alignments until:
 *     1) t matches opposite characters in P, or
 *     2) a prefix of P matches a suffix of t, or
 *     3) P moves past t, whichever happens first.
 * + time complexity: average O(n), best O(n/m), worst O(n*m)
 *   space complexity: O(T+m), T = size of ASCII table
 */
#include <algorithm>
const int num_chars = 256;  // standard ascii

void suffixes(string x, int *suff) 
{
    int m = x.size();
    int f, g, i;
    
    suff[m - 1] = m;
    g = m - 1;
    for (i = m - 2; i >= 0; --i) {
        if (i > g && suff[i + m - 1 - f] < i - g)
            suff[i] = suff[i + m - 1 - f];
        else {
            if (i < g)
                g = i;
            f = i;
            while (g >= 0 && x[g] == x[g + m - 1 - f])
                --g;
            suff[i] = f - g;
        }
    }
}
//
bool string_match_boyer_moore(string str_text, string str_pattern, vector<int>& results)
{
    int n = str_text.size();
    int m = str_pattern.size();

    int bad_chars[num_chars];
    int good_suffix[m];
  
    // preprocessing the bad character
    {
        for (int i = 0; i < num_chars; i++) {
            bad_chars[i] = -1;
        }
    
        for (int i = 0; i < m; i++) {
            bad_chars[str_pattern[i]] = i;
        }
    }

    // preprocessing the good suffix
    {
        int i, j, suffix[m];
    
        suffixes(str_pattern, suffix);
    
        for (i = 0; i < m; ++i) {
            good_suffix[i] = m;
        }
        j = 0;
        for (i = m - 1; i >= 0; --i) {
            if (suffix[i] == i + 1) {
                for (; j < m - 1 - i; ++j) {
                    if (good_suffix[j] == m) {
                        good_suffix[j] = m - 1 - i;
                    }
                }
            }
        }
        for (i = 0; i <= m - 2; ++i) {
            good_suffix[m - 1 - suffix[i]] = m - 1 - i;
        }
    }

    int s = 0; // s is shift of the pattern with respect to text
    while(s <= n - m)
    {
        int j = m - 1;
        while (j >= 0 && str_pattern[j] == str_text[s + j]) { --j; }

        if (j < 0)  
        {  
            results.push_back(s);
            s += good_suffix[0];
        }  
        else {
            s += max(good_suffix[j], j - bad_chars[str_text[s + j]]);
        }
    }
    return results.size();
}
/**********************************************************************
 * Finite Automate Algorithm
 * + every (unique) letter has m+1 states 0 to m, where m is the length
 *   of the pattern string.
 * + at the state k (0 <= k <= m), means the prefix from 0 to k of the 
 *   pattern have been found (matched), the whole pattern is found when
 *   reaching to the state m.
 * + the next state of the state k, we assume getting one of the letters
 *   in the pattern, then the length of the suffix (of the prefix pluses
 *   the letter we presumed to get) that matches with the prefix of the
 *   pattern is the next state value of the letter.
 * + to find the pattern match, we need to build the FA table first:
 *   e.g. the pattern is "aabac", the FA table is:
 *   state | a  b  c
 *     0   | 1  0  0
 *     1   | 2  0  0
 *     2   | 2  3  0
 *     3   | 4  0  0
 *     4   | 2  0  5
 *     5   | 1  0  0
 * + to find the pattern matches, go through the letters one by one from
 *   the beginning to the end, find the states based on the FA table,
 *   save the index (the beginning of the pattern) when the state is m.
 * + time complexity: O(n)
 *   time complexity of the preprocessing: O(m^3*|)
 */
static int finite_automate_next_state(string str_pattern, int state, int x) 
{
    int m = str_pattern.size();
    if (state < m && x == str_pattern[state]) {
        return state + 1;
    }
   int s, i;
   for (s = state; s > 0; s--) {
      if (str_pattern[s-1] == x) {
         for (i = 0; i < s-1; i++)
            if (str_pattern[i] != str_pattern[state-s+1+i])
               break;
         if (i == s-1)
            return s;
      }
   }
   return 0;
}
//
bool string_match_finite_automate(string str_text, string str_pattern, vector<int>& results)
{
    int m = str_pattern.size();
    int n = str_text.size();
    int FA[m + 1][num_chars] = { 0 };

    // construct the TF table
    for (int s = 0; s <= m; ++s) {
        for (int k = 0; k < num_chars; ++k) {
            FA[s][k] = finite_automate_next_state(str_pattern, s, k);
        }
    }

    // search the pattern
    int state = 0;
    for (int i = 0; i < n; ++i) {
        state = FA[state][str_text[i]];
        if (state == m) {
            results.push_back(i - m + 1);
        }
    }
    return results.size();
}
/**********************************************************************
 * C++ String find()
 * + use the method find() of the C++ class <string> to get the match
 */
bool string_match_cpp_find(string str_text, string str_pattern, vector<int>& results)
{
    int found = str_text.find(str_pattern); 
    while (found != string::npos) {
        results.push_back(found);
        found = str_text.find(str_pattern, found + 1);
    } 
}

/* testing driver code
 */
#define TESTING_STRING_MATCH(s, f) { \
    r.clear(); \
    cout << s << ": "; \
    f(txt, pat, r); \
    cout << "found " << r.size() << " matches = "; \
    for (int i = 0; i < r.size(); ++i) { cout << r[i] << ", "; } cout << endl; \
}

#include <regex>

int main()
{
    string txt = "aabaacaadaabaaba";
    string pat = "aaba";
    cout << "Search the pattern \"" << pat <<"\" in the string \"" << txt << "\"" << endl;

    vector<int> r;  // store the results

    TESTING_STRING_MATCH("Naive Pattern", string_match_naive);

    TESTING_STRING_MATCH("Knuth-Moris-Pratt", string_match_kmp);

    TESTING_STRING_MATCH("Rabin-Karp", string_match_rabin_karp);

    TESTING_STRING_MATCH("Boyer-Moore", string_match_boyer_moore);

    TESTING_STRING_MATCH("Finite Automata", string_match_finite_automate);

    TESTING_STRING_MATCH("C++ Find Method", string_match_cpp_find);

    regex e("(aaba)(.*)");
    if (regex_match(txt, e)) {
        cout << "C++ regex() found the pattern" << endl;
    }
}
