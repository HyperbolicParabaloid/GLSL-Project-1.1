#ifndef HASHTABLE_CLASS_H
#define HASHTABLE_CLASS_H

#include "Object.h"
class HashTable
{
private:
	int tableSize;
	glm::vec3 *hashTable;
	int nextPrime(int _num);
	bool isPrime(int _num);
	int hashFunc(glm::vec3 _v);
public:
	HashTable(int _size);
	~HashTable();
	int addItem(glm::vec3 _v);
	glm::vec3 getItem(int _index);
	int isInTable(glm::vec3 _v);
};
#endif
