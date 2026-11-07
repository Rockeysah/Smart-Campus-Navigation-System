// DepartmentNode.h
// Purpose: One node of the general department tree (any number of children).
// Author: Erin Sen
#ifndef DEPARTMENTNODE_H
#define DEPARTMENTNODE_H
 
#include <string>
#include <vector>
 
// One department. Children are kept in insertion order.
struct DepartmentNode {
    std::string name;                        // spelling from first accepted record
    DepartmentNode* parent;                  // nullptr for the root
    std::vector<DepartmentNode*> children;   // any number of children
    int depth;                               // edges from the root (root = 0)
 
    DepartmentNode(const std::string& n, DepartmentNode* p, int d)
        : name(n), parent(p), depth(d) {}
};
 
#endif
