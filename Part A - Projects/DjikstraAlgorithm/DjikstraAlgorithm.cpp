// DjikstraAlgorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//   
/*
This project implements a robust graph data structure in C++ using an adjacency matrix representation.
The implementation supports core functionalities such as adding/removing edges, checking adjacency,
retrieving neighbors, calculating edge values, and finding the shortest path using Dijkstra's algorithm.

Key learning points:
1. **Graph Representation**:
   The adjacency matrix provides a straightforward way to represent graphs, ensuring symmetry for undirected graphs.
2. **Dijkstra's Algorithm**:
   - Priority Queues: Implemented with a `std::priority_queue` to efficiently select the next node with the smallest distance.
     This ensures optimal performance when traversing the graph.
   - Avoiding Infinite Loops: Properly managed visited nodes and updated distances using vectors (`dist` and `previous`).
   - Exit Conditions: The `while` loop in Dijkstra’s algorithm terminates either when the destination node is reached or
     when the priority queue becomes empty, ensuring all reachable nodes are processed without unnecessary iterations.

Challenges and Insights:
- Efficient Priority Queue Management: Maintaining the correct order in the priority queue required careful updates to
  ensure only valid nodes were re-added with updated costs.
- Path Reconstruction: The `previous` vector was crucial for backtracking from the destination node to the source,
  allowing for correct path reconstruction without additional overhead.

Overall, this project deepened my understanding of priority queue operations, dynamic data management,
and implementing efficient algorithms with proper termination and loop conditions.
*/

#include <iostream>
#include <list>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iomanip> // For setprecision and fixed
#include <queue>
#include <limits>
#include <stack>

using namespace std;

class Graph {

private:
    int nodes;                              // Number of nodes
    vector<vector<double>> AdjacencyMatrix;   // Adjacency matrix representation
    double minWeight;                       // Minimum edge weight
    double maxWeight;                       // Maximum edge weight

public:
    // Default Constructor
    Graph() : nodes(0) {}

    // Constructor given # of nodes
    Graph(int n) : nodes(n), AdjacencyMatrix(n, vector<double>(n, 0.0)), minWeight(1.0), maxWeight(10.0) {}

    // Constructor given matrix
    Graph(const vector<vector<double>>& matrix)
        : nodes(matrix.size()), AdjacencyMatrix(matrix), minWeight(1.0), maxWeight(10.0) {
    }

    // Constructor given # of nodes, density, and min/max distance
    Graph(int n, double density, double min, double max) 
        : nodes(n), AdjacencyMatrix(n, vector<double>(n, 0.0)), minWeight(min), maxWeight(max) {
        srand(time(0));                      // Set random seed
        for (int i = 0; i < n; ++i) {                // Iterate over each node starting point
            for (int j = i; j < n; ++j) {        // Iterate over each potential arrival node (undirected graph)
                if (i != j) {      // Avoid self-loops
                    // Take into account given density, in order to get the right proportion of edges from each node
                    if ((static_cast<double>(rand()) / RAND_MAX) < density) {
                        double random_edge = (static_cast<double>(rand()) * (max - min)) / RAND_MAX + min; // Scale in [min, max]
                        double rounded_edge = round(random_edge * 10) / 10; // Round up number to one decimal
                        AdjacencyMatrix[i][j] = rounded_edge;   // i->j edge
                        AdjacencyMatrix[j][i] = rounded_edge;   // j->i edge (symmetric graph)
                    }
                }
            }
        }
    }

    // Constructor Copy
    Graph(const Graph& g2) : nodes(g2.nodes), AdjacencyMatrix(g2.AdjacencyMatrix) {}

    ~Graph() {}

    int V() const; // Number of vertices
    int E() const; // Number of edges
    bool adjacent(int x, int y) const; // Check edge existence between two nodes
    void neighbors(int x) const; // Print all the nodes linked to the given one
    void add(int x, int y); // Add an edge where it does not exist
    void removeEdge(int x, int y); // Remove an existing edge
    double get_edge_value(int x, int y) const; // Return value of an edge
    void set_edge_value(int x, int y, double v); // Set value of an edge
    void print() const; // Print the Graph
    void Dijkstra(int i, int j) const; // Algorithm Method
};


// Return number of vertices in the Graph
int Graph::V() const {
    return nodes;
}

