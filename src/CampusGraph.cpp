
#include "CampusGraph.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>

using namespace std;

CampusGraph::CampusGraph() {
}

// Remove whitespace from the beginning and end of a string.
string CampusGraph::trim(const string& text) {
    size_t start = 0;
    size_t end = text.size();

    while (start < end &&
           isspace(static_cast<unsigned char>(text[start]))) {
        start++;
    }

    while (end > start &&
           isspace(static_cast<unsigned char>(text[end - 1]))) {
        end--;
    }

    return text.substr(start, end - start);
}

// Convert a string to lowercase for case-insensitive comparison.
string CampusGraph::normalize(const string& text) {
    string result = trim(text);

    for (char& character : result) {
        character = static_cast<char>(
            tolower(static_cast<unsigned char>(character))
        );
    }

    return result;
}

// Find a building by name.
// Returns -1 if the building does not exist.
int CampusGraph::findBuildingIndex(const string& name) const {
    string target = normalize(name);

    for (size_t i = 0; i < vertices.size(); i++) {
        if (normalize(vertices[i].name) == target) {
            return static_cast<int>(i);
        }
    }

    return -1;
}

// Check whether two buildings are already connected.
bool CampusGraph::connectionExists(int first, int second) const {
    for (int neighbor : vertices[first].neighbors) {
        if (neighbor == second) {
            return true;
        }
    }

    return false;
}

// Add a building to the graph.
bool CampusGraph::addBuilding(const string& buildingName) {
    string name = trim(buildingName);

    if (name.empty()) {
        return false;
    }

    // Do not accept duplicate building names.
    if (findBuildingIndex(name) != -1) {
        return false;
    }

    Vertex newVertex;
    newVertex.name = name;

    vertices.push_back(newVertex);

    return true;
}

// Add an undirected connection between two buildings.
bool CampusGraph::addConnection(
    const string& buildingA,
    const string& buildingB
) {
    int first = findBuildingIndex(buildingA);
    int second = findBuildingIndex(buildingB);

    // Both buildings must exist.
    if (first == -1 || second == -1) {
        return false;
    }

    // A building cannot connect to itself.
    if (first == second) {
        return false;
    }

    // Do not accept duplicate connections.
    if (connectionExists(first, second)) {
        return false;
    }

    // Undirected edge: add both directions.
    vertices[first].neighbors.push_back(second);
    vertices[second].neighbors.push_back(first);

    return true;
}

// Load buildings from buildings.txt.
void CampusGraph::loadBuildings(const string& filename) {
    ifstream inputFile(filename);

    vertices.clear();

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file: "
             << filename << endl;

        cout << "Treating this file as empty." << endl;
        cout << "Accepted: 0 | Rejected: 0" << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (getline(inputFile, line)) {
        lineNumber++;

        string name = trim(line);

        // Ignore blank lines.
        if (name.empty()) {
            continue;
        }

        // Ignore comment lines.
        if (name[0] == '#') {
            continue;
        }

        string reason;

        if (name.find('|') != string::npos) {
            reason = "Building name contains an invalid delimiter";
        }
        else if (findBuildingIndex(name) != -1) {
            reason = "Duplicate building name";
        }

        if (!reason.empty()) {
            cout << filename << ", line " << lineNumber
                 << ": Rejected - " << reason << endl;
            rejected++;
            continue;
        }

        if (addBuilding(name)) {
            accepted++;
        }
        else {
            cout << filename << ", line " << lineNumber
                 << ": Rejected - Invalid building name" << endl;
            rejected++;
        }
    }

    inputFile.close();

    cout << "\nBuilding file: " << filename << endl;
    cout << "Accepted: " << accepted
         << " | Rejected: " << rejected << endl;
}

