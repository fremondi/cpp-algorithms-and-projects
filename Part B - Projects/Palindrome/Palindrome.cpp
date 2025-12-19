// Palindrome.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <numeric> // For Accumulate, inner product
#include <string>

using namespace std;

template <typename Bidirectional>
bool isPalindrome(Bidirectional first, Bidirectional last)
{
    while (true) {
        --last;
        if (first == last)  // Middle of odd-length sequence
            break;
        if (*first != *last)  // Mismatch found
            return false;
        ++first;
        if (first == last)  // Middle of even-length sequence
            break;
    }
    return true;
}

template <typename RandomAccess>
RandomAccess pickRandEl(RandomAccess first, RandomAccess last)
{
    ptrdiff_t temp = last - first;                    // Get range size
    return first + rand() % temp;                     // Return random offset from start
}

template <class InputIter, class T>
InputIter find(InputIter b, InputIter e, const T& t);

template <class InputIter, class Predicate>
InputIter find_if(InputIter b, InputIter e, Predicate p);

int main() {
    string s = "radar";
    if (isPalindrome(s.begin(), s.end()))
        cout << s << " is a palindrome!\n"; 

    vector<int> v = { 10, 20, 30, 40, 50 };
    auto it = pickRandEl(v.begin(), v.end());
    cout << *it << endl;  // Prints a random element

    vector<int> v = { 1, 3, 5, 7, 9 };
    auto it = find(v.begin(), v.end(), 5);        // matches value
    auto it2 = find_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }); // matches condition


    string words[5] = { "my", "hop", "mop", "hope", "cope" };
    string* where;

    where = find(words, words + 5, "hop");
    cout << *++where << endl;   // Outputs: mop

    sort(words, words + 5);     // Sorts array alphabetically

    where = find(words, words + 5, "hop");
    cout << *++where << endl;   // Outputs: hope

    double v1[3] = { 1.0, 2.5, 4.6 };
    double v2[3] = { 1.0, 2.0, -3.5 };
    double sum, inner_p;

    sum = accumulate(v1, v1 + 3, 0.0);  // sum = 1.0 + 2.5 + 4.6 = 8.1

    inner_p = inner_product(v1, v1 + 3, v2, 0.0);
    // = (1.0 * 1.0) + (2.5 * 2.0) + (4.6 * -3.5)
    // = 1.0 + 5.0 - 16.1 = -10.1

    cout << "sum = " << sum << ", product = " << inner_p << endl;

    int data[3] = { 9, 10, 11 };

    print(data, data + 3, "Original values");

    transform(data, data + 3, data, bind2nd(times<int>(), 2));

    print(data, data + 3, "New values");

    return 0;
}