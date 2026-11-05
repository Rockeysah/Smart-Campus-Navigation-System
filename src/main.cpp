#include <iostream>
#include "CampusGraph.h"

using namespace std;

int main() {

    CampusGraph campus;

    // Load in the required order.
    campus.loadBuildings("data/buildings.txt");
    campus.loadConnections("data/connections.txt");

    int choice;

    do {
        cout << "\n===== CAMPUS GRAPH MENU =====\n";
        cout << "1. View campus graph\n";
        cout << "2. Display graph counts and degrees\n";
        cout << "3. Find reachable buildings (BFS)\n";
        cout << "4. Find shortest path (BFS)\n";
        cout << "5. Graph connectivity report\n";
        cout << "6. Exit\n";

        cout << "Enter choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        cin.ignore(10000, '\n');

        switch (choice) {

            case 1:
                campus.displayGraph();
                break;

            case 2:
                campus.displayGraphStatistics();
                break;

            case 3: {
                string start;

                cout << "Enter starting building: ";
                getline(cin, start);

                campus.findReachableBuildings(start);
                break;
            }

            case 4: {
                string start;
                string destination;

                cout << "Enter starting building: ";
                getline(cin, start);

                cout << "Enter destination building: ";
                getline(cin, destination);

                campus.findShortestPath(start, destination);
                break;
            }

            case 5:
                campus.displayConnectivityReport();
                break;

            case 6:
                cout << "Exiting graph menu.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 6);

    return 0;
}