// Load connections from connections.txt.
void CampusGraph::loadConnections(const string& filename) {
    ifstream inputFile(filename);

    // Clear old connections before loading.
    for (Vertex& vertex : vertices) {
        vertex.neighbors.clear();
    }

    if (!inputFile.is_open()) {
        cout << "Error: Could not open file: "
             << filename << endl;

        cout << "Treating this file as empty." << endl;
        cout << "Accepted: 0 | Rejected: 0" << endl;
        return;
    }

    string line;
    int lineNumber = 0;
    int accepted = 0;
    int rejected = 0;

    while (getline(inputFile, line)) {
        lineNumber++;

        string cleanedLine = trim(line);

        // Ignore blank lines and comments.
        if (cleanedLine.empty() || cleanedLine[0] == '#') {
            continue;
        }

        size_t delimiter = cleanedLine.find('|');

        string reason;

        if (delimiter == string::npos) {
            reason = "Missing | delimiter";
        }
        else if (cleanedLine.find('|', delimiter + 1)
                 != string::npos) {
            reason = "Too many fields";
        }
        else {
            string buildingA =
                trim(cleanedLine.substr(0, delimiter));

            string buildingB =
                trim(cleanedLine.substr(delimiter + 1));

            if (buildingA.empty() || buildingB.empty()) {
                reason = "Building name cannot be empty";
            }
            else {
                int first = findBuildingIndex(buildingA);
                int second = findBuildingIndex(buildingB);

                if (first == -1 || second == -1) {
                    reason = "Unknown building reference";
                }
                else if (first == second) {
                    reason = "Self-connection is not allowed";
                }
                else if (connectionExists(first, second)) {
                    reason = "Duplicate connection";
                }
                else if (addConnection(buildingA, buildingB)) {
                    accepted++;
                }
                else {
                    reason = "Invalid connection";
                }
            }
        }

        if (!reason.empty()) {
            cout << filename << ", line " << lineNumber
                 << ": Rejected - " << reason << endl;
            rejected++;
        }
    }

    inputFile.close();

    cout << "\nConnection file: " << filename << endl;
    cout << "Accepted: " << accepted
         << " | Rejected: " << rejected << endl;
}

// Display each building and its neighbors.
void CampusGraph::displayGraph() const {
    cout << "\n========== CAMPUS GRAPH ==========\n";

    if (vertices.empty()) {
        cout << "No buildings available.\n";
        return;
    }

    for (size_t i = 0; i < vertices.size(); i++) {
        cout << vertices[i].name << " -> ";

        if (vertices[i].neighbors.empty()) {
            cout << "No connections";
        }
        else {
            for (size_t j = 0;
                 j < vertices[i].neighbors.size(); j++) {

                cout << vertices[
                    vertices[i].neighbors[j]
                ].name;

                if (j + 1 < vertices[i].neighbors.size()) {
                    cout << ", ";
                }
            }
        }

        cout << endl;
    }
}

// Return the number of buildings.
int CampusGraph::getBuildingCount() const {
    return static_cast<int>(vertices.size());
}

// Return the number of undirected connections.
// Each connection appears twice in the adjacency list,
// so divide the total degree by 2.
int CampusGraph::getConnectionCount() const {
    int totalDegrees = 0;

    for (const Vertex& vertex : vertices) {
        totalDegrees += static_cast<int>(
            vertex.neighbors.size()
        );
    }

    return totalDegrees / 2;
}

// Display graph counts and the degree of each building.
void CampusGraph::displayGraphStatistics() const {
    cout << "\n========== GRAPH STATISTICS ==========\n";

    cout << "Total buildings: " << getBuildingCount() << endl;
    cout << "Total connections: " << getConnectionCount() << endl;

    if (vertices.empty()) {
        cout << "No building degrees available.\n";
        return;
    }

    cout << "\nBuilding degrees:\n";

    for (const Vertex& vertex : vertices) {
        cout << vertex.name
             << ": " << vertex.neighbors.size()
             << endl;
    }
}

// Breadth-First Search.
// Find all buildings reachable from the starting building.
void CampusGraph::findReachableBuildings(
    const string& startName
) const {
    cout << "\n========== BFS REACHABILITY ==========\n";

    int start = findBuildingIndex(startName);

    if (start == -1) {
        cout << "Error: Starting building not found.\n";
        return;
    }

    vector<bool> visited(vertices.size(), false);
    vector<int> distance(vertices.size(), -1);

    queue<int> bfsQueue;

    // Mark visited when adding to the queue.
    visited[start] = true;
    distance[start] = 0;
    bfsQueue.push(start);

    int reachableCount = 0;

    cout << "Starting building: "
         << vertices[start].name << endl;

    cout << "\nReachable buildings:\n";

    while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();

        cout << vertices[current].name
             << " | Distance: "
             << distance[current] << endl;

        reachableCount++;

        // Visit neighbors in insertion order.
        for (int neighbor : vertices[current].neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                distance[neighbor] = distance[current] + 1;

                bfsQueue.push(neighbor);
            }
        }
    }

    cout << "\nTotal reachable buildings: "
         << reachableCount << endl;

    cout << "Total buildings in graph: "
         << vertices.size() << endl;
}