// Return number of edges in the Graph
int Graph::E() const {
    int e = 0;
    for (int i = 0; i < nodes; ++i) {
        for (int j = i + 1; j < nodes; ++j) { // Only count upper triangle
            if (AdjacencyMatrix[i][j] > 0.0) {
                e += 1;
            }
        }
    }
    return e;
}

// Return (false) true if an edge (does not) exists
bool Graph::adjacent(int x, int y) const {
    return AdjacencyMatrix[x][y] > 0.0; // Directly return the condition
}

// Print nodes linked to a given one
void Graph::neighbors(int x) const {
    cout << "The nodes linked to " << x << " are the following: " << endl;
    for (int y = 0; y < AdjacencyMatrix[x].size(); ++y) {
        if (AdjacencyMatrix[x][y] > 0.0) {
            cout << y << " " << endl;
        }
    }
    cout << endl;
}

// Add edge between x and y
void Graph::add(int x, int y) {
    // Check if edge already exists
    if (AdjacencyMatrix[x][y] != 0.0) {
        cout << "An edge between " << x << " and " << y << " already exists." << endl;
        return;
    }
    double sum = 0.0;
    int c = 0;
    // Compute the average weight of existing edges from node x
    for (int j = 0; j < AdjacencyMatrix[x].size(); ++j) {
        if (AdjacencyMatrix[x][j] > 0.0) {
            sum += AdjacencyMatrix[x][j];
            c += 1;
        }
    }
    // Handle the case where no previous edges exist
    double average_edge;
    if (c > 0) {
        average_edge = sum / nodes; // Average of existing edges
    }
    else {
        // Generate a random number within [minWeight, maxWeight] for the edge weight
        average_edge = (static_cast<double>(rand()) * (maxWeight - minWeight)) / RAND_MAX + minWeight;
    }
    // Add the new edge
    AdjacencyMatrix[x][y] = average_edge;
    AdjacencyMatrix[y][x] = average_edge; // Ensure symmetry for an undirected graph
    cout << "An edge between " << x << " and " << y << " has been created. "
        << "Its weight is " << average_edge << " (calculated or randomly generated)." << endl;
}


// Remove edge between two nodes
void Graph::removeEdge(int x, int y) {
    if (AdjacencyMatrix[x][y] == 0.0) {
        cout << "No edge between " << x << " and " << y << " exist" << endl;
        return;
    }
    else {
        AdjacencyMatrix[x][y] = 0.0;
        AdjacencyMatrix[y][x] = 0.0;
    }
}

// Return edge value between two nodes
double Graph::get_edge_value(int x, int y) const {
    return AdjacencyMatrix[x][y];
}

// Set edge value between two nodes
void Graph::set_edge_value(int x, int y, double v) {
    AdjacencyMatrix[x][y] = v;
    AdjacencyMatrix[y][x] = v;
    cout << "Edge value between " << x << " and " << y << " set to " << v << "." << endl;
}

// Print the Adjacency Matrix
void Graph::print() const {
    cout << "Adjacency Matrix: " << endl;
    for (const auto& row : AdjacencyMatrix) {      // Iterate over each row
        for (double value : row) {                 // Iterate over each value in the row
            cout << fixed << setprecision(1) << value << "   "; // Round up to 1 decimal
        }
        cout << endl;
    }
}

