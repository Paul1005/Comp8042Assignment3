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
	vector<unsigned int> buckets;

	/// Status of all the buckets
	vector<BucketStatus> status;

	/// Adjusts the capacity of the hashtable and rehashes
	void expandAndRehash() {
		c *= 2;

		vector<unsigned int> newBuckets;
		vector<BucketStatus> newStatus;
		newBuckets.resize(c);
		newStatus.resize(c);

		vector<unsigned int> tempBuckets;
		vector<BucketStatus> tempStatus;

		tempBuckets = buckets;
		tempStatus = status;

		buckets = newBuckets;
		status = newStatus;

		for (int i = 0; i < tempBuckets.size(); i++) {
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
		buckets.resize(c);
		status.resize(c);
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
	bool insert(const K& key) {
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