// Find the shortest path between two buildings using BFS.
void CampusGraph::findShortestPath(
    const string& startName,
    const string& destinationName
) const {
    cout << "\n========== SHORTEST PATH ==========\n";

    int start = findBuildingIndex(startName);
    int destination = findBuildingIndex(destinationName);

    if (start == -1) {
        cout << "Error: Starting building not found.\n";
        return;
    }

    if (destination == -1) {
        cout << "Error: Destination building not found.\n";
        return;
    }

    vector<bool> visited(vertices.size(), false);
    vector<int> parent(vertices.size(), -1);
    vector<int> distance(vertices.size(), -1);

    queue<int> bfsQueue;

    visited[start] = true;
    distance[start] = 0;
    bfsQueue.push(start);

    while (!bfsQueue.empty()) {
        int current = bfsQueue.front();
        bfsQueue.pop();

        // Destination has been reached.
        if (current == destination) {
            break;
        }

        for (int neighbor : vertices[current].neighbors) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                parent[neighbor] = current;
                distance[neighbor] = distance[current] + 1;

                bfsQueue.push(neighbor);
            }
        }
    }

    if (!visited[destination]) {
        cout << "No path exists between "
             << vertices[start].name << " and "
             << vertices[destination].name << ".\n";
        return;
    }

    // Reconstruct the path by following parent indexes.
    vector<int> path;

    int current = destination;

    while (current != -1) {
        path.push_back(current);
        current = parent[current];
    }

    reverse(path.begin(), path.end());

    cout << "Shortest path:\n";

    for (size_t i = 0; i < path.size(); i++) {
        cout << vertices[path[i]].name;

        if (i + 1 < path.size()) {
            cout << " -> ";
        }
    }

    cout << "\nNumber of connections: "
         << distance[destination] << endl;
}

// Display connected components, isolated buildings,
// and all buildings with the highest degree.
void CampusGraph::displayConnectivityReport() const {
    cout << "\n========== CONNECTIVITY REPORT ==========\n";

    int buildingCount = getBuildingCount();
    int connectionCount = getConnectionCount();

    cout << "Total buildings: " << buildingCount << endl;
    cout << "Total connections: " << connectionCount << endl;

    if (vertices.empty()) {
        cout << "No buildings available.\n";
        cout << "Connected components: 0\n";
        return;
    }

    // Find all buildings with the highest degree.
    size_t highestDegree = 0;

    for (const Vertex& vertex : vertices) {
        highestDegree = max(
            highestDegree,
            vertex.neighbors.size()
        );
    }

    cout << "\nHighest degree: " << highestDegree << endl;
    cout << "Building(s) with highest degree:\n";

    for (const Vertex& vertex : vertices) {
        if (vertex.neighbors.size() == highestDegree) {
            cout << "- " << vertex.name << endl;
        }
    }

    // Find isolated buildings.
    cout << "\nIsolated buildings:\n";

    bool foundIsolated = false;

    for (const Vertex& vertex : vertices) {
        if (vertex.neighbors.empty()) {
            cout << "- " << vertex.name << endl;
            foundIsolated = true;
        }
    }

    if (!foundIsolated) {
        cout << "None\n";
    }

    // Find connected components using BFS.
    vector<bool> visited(vertices.size(), false);

    int componentCount = 0;

    cout << "\nConnected components:\n";

    for (size_t i = 0; i < vertices.size(); i++) {
        if (visited[i]) {
            continue;
        }

        componentCount++;

        queue<int> bfsQueue;

        visited[i] = true;
        bfsQueue.push(static_cast<int>(i));

        cout << "Component " << componentCount << ": ";

        bool first = true;

        while (!bfsQueue.empty()) {
            int current = bfsQueue.front();
            bfsQueue.pop();

            if (!first) {
                cout << ", ";
            }

            cout << vertices[current].name;
            first = false;

            for (int neighbor : vertices[current].neighbors) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    bfsQueue.push(neighbor);
                }
            }
        }

        cout << endl;
    }

    cout << "\nTotal connected components: "
         << componentCount << endl;

    if (componentCount == 1) {
        cout << "The graph is connected.\n";
    }
    else {
        cout << "The graph is not connected.\n";
    }
}
