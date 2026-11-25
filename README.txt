SMART CAMPUS NAVIGATION AND FACILITY ANALYTICS SYSTEM

1. PROJECT PURPOSE

The Smart Campus Navigation and Facility Analytics System is a C++ program that manages campus buildings, connections, departments, and facilities.

The system uses three main data structures:
- Graph with Breadth-First Search (BFS) for campus navigation.
- General Tree for department organization.
- Hash Table with separate chaining for facility lookup.

2. REQUIREMENTS

- C++ compiler supporting C++17.
- Terminal or command-line environment.

3. HOW TO COMPILE

Open a terminal in the project directory and run:

g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o campus_system

4. HOW TO RUN

Run:

./campus_system

The program loads its input files from the data directory by default.

5. INPUT FILES

The data directory contains:

- buildings.txt — campus building names.
- connections.txt — connections between buildings.
- departments.txt — department hierarchy.
- facilities.txt — facility records.

6. DIRECTORY STRUCTURE

Smart-Campus-Navigation-System/
|
|-- data/
|   |-- buildings.txt
|   |-- connections.txt
|   |-- departments.txt
|   |-- facilities.txt
|
|-- include/
|   |-- CampusGraph.h
|   |-- DepartmentTree.h
|   |-- DepartmentNode.h
|   |-- HashTable.h
|
|-- src/
|   |-- CampusGraph.cpp
|   |-- DepartmentTree.cpp
|   |-- HashTable.cpp
|   |-- main.cpp
|
|-- .gitignore
|-- README.txt
|-- CONTRIBUTIONS.txt
|-- design_document.txt
|-- test_results.txt

7. MAIN MENU FEATURES

1. View all facilities
2. Search facility by ID
3. View campus graph
4. Display graph counts and degrees
5. Find reachable buildings using BFS
6. Add department
7. Search department
8. Display department hierarchy
9. Run preorder traversal
0. Exit

8. NOTES

- Facility IDs use the format F followed by exactly three digits, such as F001.
- The campus graph uses an adjacency list.
- The facility hash table uses separate chaining to handle collisions.
- The department structure uses a general tree.