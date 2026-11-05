
#ifndef CAMPUS_GRAPH_H
#define CAMPUS_GRAPH_H

#include <string>
#include <vector>

class CampusGraph {
private:
    struct Vertex {
        std::string name;
        std::vector<int> neighbors;
    };

    std::vector<Vertex> vertices;

    // Find a building without changing its stored spelling.
    int findBuildingIndex(const std::string& name) const;

    // Helper functions for input validation.
    static std::string trim(const std::string& text);
    static std::string normalize(const std::string& text);

    // Check whether an edge already exists.
    bool connectionExists(int first, int second) const;

public:
    CampusGraph();

    // Add buildings and undirected connections.
    bool addBuilding(const std::string& buildingName);
    bool addConnection(
        const std::string& buildingA,
        const std::string& buildingB
    );

    // Load data files.
    void loadBuildings(const std::string& filename);
    void loadConnections(const std::string& filename);

    // Milestone 1 features.
    void displayGraph() const;
    void displayGraphStatistics() const;
    void findReachableBuildings(const std::string& startName) const;

    // Final milestone features.
    void findShortestPath(
        const std::string& startName,
        const std::string& destinationName
    ) const;

    void displayConnectivityReport() const;

    // Useful for other parts of the program.
    int getBuildingCount() const;
    int getConnectionCount() const;
};

#endif
