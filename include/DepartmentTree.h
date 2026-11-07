// DepartmentTree.h
// Purpose: General tree (any number of children per node) for the academic
//          department hierarchy: load, add, search, display, preorder.
// Author: Erin Sen
#ifndef DEPARTMENTTREE_H
#define DEPARTMENTTREE_H
 
#include <string>
#include <vector>
 
#include "DepartmentNode.h"
 
class DepartmentTree {
public:
    DepartmentTree();                                    // creates an empty tree
    ~DepartmentTree();                                   // frees every node
    DepartmentTree(const DepartmentTree&) = delete;      // owns raw pointers
    DepartmentTree& operator=(const DepartmentTree&) = delete;
 
    // Loads departments.txt-format data from path. Prints warnings (file name,
    // line number, reason) and a load summary line. A missing file prints an
    // error naming the file and is treated as empty.
    void loadFromFile(const std::string& path, const std::string& displayName);
 
    // Adds a department (menu option 6). When the tree is empty the department
    // becomes the root and parentName is ignored. Returns false and sets
    // errorMsg (blank name, duplicate, unknown parent) leaving the tree unchanged.
    bool addDepartment(const std::string& name, const std::string& parentName,
                       std::string& errorMsg);
 
    // Menu option 7: prints name, parent (None for root), child count, depth,
    // or a not-found / blank-name / empty-tree message.
    void searchDepartment(const std::string& name) const;
 
    // Menu option 8: preorder hierarchy, one per line, 4 spaces per depth.
    void displayHierarchy() const;
 
    // Menu option 9: preorder sequence without indentation.
    void displayPreorder() const;
 
    bool isEmpty() const { return root_ == nullptr; }    // true if no departments
 
private:
    DepartmentNode* root_;                    // top department (nullptr if empty)
    std::vector<DepartmentNode*> nodes_;      // all nodes in insertion order
 
    DepartmentNode* findNode(const std::string& name) const;
    void preorderCollect(const DepartmentNode* n,
                         std::vector<const DepartmentNode*>& out) const;
};
 
#endif
 
