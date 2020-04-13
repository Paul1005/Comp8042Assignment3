#include <iostream>
#include <iomanip>
#include <math.h>
#include <random>
#include <fstream>

using namespace std;
// based on lab 5 and 6
template <typename K>
#pragma once
class Hashtable {
	enum BucketStatus { EMPTY, OCCUPIED, DELETED };

	/// The total number of collisions since the creation of the hashfunction.
	/// This includes the collisions encountered during insertions, searches,
	/// and deletions
	int numCollisions;

	/// Capacity of the hashtable
	int c;

	/// All the buckets in the hashtable
	K* buckets;

	vector<int>* offsets;

	/// Status of all the buckets
	BucketStatus* status;

	/// Adjusts the capacity of the hashtable and rehashes
	void expandAndRehash() {
		c *= 2;

		K* newBuckets = new K[c];
		BucketStatus* newStatus = new BucketStatus[c];
		for (int i = 0; i < c; i++)
		{
			newStatus[i] = EMPTY;
		}
		vector<int>* newOffsets = new vector<int>[c];

		K* tempBuckets = new K[c];
		BucketStatus* tempStatus = new BucketStatus[c];
		for (int i = 0; i < c; i++)
		{
			tempStatus[i] = EMPTY;
		}
		vector<int>* tempOffsets = new vector<int>[c];

		tempBuckets = buckets;
		tempStatus = status;
		tempOffsets = offsets;

		buckets = newBuckets;
		status = newStatus;
		offsets = newOffsets;

		for (int i = 0; i < c; i++) {
			if (tempStatus[i] == OCCUPIED) {
				for (int j = 0; j < offsets[i].size(); j++) {
					insert(tempBuckets[i], offsets[i][j]);
				}
			}
		}
	}

	int size;
	int numOfRehashes;
public:
	Hashtable() : numCollisions(0) {
		c = 1024;
		buckets = new K[c];
		status = new BucketStatus[c];
		for (int i = 0; i < c; i++)
		{
			status[i] = EMPTY;
		}
		offsets = new vector<int>[c];

		size = 0;
		numOfRehashes = 0;
	}

	~Hashtable() {
	}

	unsigned int hash(unsigned int key) {
		return (pow(key, 2) + key) / 2;
	}

	unsigned int hashFunction(const string& s, int size) const {
		unsigned int hash = 0;
		for (int i = 0; i < s.size(); i++) {
			hash += s[i];
		}
		hash = hash % size;
		return hash;
	}

	/// Tries to insert the given key into the hashtable.
	/// Returns true if the element was inserted and false if not.
	/// The insertion will fail if the element already exists in the input.
	bool insert(const K& key, int offset) {
		float loadFactor = (float)size / (float)c;
		if (loadFactor >= 0.7) {
			expandAndRehash();
			numOfRehashes++;
		}
		unsigned int h = hashFunction(key, c);
		unsigned int i = 0;
		unsigned int hi = h;
		while (status[hi] == OCCUPIED) {
			if (buckets[hi] == key) {
				offsets[hi].push_back(offset);
				return true;
			}
			numCollisions++;
			++i;
			hi = (h + hash(i)) % c;
		}
		status[hi] = OCCUPIED;
		buckets[hi] = key;
		offsets[hi].push_back(offset);
		size++;
		return true; // Key inserted successfully
	}

	vector<int> search(const K& key) {
		unsigned int h = hashFunction(key, c);
		unsigned int i = 0;
		unsigned int hi = h;
		while (status[hi] != EMPTY) {
			if (status[hi] == OCCUPIED && buckets[hi] == key) {
				// Key found
				return offsets[hi];
			}
			numCollisions++;
			++i;
			hi = (h + hash(i)) % c;
		}
		// Key not found. Hit an empty bucket.
		vector<int> v;
		return v;
	}

	bool erase(const K& key) {
		unsigned int h = hashFunction(key, c);
		unsigned int i = 0;
		unsigned int hi = h;
		while (status[hi] != EMPTY) {
			if (status[hi] == OCCUPIED && buckets[hi] == key) {
				// Key found, delete it.
				status[hi] = DELETED;
				size--;
				return true;
			}
			numCollisions++;
			++i;
			hi = (h + hash(i)) % c;
		}
		// Key not found an hit an empty bucket.
		return false;
	}

	int getNumCollisions() const {
		return numCollisions;
	}

	int getNumOfRehashes() const {
		return numOfRehashes;
	}

	void print(ofstream* logFile) {
		*logFile << endl;
		*logFile << "Format of display is" << endl;
		*logFile << "Slot number: data record" << endl;
		*logFile << "Current table size is " << c << endl;
		*logFile << "Number of elements in table is " << size << endl;
		*logFile << endl;

		for (int i = 0; i < c; i++) {
			if (status[i] == OCCUPIED) {
				*logFile << "\t  " << i << ":  [" << buckets[i] << ", [";
				for (int j = 0; j < offsets[i].size(); j++) {
					*logFile << offsets[i][j] << ", ";
				}
				*logFile << "]]" << endl;
			}
		}
	}
};