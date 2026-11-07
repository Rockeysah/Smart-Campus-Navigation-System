// DepartmentTree.cpp
// Purpose: Implementation of the general department tree (Milestone 1).
// Author: Member C
#include "DepartmentTree.h"
 
#include <fstream>
#include <iostream>
 
namespace {
 
// Removes leading/trailing spaces, tabs, \r and \n.
std::string trim(const std::string& s) {
    const char* ws = " \t\r\n\v\f";
    std::size_t b = s.find_first_not_of(ws);
    if (b == std::string::npos) return "";
    std::size_t e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}
 
// ASCII-lowercased copy of s.
std::string toLower(const std::string& s) {
    std::string r = s;
    for (std::size_t i = 0; i < r.size(); ++i)
        if (r[i] >= 'A' && r[i] <= 'Z') r[i] = static_cast<char>(r[i] - 'A' + 'a');
    return r;
}
 
// Case-insensitive equality; internal spaces still matter.
bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toLower(a) == toLower(b);
}
 
// Splits line on delimiter, keeping empty fields.
std::vector<std::string> split(const std::string& line, char delim) {
    std::vector<std::string> out;
    std::string cur;
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (line[i] == delim) { out.push_back(cur); cur.clear(); }
        else cur += line[i];
    }
    out.push_back(cur);
    return out;
}
 
} // namespace
 
// Creates an empty tree.
DepartmentTree::DepartmentTree() : root_(nullptr) {}
 
// Releases every node (all nodes are tracked in nodes_).
DepartmentTree::~DepartmentTree() {
    for (std::size_t i = 0; i < nodes_.size(); ++i) delete nodes_[i];
    nodes_.clear();
    root_ = nullptr;
}
 
// Finds a node by name ignoring ASCII case (name is trimmed first).
DepartmentNode* DepartmentTree::findNode(const std::string& name) const {
    std::string target = trim(name);
    for (std::size_t i = 0; i < nodes_.size(); ++i)
        if (equalsIgnoreCase(nodes_[i]->name, target)) return nodes_[i];
    return nullptr;
}
 
// Adds a department; the first one becomes the root (no parent needed).
bool DepartmentTree::addDepartment(const std::string& rawName,
                                   const std::string& rawParent,
                                   std::string& errorMsg) {
    std::string name = trim(rawName);
    if (name.empty()) { errorMsg = "department name is empty"; return false; }
    if (findNode(name)) {
        errorMsg = "duplicate department name '" + name + "'";
        return false;
    }
    if (root_ == nullptr) {                       // empty tree: becomes the root
        root_ = new DepartmentNode(name, nullptr, 0);
        nodes_.push_back(root_);
        return true;
    }
    std::string parentName = trim(rawParent);
    if (parentName.empty()) { errorMsg = "parent name is empty"; return false; }
    DepartmentNode* parent = findNode(parentName);
    if (!parent) {
        errorMsg = "unknown parent '" + parentName + "'";
        return false;
    }
    DepartmentNode* node = new DepartmentNode(name, parent, parent->depth + 1);
    parent->children.push_back(node);             // siblings keep insertion order
    nodes_.push_back(node);
    return true;
}
 
// Reads departments.txt-format data and validates each record (Section 5.5).
void DepartmentTree::loadFromFile(const std::string& path,
                                  const std::string& displayName) {
    int accepted = 0, rejected = 0;
    std::ifstream in(path.c_str());
    if (!in.is_open()) {
        std::cout << "ERROR: cannot open " << displayName
                  << " (treated as empty)\n";
        std::cout << displayName << ": 0 accepted, 0 rejected\n";
        return;
    }
    std::string line;
    int lineNo = 0;
    while (std::getline(in, line)) {
        ++lineNo;
        std::string t = trim(line);                       // also removes \r
        if (t.empty() || t[0] == '#') continue;           // blank / comment line
 
        std::vector<std::string> f = split(t, '|');
        std::string reason;
        bool ok = false;
        if (f.size() != 2) {
            reason = "wrong number of fields (expected DepartmentName|ParentName)";
        } else {
            std::string name = trim(f[0]);
            std::string parent = trim(f[1]);
            if (name.empty() || parent.empty()) {
                reason = "empty required field";
            } else if (root_ == nullptr) {
                // First valid record must be the root and use NONE as parent.
                if (!equalsIgnoreCase(parent, "NONE"))
                    reason = "first department (root) must use NONE as parent";
                else
                    ok = addDepartment(name, "", reason);
            } else if (equalsIgnoreCase(parent, "NONE") && !findNode(parent)) {
                reason = "only the first department may use NONE as parent";
            } else {
                ok = addDepartment(name, parent, reason);  // duplicate/unknown parent
            }
        }
        if (ok) { ++accepted; continue; }
        ++rejected;
        std::cout << "WARNING: " << displayName << " line " << lineNo
                  << ": rejected - " << reason << "\n";
    }
    std::cout << displayName << ": " << accepted << " accepted, "
              << rejected << " rejected\n";
}
 
// Collects nodes in preorder: node first, then children in insertion order.
void DepartmentTree::preorderCollect(const DepartmentNode* n,
                                     std::vector<const DepartmentNode*>& out) const {
    if (!n) return;
    out.push_back(n);
    for (std::size_t i = 0; i < n->children.size(); ++i)
        preorderCollect(n->children[i], out);
}
 
// Prints name, parent, direct-child count and depth of a department.
void DepartmentTree::searchDepartment(const std::string& rawName) const {
    if (isEmpty()) { std::cout << "No departments are loaded.\n"; return; }
    std::string name = trim(rawName);
    if (name.empty()) { std::cout << "Error: department name cannot be blank.\n"; return; }
    const DepartmentNode* n = findNode(name);
    if (!n) { std::cout << "Department '" << name << "' not found.\n"; return; }
    std::cout << "Department: " << n->name << "\n"
              << "Parent: " << (n->parent ? n->parent->name : std::string("None")) << "\n"
              << "Direct children: " << n->children.size() << "\n"
              << "Depth: " << n->depth << "\n";
}
 
// Prints the hierarchy: one department per line, 4 spaces per depth level.
void DepartmentTree::displayHierarchy() const {
    if (isEmpty()) { std::cout << "No departments are loaded.\n"; return; }
    std::vector<const DepartmentNode*> order;
    preorderCollect(root_, order);
    for (std::size_t i = 0; i < order.size(); ++i)
        std::cout << std::string(4 * order[i]->depth, ' ') << order[i]->name << "\n";
}
 
// Prints the preorder sequence on one line, without indentation.
void DepartmentTree::displayPreorder() const {
    if (isEmpty()) { std::cout << "No departments are loaded.\n"; return; }
    std::vector<const DepartmentNode*> order;
    preorderCollect(root_, order);
    std::cout << "Preorder: ";
    for (std::size_t i = 0; i < order.size(); ++i)
        std::cout << (i ? ", " : "") << order[i]->name;
    std::cout << "\n";
}
 
