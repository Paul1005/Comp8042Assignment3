#include <array>
#include <iostream>
#include "Rectangle.h"
#include "DataSet.h"

using namespace std;
// based on https://codereview.stackexchange.com/questions/154226/simple-quadtree-template-implementation 
#pragma once
class Quadtree
{
public:
	int K;
	bool isPartitioned;

	Rectangle area; 
	vector<DataSet> dataSets;

	Quadtree* parent;

	Quadtree(int currentK, Rectangle currentArea) : area(currentArea) // constructor
	{
		K = currentK;
		isPartitioned = false;
	}

	Quadtree() : area(0, 0, 0, 0) {

	}

	void addDataSet(DataSet dataSet)
	{
		if (dataSets.size() < K) {
			dataSets.push_back(dataSet);
		}
		else {
			//cout << "is full";
		}

	}

	void setChild(int index, Quadtree* node)
	{
		if (index == 0) {
			topLeft = node;
		}
		else if (index == 1) {
			topRight = node;
		}
		else if (index == 2) {
			bottomLeft = node;
		}
		else if (index == 3) {
			bottomRight = node;
		}
	}

	Quadtree getChild(int index)
	{
		if (index == 0) {
			return *topLeft;
		}
		else if (index == 1) {
			return *topRight;
		}
		else if (index == 2) {
			return *bottomLeft;
		}
		else if (index == 3) {
			return *bottomRight;
		}
	}

	vector<int> find(float latitude, float longitude) {
		if (isPartitioned) {
			for (int i = 0; i < 4; i++) {
				if (latitude < getChild(i).area.maxLat && latitude > getChild(i).area.minLat && longitude < getChild(i).area.maxLong && longitude > getChild(i).area.minLong) {
					return getChild(i).find(latitude, longitude);
				}
			}
		}
		else {
			for (DataSet dataSet : dataSets) {
				if (dataSet.coordinate.first == latitude &&  dataSet.coordinate.second == longitude) {
					return dataSet.offsets;
				}
			}
		}
		vector<int> v;
		return v;
	}

	vector<int> find(float latitude, float longitude, float halfHeight, float halfWidth) {
		vector<int> offsets;
		if (isPartitioned) {
			for (int i = 0; i < 4; i++) {
				if ((latitude + halfHeight > getChild(i).area.minLat && longitude + halfWidth > getChild(i).area.minLong) || 
					(latitude + halfHeight > getChild(i).area.minLat && longitude - halfWidth < getChild(i).area.maxLat) ||
					(latitude - halfHeight < getChild(i).area.maxLat && longitude + halfWidth > getChild(i).area.minLong) ||
					(latitude - halfHeight < getChild(i).area.maxLat && longitude - halfWidth > getChild(i).area.maxLat)) {
					vector<int> results = getChild(i).find(latitude, longitude, halfHeight, halfWidth);
					offsets.insert(offsets.end(), results.begin(), results.end());
				}
			}
		}
		else {
			for (DataSet dataSet : dataSets) {
				if (dataSet.coordinate.first < latitude + halfHeight && dataSet.coordinate.first > latitude - halfHeight && dataSet.coordinate.second < longitude + halfWidth && dataSet.coordinate.second > longitude - halfWidth) {
					offsets.insert(offsets.end(), dataSet.offsets.begin(), dataSet.offsets.end());
				}
			}
		}
		return offsets;
	}

	void print() {
		if (isPartitioned) {
			for (int i = 0; i < 4; i++) {
				getChild(i).print();
			}
		}
		else {
			for (DataSet dataSet : dataSets) {
				cout << "latitude: " << dataSet.coordinate.first;
				cout << "longitude: " << dataSet.coordinate.second;
				cout << "file offsets:";
				for (int offset : dataSet.offsets) {
					cout << offset;
				}
			}
		}
	}
private:
	Quadtree* topLeft;
	Quadtree* topRight;
	Quadtree* bottomLeft;
	Quadtree* bottomRight;
};

