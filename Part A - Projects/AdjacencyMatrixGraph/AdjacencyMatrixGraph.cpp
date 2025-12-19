#include <iostream>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()

using namespace std;

// Function to generate a random probability between 0 and 1
double prob() {
    return static_cast<double>(rand()) / RAND_MAX;
}

// Function to check if the graph is connected
bool is_connected(bool* graph[], int size) {
    // Initialize variables
    int old_size = 0, c_size = 0;

    // Dynamically allocate memory for open and close arrays
    bool* close = new bool[size];
    bool* open = new bool[size];

    // Initialize all nodes as not open or closed
    for (int i = 0; i < size; ++i) {
        open[i] = close[i] = false;
    }

    // Start with the first node (index 0)
    open[0] = true;

    // Loop until the connected size equals the total size or no new nodes are added
    while (c_size < size) {
        // Iterate through all nodes
        for (int i = 0; i < size; ++i) {
            old_size = c_size;

            // If a node is open and not yet closed
            if (open[i] && close[i] == false) {
                close[i] = true;  // Mark it as closed
                c_size++;         // Increment the connected size

                // Check all neighbors of the current node
                for (int j = 0; j < size; ++j) {
                    open[j] = open[j] || graph[i][j]; // Mark neighbors as open
                }
            }
        }

        // If no new nodes were added during this iteration, the graph is not connected
        if (old_size == c_size) {
            delete[] close;
            delete[] open;
            return false; // Graph is not connected
        }
    }

    // Clean up dynamically allocated memory
    delete[] close;
    delete[] open;

    // If all nodes are connected, return true
    return true;
}

int main() {
    int size = 5; // Size of the graph (number of nodes)
    double density = 0.89; // Density of edges in the graph

    // Seed the random number generator
    srand(time(0));

    // Dynamically allocate a 2D array of booleans (adjacency matrix)
    bool** graph = new bool* [size];
    for (int i = 0; i < size; ++i) {
        graph[i] = new bool[size];
    }

    // Fill the adjacency matrix
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i == j) {
                graph[i][j] = false; // No self-loops
            }
            else {
                graph[i][j] = (prob() < density); // Random edge based on density
                graph[j][i] = graph[i][j]; // Symmetric for an undirected graph
            }
        }
    }

    // Print the adjacency matrix
    cout << "Adjacency Matrix:" << endl;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << graph[i][j] << " ";
        }
        cout << endl;
    }

    // Check if the graph is connected
    if (is_connected(graph, size)) {
        cout << "The graph is connected." << endl;
    }
    else {
        cout << "The graph is not connected." << endl;
    }

    // Deallocate memory
    for (int i = 0; i < size; ++i) {
        delete[] graph[i];
    }
    delete[] graph;

    return 0;
}

