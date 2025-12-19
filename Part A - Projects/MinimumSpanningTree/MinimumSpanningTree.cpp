// MinimumSpanningTree.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <fstream>
#include <iomanip>

using namespace std;

class Graph {
private:
    int nodes;
    vector<vector<double>> AdjacencyMatrix;

public:
    // Default Constructor
    Graph() : nodes(0) {}

    // Constructor given # of nodes
    Graph(int n) : nodes(n), AdjacencyMatrix(n, vector<double>(n, 0.0)) {}

    // Constructor to read from a file
    Graph(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error: Cannot open file!" << endl;
            return;
        }

        file >> nodes;
        AdjacencyMatrix.assign(nodes, vector<double>(nodes, 0.0));

        int i, j;
        double cost;
        while (file >> i >> j >> cost) {
            AdjacencyMatrix[i][j] = cost;
            AdjacencyMatrix[j][i] = cost;
        }

        file.close();
    }

    // Function to print adjacency matrix
    void printGraph() const {
        cout << "Adjacency Matrix:\n";
        for (const auto& row : AdjacencyMatrix) {
            for (double weight : row) {
                cout << fixed << setprecision(2) << weight << "\t";
            }
            cout << endl;
        }
    }

    // Prim's Algorithm for Minimum Spanning Tree (MST)
    void primMST() {
        if (nodes == 0) {
            cout << "Graph is empty!\n";
            return;
        }

        vector<double> key(nodes, numeric_limits<double>::max()); // Store min edge weights
        vector<int> parent(nodes, -1); // Stores MST structure
        vector<bool> inMST(nodes, false); // Track included nodes

        // Min-heap to pick min weight edge
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<>> pq;

        // Start with node 0
        key[0] = 0;
        pq.push({ 0, 0 });

        while (!pq.empty()) {
            int u = pq.top().second; // Get the node with the smallest weight
            pq.pop();

            if (inMST[u]) continue; // If already in MST, skip
            inMST[u] = true;

            // Examine adjacent nodes
            for (int v = 0; v < nodes; ++v) {
                double weight = AdjacencyMatrix[u][v];
                if (weight > 0 && !inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push({ key[v], v });
                }
            }
        }

        // Print MST
        cout << "Edges in Minimum Spanning Tree:\n";
        double totalCost = 0;
        for (int i = 1; i < nodes; ++i) {
            if (parent[i] != -1) {
                cout << parent[i] << " - " << i << " | Weight: " << AdjacencyMatrix[i][parent[i]] << endl;
                totalCost += AdjacencyMatrix[i][parent[i]];
            }
        }
        cout << "Total MST Cost: " << totalCost << endl;
    }
};

int main() {
    Graph g("SampleTestData_mst_data.txt"); // Read graph from file
    g.printGraph();  // Print adjacency matrix
    g.primMST();     // Run Prim's algorithm and output MST
    return 0;
}
