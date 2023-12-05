#include "HashTable.h"

int HashTable::hashFunc(glm::vec3 _v)
{
    int index = 0;
    glm::vec3 v = (_v + 2.f) * 1.f;
    //std::cout << "v = (" << v.x << ", " << v.y << ", " << v.z << ")\n";
    index += abs(int(pow((v.x), 13))) % (3637 * tableSize);
    index += abs(int(pow((v.z), 19))) % (2221 * tableSize);
    index += abs(int(pow((v.y), 17))) % (4397 * tableSize);
    index = abs(index);
    index %= tableSize;
    //std::cout << "Current index: index = " << index << "\n";
	return index;
}


// Function that returns true if n 
// is prime else returns false 
bool HashTable::isPrime(int _num)
{
    // Corner cases 
    if (_num <= 1)  return false;
    if (_num <= 3)  return true;

    // This is checked so that we can skip  
    // middle five numbers in below loop 
    if (_num % 2 == 0 || _num % 3 == 0) return false;

    for (int i = 5; i * i <= _num; i = i + 6)
        if (_num % i == 0 || _num % (i + 2) == 0)
            return false;

    return true;
}


// Function to return the smallest
// prime number greater than N
int HashTable::nextPrime(int _num)
{

    // Base case
    if (_num <= 1)
        return 2;

    int prime = _num;
    bool found = false;

    // Loop continuously until isPrime returns
    // true for a number greater than n
    while (!found) {
        prime++;

        if (isPrime(prime))
            found = true;
    }

    return prime;
}


HashTable::HashTable(int _size) {
	tableSize = nextPrime(_size * 2);
    glm::vec3* newHashTable = new glm::vec3[tableSize];	delete[] hashTable;
    hashTable = newHashTable;
    std::fill(hashTable, hashTable + tableSize, glm::vec3(FLT_MAX));
}


HashTable::~HashTable() {
    delete[] hashTable;
}


int HashTable::addItem(glm::vec3 _v) {
    int index = hashFunc(_v);
    if (isInTable(_v) != -1)
        return -1;
    else
        hashTable[index] = _v;
    return index;
}


glm::vec3 HashTable::getItem(int _index) {
    return hashTable[_index];
}


int HashTable::isInTable(glm::vec3 _v) {
    int index = hashFunc(_v);
    if (hashTable[index] == glm::vec3(FLT_MAX))
        return -1;
    else return index;
}