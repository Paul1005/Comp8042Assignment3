#include <iostream>
#include <gtest/gtest.h>
#include <iomanip>
#include <math.h>
#include <random>

using namespace std;

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

	int* offsets;

	/// Status of all the buckets
	BucketStatus* status;

	/// Adjusts the capacity of the hashtable and rehashes
	void expandAndRehash() {
		c *= 2;

		K* newBuckets = new K[c];
		BucketStatus* newStatus = new BucketStatus[c];
		int* newOffsets = new int[c];

		K* tempBuckets = new K[c];
		BucketStatus* tempStatus = new BucketStatus[c];
		int* tempOffsets = new int[c];

		tempBuckets = buckets;
		tempStatus = status;
		tempOffsets = offsets;

		buckets = newBuckets;
		status = newStatus;
		offsets = newOffsets;

		for (int i = 0; i < c; i++) {
			if (tempStatus[i] == OCCUPIED) {
				insert(tempBuckets[i]);
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
		offsets = new int[c];

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
			if (buckets[hi] == key)
				return false; // Key already exists
			numCollisions++;
			++i;
			hi = (h + hash(i)) % c;
		}
		status[hi] = OCCUPIED;
		buckets[hi] = key;
		offsets[hi] = offset;
		size++;
		return true; // Key inserted successfully
	}

	bool search(const K& key) {
		unsigned int h = hashFunction(key, c);
		unsigned int i = 0;
		unsigned int hi = h;
		while (status[hi] != EMPTY) {
			if (status[hi] == OCCUPIED && buckets[hi] == key) {
				// Key found
				return true;
			}
			numCollisions++;
			++i;
			hi = (h + hash(i)) % c;
		}
		// Key not found. Hit an empty bucket.
		return false;
	}

	bool erase(const K& key) {
		unsigned int h = hashFunction(key, c);
		unsigned int i = 0;
		unsigned int hi = h;
		while (status[hi] != EMPTY) {
			if (buckets[hi] == OCCUPIED && buckets[hi] == key) {
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
};