void Graph::Dijkstra(int i, int j) const {
    // Get the number of nodes in the adjacency matrix
    int n = AdjacencyMatrix.size();

    // Validate the input node indices
    if (i < 0 || j < 0 || i >= n || j >= n) {
        cerr << "Invalid node indices." << endl;
        return;
    }

    // Initialize distance vector with infinity for all nodes
    vector<double> dist(n, numeric_limits<double>::infinity());
    // Initialize the previous node vector to reconstruct paths
    vector<int> previous(n, -1);
    // Set the distance to the source node as 0
    dist[i] = 0.0;

    // Define a priority queue to store nodes and their current costs
    // NodeCost is a pair where the first element is the cost, and the second is the node index
    using NodeCost = pair<double, int>;
    priority_queue<NodeCost, vector<NodeCost>, greater<>> pq;

    // Add the source node to the priority queue with a cost of 0
    pq.push({ 0.0, i });

    // Main loop: process nodes in the priority queue
    while (!pq.empty()) {
        // Extract the node with the smallest cost
        double current_cost = pq.top().first;
        int current_node = pq.top().second;
        pq.pop();

        // If the destination node is reached, stop processing
        if (current_node == j) break;

        // Explore all neighbors of the current node
        for (int neighbor = 0; neighbor < n; ++neighbor) {
            // Get the cost of the edge from the current node to the neighbor
            double edge_cost = AdjacencyMatrix[current_node][neighbor];

            // If the edge exists (cost > 0), calculate the potential new cost
            if (edge_cost > 0.0) {
                double new_cost = current_cost + edge_cost;

                // If the new cost is lower than the current known cost, update it
                if (new_cost < dist[neighbor]) {
                    dist[neighbor] = new_cost;          // Update the shortest distance to the neighbor
                    previous[neighbor] = current_node; // Update the previous node for path reconstruction
                    pq.push({ new_cost, neighbor });   // Add the neighbor to the priority queue
                }
            }
        }
    }

    // If the destination node is still unreachable, print a message and return
    if (dist[j] == numeric_limits<double>::infinity()) {
        cout << "No path exists from " << i << " to " << j << "." << endl;
        return;
    }

    // Print the shortest path cost to the destination node
    cout << "The shortest path cost from " << i << " to " << j << " is " << dist[j] << "." << endl;

    // Reconstruct the shortest path from source to destination
    stack<int> path; // Use a stack to store the path (reverses the order)
    for (int at = j; at != -1; at = previous[at]) {
        path.push(at); // Traverse the `previous` vector backward from destination to source
    }

    // Print the reconstructed path
    cout << "Path: ";
    while (!path.empty()) {
        cout << path.top() << (path.size() > 1 ? " -> " : ""); // Print nodes in the path
        path.pop();
    }
    cout << endl;
}





int main()
{

    Graph g1(50, 0.4, 1.0, 10.0);
    cout << "Graph 1:" << endl;
    g1.print();

    // Test Dijkstra's algorithm from node 15 to node 35
    int source = 15; // Starting node
    int destination = 35; // Destination node
    cout << "\nRunning Dijkstra's Algorithm on Graph 1 from node " << source << " to node " << destination << ":\n";
    g1.Dijkstra(source, destination);


    Graph g2(50, 0.2, 1.0, 10.0);
    cout << "Graph 2:" << endl;
    g2.print();

    // Test Dijkstra's algorithm from node 11 to node 43
    int source_2 = 11; // Starting node
    int destination_2 = 43; // Destination node
    cout << "\nRunning Dijkstra's Algorithm on Graph 2 from node " << source_2 << " to node " << destination_2 << ":\n";
    g2.Dijkstra(source_2, destination_2);

    // Testing all the other methods of the Graph class

    // Initialize a graph with 5 nodes
    Graph G(5);
    G.print();

    cout << "Number of vertices in the graph: " << G.V() << endl;
    cout << "Number of edges in the graph: " << G.E() << endl;

    // Adding edges
    cout << "\nAdding edges..." << endl;
    G.add(0, 1);
    G.add(0, 2);
    G.add(1, 3);
    G.add(3, 4);

    // Print number of edges after adding
    cout << "Number of edges after adding: " << G.E() << endl;

    // Test adjacency
    cout << "\nTesting adjacency:" << endl;
    cout << "Are 0 and 1 adjacent? " << (G.adjacent(0, 1) ? "Yes" : "No") << endl;
    cout << "Are 0 and 3 adjacent? " << (G.adjacent(0, 3) ? "Yes" : "No") << endl;

    // Print neighbors of each node
    cout << "\nPrinting neighbors:" << endl;
    for (int i = 0; i < G.V(); ++i) {
        G.neighbors(i);
    }

    // Set and get edge values
    cout << "\nSetting and retrieving edge values..." << endl;
    G.set_edge_value(0, 1, 5.5);
    cout << "Edge value between 0 and 1: " << G.get_edge_value(0, 1) << endl;

    G.set_edge_value(3, 4, 2.2);
    cout << "Edge value between 3 and 4: " << G.get_edge_value(3, 4) << endl;

    // Remove edges
    cout << "\nRemoving an edge..." << endl;
    G.removeEdge(0, 1);
    cout << "Number of edges after removal: " << G.E() << endl;

    // Check adjacency after removal
    cout << "Are 0 and 1 adjacent after removal? " << (G.adjacent(0, 1) ? "Yes" : "No") << endl;

    return 0;

}
