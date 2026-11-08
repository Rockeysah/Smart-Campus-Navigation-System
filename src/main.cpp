// Purpose: Coordinates Milestone 1 loading and menu operations.
// Author: Project 2 team (integration file)
#include "CampusGraph.h"
#include "DepartmentTree.h"
#include "HashTable.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

static std::string trimText(const std::string& s) {
const std::string ws = " \t\r\n\v\f";
std::size_t a = s.find_first_not_of(ws);
if (a == std::string::npos) return "";
return s.substr(a, s.find_last_not_of(ws) - a + 1);
}
static std::string lowerText(std::string s) {
for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
return s;
}
static std::string upperText(std::string s) {
for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
return s;
}
static bool blankOrComment(const std::string& line) {
std::string t = trimText(line);
return t.empty() || t[0] == '#';
}
static std::vector<std::string> splitFields(const std::string& line) {
std::vector<std::string> fields;
std::string field;
std::stringstream stream(line);
while (std::getline(stream, field, '|')) fields.push_back(trimText(field));
if (!line.empty() && line.back() == '|') fields.push_back("");
return fields;
}
static bool knownBuilding(const std::string& raw, const std::string& dir, std::string& spelling) {
std::ifstream in(dir + "/buildings.txt");
std::string line, target = lowerText(trimText(raw));
while (std::getline(in, line)) {
if (blankOrComment(line)) continue;
std::string candidate = trimText(line);
if (lowerText(candidate) == target) { spelling = candidate; return true; }
}
return false;
}
static void loadFacilities(const std::string& dir, FacilityDatabase& db) {
const std::string file = dir + "/facilities.txt";
std::ifstream in(file);
if (!in) {
std::cout << "ERROR: cannot open " << file << " (treated as empty)\n";
std::cout << "facilities.txt: 0 accepted, 0 rejected; "; db.printHashStats();
return;
}
std::string line; int lineNo = 0, accepted = 0, rejected = 0;
while (std::getline(in, line)) {
++lineNo;
if (blankOrComment(line)) continue;
std::vector<std::string> f = splitFields(line);
std::string reason;
if (f.size() != 4) reason = "wrong number of fields (expected 4)";
else if (f[0].empty() || f[1].empty() || f[2].empty() || f[3].empty()) reason = "empty required field";
else if (!HashTable::validID(upperText(f[0]))) reason = "invalid ID; expected F followed by exactly three digits";
else {
std::string building;
if (!knownBuilding(f[2], dir, building)) reason = "unknown building '" + f[2] + "'";
else {
Facility item{upperText(f[0]), f[1], building, f[3]};
if (db.add(item)) ++accepted;
else reason = "duplicate facility ID";
}
}
if (!reason.empty()) {
++rejected;
std::cout << "WARNING: facilities.txt line " << lineNo << ": rejected - " << reason << '\n';
}
}
std::cout << "facilities.txt: " << accepted << " accepted, " << rejected << " rejected; ";
db.printHashStats();
}
static bool readName(const std::string& prompt, std::string& value) {
std::cout << prompt;
if (!std::getline(std::cin, value)) return false;
value = trimText(value);
if (value.empty()) { std::cout << "Error: name cannot be blank.\n"; return false; }
return true;
}
static bool readChoice(int& choice) {
std::string line;
if (!std::getline(std::cin, line)) return false;
std::stringstream ss(line); char extra;
if (!(ss >> choice) || (ss >> extra)) return false;
return true;
}
int main(int argc, char* argv[]) {
const std::string dir = argc > 1 ? argv[1] : "data";
CampusGraph graph;
DepartmentTree departments;
FacilityDatabase facilities;
std::cout << "Loading campus data from " << dir << "/\n";
graph.loadBuildings(dir + "/buildings.txt");
graph.loadConnections(dir + "/connections.txt");
loadFacilities(dir, facilities);
departments.loadFromFile(dir + "/departments.txt", "departments.txt");

int choice = -1;
while (choice != 0) {
std::cout << "\n===== SMART CAMPUS SYSTEM (MILESTONE 1) =====\n"
<< "1. View all facilities\n2. Search facility by ID\n"
<< "3. View campus graph\n4. Display graph counts and degrees\n"
<< "5. Find reachable buildings (BFS)\n6. Add department\n"
<< "7. Search department\n8. Display department hierarchy\n"
<< "9. Run preorder traversal\n0. Exit\nEnter choice: ";
if (!readChoice(choice)) { std::cout << "Invalid menu input. Enter a number from 0 to 9.\n"; choice = -1; continue; }
if (choice == 1) facilities.displayAll();
else if (choice == 2) {
std::string id; std::cout << "Enter facility ID: "; std::getline(std::cin, id); id = upperText(trimText(id));
if (!HashTable::validID(id)) std::cout << "Invalid format: ID must be F followed by exactly three digits.\n";
else { const Facility* f = facilities.findByID(id); if (!f) std::cout << "Not found: " << id << '\n';
else std::cout << "Found: " << f->id << " | " << f->name << " | " << f->building << " | " << f->category << '\n'; }
}
else if (choice == 3) graph.displayGraph();
else if (choice == 4) graph.displayGraphStatistics();
else if (choice == 5) { std::string name; if (readName("Enter starting building: ", name)) graph.findReachableBuildings(name); }
else if (choice == 6) {
std::string name, parent, error;
if (readName("Enter department name: ", name)) {
if (departments.isEmpty()) {
if (departments.addDepartment(name, "", error)) std::cout << "Root department added.\n";
else std::cout << "Error: " << error << '\n';
} else if (readName("Enter existing parent department: ", parent)) {
if (departments.addDepartment(name, parent, error)) std::cout << "Department added.\n";
else std::cout << "Error: " << error << '\n';
}
}
}
else if (choice == 7) { std::string name; if (readName("Enter department name: ", name)) departments.searchDepartment(name); }
else if (choice == 8) departments.displayHierarchy();
else if (choice == 9) departments.displayPreorder();
else if (choice == 0) std::cout << "Goodbye.\n";
else std::cout << "Invalid choice. Enter a number from 0 to 9.\n";
}
return 0;
}