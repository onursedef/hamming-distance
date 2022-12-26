#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <numeric>

using namespace std;

// Calculates the Hamming distance between two binary vectors
int hammingDistance(vector<int> v1, vector<int> v2) {
    int distance = 0;
    for (int i = 0; i < v1.size(); i++) {
        if (v1[i] != v2[i]) {
            distance++;
        }
    }
    return distance;
}


// Generates all possible binary vectors of length n
vector<vector<int>> generateVectors(int n) {
    if (n == 0) {
        return {{}};
    } else {
        vector<vector<int>> prevVectors = generateVectors(n-1);
        vector<vector<int>> vectors;
        for (vector<int> v : prevVectors) {
            vectors.push_back(v);
            v.push_back(1);
            vectors.push_back(v);
        }
        return vectors;
    }
}


// Class to represent a graph
class Graph {
public:
    // Constructor
    Graph() {}

    // Adds an edge between the given vertices
    void addEdge(int v1, int v2) {
        adjacencyList[v1].push_back(v2);
        adjacencyList[v2].push_back(v1);
    }

    // Returns the number of vertices in the graph
    int numVertices() const {
        return adjacencyList.size();
    }

    // Returns true if there is an edge between the given vertices, false otherwise
    bool hasEdge(int v1, int v2) const {
        for (int w : adjacencyList.at(v1)) {
            if (w == v2) {
                return true;
            }
        }
        return false;
    }

private:
    map<int, vector<int>> adjacencyList;
};


// Generates the Hamming graph H(n,d)
Graph generateGraph(int n, int d) {
    Graph graph;
    vector<vector<int>> vectors = generateVectors(n);
    for (int i = 0; i < vectors.size(); i++) {
        for (int j = i+1; j < vectors.size(); j++) {
            if (hammingDistance(vectors[i], vectors[j]) <= d) {
                graph.addEdge(i, j);
            }
        }
    }
    return graph;
}


// Recursive function to find the maximum clique in the graph using the Bron–Kerbosch algorithm
void bronKerbosch(Graph &graph, vector<int> &R, vector<int> &P, vector<int> &X, vector<int> &maximumClique) {

    if (P.empty() && X.empty()) {
        if (R.size() > maximumClique.size()) {
            maximumClique = R;
        }
        return;
    }
    int u = -1;
    if (!P.empty()) {
        u = P.back();
        P.pop_back();
    }
    while (!P.empty()) {
        int v = P.back();
        P.pop_back();
        R.push_back(v);
        vector<int> newP, newX;
        for (int w : P) {
            if (graph.hasEdge(v, w)) {
                newP.push_back(w);
            }
        }
        for (int w : X) {
            if (graph.hasEdge(v, w)) {
                newX.push_back(w);
            }
        }
        bronKerbosch(graph, R, newP, newX, maximumClique);
        R.pop_back();
        X.push_back(v);
    }
    if (u != -1) {
        P.push_back(u);
    }
}

// Function to find the maximal number of binary vectors of length n
// such that any two distinct vectors have a Hamming distance 2d
int findMaxVectors(int n, int d) {
    // Base case: if n == 0, return 1
    if (n == 0) return 1;

    // Recursive case: consider all possible subsets of binary vectors
    // of length n and check if the distance between all pairs of vectors
    // in the subset is at least 2 * d
    int maxVectors = 0;
    for (int i = 0; i < (1 << n); i++) {
        // Create a vector to hold the binary vectors in the current subset
        vector<vector<int>> vectors;
        for (int j = 0; j < n; j++) {
            // Create a new vector to represent the current binary vector
            vector<int> v;
            for (int k = 0; k < n; k++) {
                // If the kth bit of i is set, add a 1 to the vector
                if (i & (1 << k)) {
                    v.push_back(1);
                }
                    // Otherwise, add a 0 to the vector
                else {
                    v.push_back(0);
                }
            }
            vectors.push_back(v);
        }

        // Check if the distance between all pairs of vectors in the subset
        // is at least 2 * d
        bool validSubset = true;
        for (int j = 0; j < vectors.size(); j++) {
            for (int k = 0; k < vectors.size(); k++) {
                int distance = 0;
                for (int l = 0; l < n; l++) {
                    if (vectors[j][l] != vectors[k][l]) distance++;
                }
                if (distance < 2 * d) {
                    validSubset = false;
                    break;
                }
            }
            if (!validSubset) break;
        }

        // If the subset is valid, update the maximum number of vectors
        if (validSubset) {
            maxVectors = max(maxVectors, (int)vectors.size());
        }
    }
    return maxVectors;
}


int main() {
    // Set n and d to 5 and 4, respectively
    int n = 5;
    int d = 4;

    // Generate the Hamming graph H(5,4)
    Graph graph = generateGraph(n, d);
    //graph.printAdjacencyList();

    // Initialize the maximum clique to the empty set
    vector<int> maximumClique;
    vector<int> R, X;
    vector<int> P(graph.numVertices());
    std::iota(P.begin(), P.end(), 0);


    // Find the maximum clique in the graph using the Bron–Kerbosch algorithm
    bronKerbosch(graph, R, P, X, maximumClique);

    // Print the size of the maximum clique
    cout << "A(5,4) = " << maximumClique.size() << endl;

    return 0;
}
