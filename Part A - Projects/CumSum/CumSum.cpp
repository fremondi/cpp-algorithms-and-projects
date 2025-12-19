#include <iostream>
#include <vector>
using namespace std; // Use the standard namespace

const int N = 40; // Replace #define with const

// Inline function definition for sum
inline void sum(int* p, int n, const vector<int>&d) {
    int i;
    *p = 0;
    for (i = 0; i < n; ++i)
        *p += d[i]; // Accumulate the sum
}

int main() {
    int accum = 0;

    // Use vector for dynamic arrays
    vector<int> data(N);

    // Initialize the vector
    for (int i = 0; i < N; ++i)
        data[i] = i;

    // Call the inline sum function
    sum(&accum, N, data);

    // Use C++ I/O instead of printf
    cout << "Sum is " << accum << endl;

    return 0;
}