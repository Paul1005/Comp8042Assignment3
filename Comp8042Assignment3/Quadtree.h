#include <array>
#include <iostream>
#include "Rectangle.h"
#include "DataSet.h"
#include "GISDataEntry.h"

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

	void insert(GISDataEntry dataEntry, int offset) {
		if (dataEntry.PRIM_LAT_DEC < area.getMaxLat() && 
			dataEntry.PRIM_LAT_DEC > area.getMinLat() && 
			dataEntry.PRIM_LONG_DEC < area.getMaxLong() && 
			dataEntry.PRIM_LONG_DEC > area.getMinLong()) { // if it fits in the designated area
			bool wasAddedToExistingDataSet = false;
			for (int i = 0; i < dataSets.size(); i++) { // see if we can add it to an existing data point
				if (dataSets[i].coordinate.first == dataEntry.PRIM_LAT_DEC && dataSets[i].coordinate.second == dataEntry.PRIM_LONG_DEC) {
					dataSets[i].addOffset(offset);
					wasAddedToExistingDataSet = true;
					break;
				}
			}

			if (!wasAddedToExistingDataSet) { // if we can't
				if (dataSets.size() == K) { // if we already have K data points
					if (!isPartitioned) { // if node hasn't already been partitioned
						// create quadtree children
						float top = area.getMaxLat();
						float bottom = area.getMinLat();
						float left = area.getMinLong();
						float right = area.getMaxLong();

						float horizontalMiddle = (top + bottom) / 2;
						float verticalMiddle = (left + right) / 2;

						Rectangle topLeftRectangle(top, horizontalMiddle, verticalMiddle, left);
						topLeft = new Quadtree(4, topLeftRectangle);

						Rectangle topRightRectangle(top, horizontalMiddle, right, verticalMiddle);
						topRight = new Quadtree(4, topRightRectangle);

						Rectangle bottomLeftRectangle(horizontalMiddle, bottom, verticalMiddle, left);
						bottomLeft = new Quadtree(4, bottomLeftRectangle);

						Rectangle bottomRightRectangle(horizontalMiddle, bottom, right, verticalMiddle);
						bottomRight = new Quadtree(4, bottomRightRectangle);

						for (int i = 0; i < dataSets.size(); i++) { // insert each of our old datasets into the children
							if (dataSets[i].coordinate.first < topLeftRectangle.getMaxLat() && 
								dataSets[i].coordinate.first > topLeftRectangle.getMinLat() && 
								dataSets[i].coordinate.second < topLeftRectangle.getMaxLong() && 
								dataSets[i].coordinate.second > topLeftRectangle.getMinLong()) {
								topLeft->addDataSet(dataSets[i]);
							}
							else if (dataSets[i].coordinate.first < topRightRectangle.getMaxLat() && 
								dataSets[i].coordinate.first > topRightRectangle.getMinLat() && 
								dataSets[i].coordinate.second < topRightRectangle.getMaxLong() && 
								dataSets[i].coordinate.second > topRightRectangle.getMinLong()) {
								topRight->addDataSet(dataSets[i]);
							}
							else if (dataSets[i].coordinate.first < bottomLeftRectangle.getMaxLat() && 
								dataSets[i].coordinate.first > bottomLeftRectangle.getMinLat() && 
								dataSets[i].coordinate.second < bottomLeftRectangle.getMaxLong() && 
								dataSets[i].coordinate.second > bottomLeftRectangle.getMinLong()) {
								bottomLeft->addDataSet(dataSets[i]);
							}
							else if (dataSets[i].coordinate.first < bottomRightRectangle.getMaxLat() && 
								dataSets[i].coordinate.first > bottomRightRectangle.getMinLat() && 
								dataSets[i].coordinate.second < bottomRightRectangle.getMaxLong() && 
								dataSets[i].coordinate.second > bottomRightRectangle.getMinLong()) {
								bottomRight->addDataSet(dataSets[i]);
							}
						}

						// add the children to this node.
						topLeft->parent = topRight->parent = bottomLeft->parent = bottomRight->parent = this;
						isPartitioned = true;
					}

					// insert entry into a child node
					topLeft->insert(dataEntry, offset);
					topRight->insert(dataEntry, offset);
					bottomLeft->insert(dataEntry, offset);
					bottomRight->insert(dataEntry, offset);
				}
				else { // if there's room, create a new dataset
					DataSet newDataSet(dataEntry.PRIM_LAT_DEC, dataEntry.PRIM_LONG_DEC);
					newDataSet.addOffset(offset);
					dataSets.push_back(newDataSet);
				}
			}
		}
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

	Quadtree* getChild(int index)
	{
		if (index == 0) {
			return topLeft;
		}
		else if (index == 1) {
			return topRight;
		}
		else if (index == 2) {
			return bottomLeft;
		}
		else if (index == 3) {
			return bottomRight;
		}
	}

	vector<int> find(float latitude, float longitude) {
		if (isPartitioned) {
			for (int i = 0; i < 4; i++) {
				if (latitude < getChild(i)->area.getMaxLat() && latitude > getChild(i)->area.getMinLat() && longitude < getChild(i)->area.getMaxLong() && longitude > getChild(i)->area.getMinLong()) {
					return getChild(i)->find(latitude, longitude);
				}
			}
		}
		else {
			for (DataSet dataSet : dataSets) {
				if (dataSet.coordinate.first == latitude && dataSet.coordinate.second == longitude) {
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
				if ((latitude + halfHeight > getChild(i)->area.getMinLat() && longitude + halfWidth > getChild(i)->area.getMinLong()) ||
					(latitude + halfHeight > getChild(i)->area.getMinLat() && longitude - halfWidth < getChild(i)->area.getMaxLat()) ||
					(latitude - halfHeight < getChild(i)->area.getMinLat() && longitude + halfWidth > getChild(i)->area.getMinLong()) ||
					(latitude - halfHeight < getChild(i)->area.getMinLat() && longitude - halfWidth > getChild(i)->area.getMaxLat())) {
					vector<int> results = getChild(i)->find(latitude, longitude, halfHeight, halfWidth);
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
				getChild(i)->print();
			}
		}
		else {
			for (DataSet dataSet : dataSets) {
				cout << "latitude: " << dataSet.coordinate.first << endl;;
				cout << "longitude: " << dataSet.coordinate.second << endl;;
				cout << "file offsets:" << endl;
				for (int offset : dataSet.offsets) {
					cout << offset << endl;;
				}
				cout << endl;
			}
		}
	}
private:
	Quadtree* topLeft;
	Quadtree* topRight;
	Quadtree* bottomLeft;
	Quadtree* bottomRight;
};

