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
	typedef array<Quadtree, 4> Children; // class scope typedef

	Rectangle area; 
	vector<DataSet> dataSets;

	Quadtree* parent;

	Quadtree(int currentK, Rectangle currentArea) : area(currentArea) // constructor
	{
		K = currentK;
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

	void setChildren(Children nodes)
	{
		children = &nodes;
	}

	void setChild(int _index, Quadtree node)
	{
		children->at(_index) = node;
	}

	Quadtree getChild(int _index)
	{
		return children->at(_index);
	}

	Children* getChildren() {
		return children;
	}

	vector<int> find(float latitude, float longitude) {
		if (children->size() == 4) {
			for (int i = 0; i < children->max_size(); i++) {
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
		if (children->size() == 4) {
			for (int i = 0; i < children->max_size(); i++) {
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
		if (children->size() == 4) {
			for (int i = 0; i < children->max_size(); i++) {
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
	Children* children;
};

