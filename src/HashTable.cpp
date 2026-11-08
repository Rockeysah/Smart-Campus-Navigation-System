// Purpose: Implements a custom string hash table using separate chaining.
// Author: Sushil Tiwari
#include "HashTable.h"
#include <cctype>

static std::string upperText(const std::string& s) {
std::string result = s;
for (char& c : result) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
return result;
}
HashTable::HashTable(std::size_t capacity) : buckets(capacity ? capacity : 11), entryCount(0), collisionCount(0) {}
std::string HashTable::normalize(const std::string& value) const { return upperText(value); }
std::size_t HashTable::hashFunction(const std::string& key) const {
unsigned long long hash = 0;
for (unsigned char c : key) hash = (hash * 31 + c) % buckets.size();
return static_cast<std::size_t>(hash);
}
bool HashTable::insert(const std::string& key, int recordIndex) {
std::string normalized = normalize(key);
std::size_t index = hashFunction(normalized);
for (const Entry& entry : buckets[index]) if (entry.key == normalized) return false;
if (!buckets[index].empty()) ++collisionCount;
buckets[index].push_back({normalized, recordIndex});
++entryCount;
return true;
}
int HashTable::find(const std::string& key) const {
std::string normalized = normalize(key);
std::size_t index = hashFunction(normalized);
for (const Entry& entry : buckets[index]) if (entry.key == normalized) return entry.recordIndex;
return -1;
}
bool HashTable::contains(const std::string& key) const { return find(key) != -1; }
std::size_t HashTable::capacity() const { return buckets.size(); }
int HashTable::size() const { return entryCount; }
int HashTable::collisions() const { return collisionCount; }
bool HashTable::validID(const std::string& id) {
if (id.size() != 4 || std::toupper(static_cast<unsigned char>(id[0])) != 'F') return false;
return std::isdigit(static_cast<unsigned char>(id[1])) &&
std::isdigit(static_cast<unsigned char>(id[2])) &&
std::isdigit(static_cast<unsigned char>(id[3]));
}

#include <iostream>

FacilityDatabase::FacilityDatabase() : idTable(11) {}

bool FacilityDatabase::add(const Facility& facility) {
if (!HashTable::validID(facility.id)) return false;
int index = static_cast<int>(records.size());
if (!idTable.insert(facility.id, index)) return false;
records.push_back(facility);
return true;
}

const Facility* FacilityDatabase::findByID(const std::string& id) const {
int index = idTable.find(id);
if (index < 0 || index >= static_cast<int>(records.size())) return nullptr;
return &records[static_cast<std::size_t>(index)];
}

void FacilityDatabase::displayAll() const {
if (records.empty()) {
std::cout << "No facilities are available.\n";
return;
}
for (const Facility& facility : records) {
std::cout << facility.id << " | " << facility.name << " | "
<< facility.building << " | " << facility.category << '\n';
}
std::cout << "Total facilities: " << records.size() << '\n';
}

int FacilityDatabase::size() const {
return static_cast<int>(records.size());
}

void FacilityDatabase::printHashStats() const {
std::cout << "Hash capacity: " << idTable.capacity()
<< ", entries: " << idTable.size()
<< ", collisions: " << idTable.collisions() << '\n';
}