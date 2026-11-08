// Purpose: Custom separate-chaining hash table for normalized facility IDs.
// Author: Sushil Tiwari
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include <cstddef>
#include <vector>

struct Facility {
std::string id;
std::string name;
std::string building;
std::string category;
};

class HashTable {
private:
struct Entry { std::string key; int recordIndex; };
std::vector<std::vector<Entry>> buckets;
int entryCount;
int collisionCount;
std::string normalize(const std::string& value) const;
std::size_t hashFunction(const std::string& key) const;
public:
explicit HashTable(std::size_t capacity = 11);
bool insert(const std::string& key, int recordIndex);
int find(const std::string& key) const;
bool contains(const std::string& key) const;
std::size_t capacity() const;
int size() const;
int collisions() const;
static bool validID(const std::string& id);
};

class FacilityDatabase {
private:
std::vector<Facility> records;
HashTable idTable;
public:
FacilityDatabase();
bool add(const Facility& facility);
const Facility* findByID(const std::string& id) const;
void displayAll() const;
int size() const;
void printHashStats() const;
};
#endif