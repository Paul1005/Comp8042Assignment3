// Comp8042Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>        
#include "Hashtable.h"
#include "GISDataEntry.h"
#include "Quadtree.h"
#include "DataSet.h"

using namespace std;

float arcSecond = 1 / 3600;

void insertIntoQuadtree(GISDataEntry dataEntry, Quadtree* quadtree, int offset) {
	if (dataEntry.PRIM_LAT_DEC < (*quadtree).area.maxLat && dataEntry.PRIM_LAT_DEC >(*quadtree).area.minLat&& dataEntry.PRIM_LONG_DEC <(*quadtree).area.maxLong && dataEntry.PRIM_LONG_DEC >(*quadtree).area.minLong) { // if it fits in the designated area
		bool wasAddedToExistingDataSet = false;
		for (int i = 0; i < (*quadtree).dataSets.size(); i++) { // see if we can add it to an existing data point
			if ((*quadtree).dataSets[i].coordinate.first == dataEntry.PRIM_LAT_DEC && (*quadtree).dataSets[i].coordinate.second == dataEntry.PRIM_LONG_DEC) {
				(*quadtree).dataSets[i].addOffset(offset);
				wasAddedToExistingDataSet = true;
				break;
			}
		}

		if (!wasAddedToExistingDataSet) { // if we can't
			if ((*quadtree).dataSets.size() == (*quadtree).K) { // if we already have K data points
				if (!(*quadtree).isPartitioned) { // if node hasn't already been partitioned
					// create quadtree children
					float top = (*quadtree).area.maxLat;
					float bottom = (*quadtree).area.minLat;
					float left = (*quadtree).area.minLong;
					float right = (*quadtree).area.maxLong;

					Rectangle topLeftRectangle(top, top / 2, right / 2, left);
					Quadtree topLeft(4, topLeftRectangle);

					Rectangle topRightRectangle(top, top / 2, right, right / 2);
					Quadtree topRight(4, topRightRectangle);

					Rectangle bottomLeftRectangle(top / 2, bottom, right / 2, left);
					Quadtree bottomLeft(4, bottomLeftRectangle);

					Rectangle bottomRightRectangle(top / 2, bottom, right, right / 2);
					Quadtree bottomRight(4, bottomRightRectangle);

					for (int i = 0; i < (*quadtree).dataSets.size(); i++) { // insert each of our old datasets into the children
						if ((*quadtree).dataSets[i].coordinate.first < topLeft.area.maxLat && (*quadtree).dataSets[i].coordinate.first > topLeft.area.minLat&& (*quadtree).dataSets[i].coordinate.second <topLeft.area.maxLong && (*quadtree).dataSets[i].coordinate.second > topLeft.area.minLong) {
							topLeft.addDataSet((*quadtree).dataSets[i]);
						}
						else if ((*quadtree).dataSets[i].coordinate.first < topRight.area.maxLat && (*quadtree).dataSets[i].coordinate.first > topRight.area.minLat&& (*quadtree).dataSets[i].coordinate.second <topRight.area.maxLong && (*quadtree).dataSets[i].coordinate.second > topRight.area.minLong) {
							topRight.addDataSet((*quadtree).dataSets[i]);;
						}
						else if ((*quadtree).dataSets[i].coordinate.first < bottomLeft.area.maxLat && (*quadtree).dataSets[i].coordinate.first > bottomLeft.area.minLat&& (*quadtree).dataSets[i].coordinate.second <bottomLeft.area.maxLong && (*quadtree).dataSets[i].coordinate.second > bottomLeft.area.minLong) {
							bottomLeft.addDataSet((*quadtree).dataSets[i]);;
						}
						else if ((*quadtree).dataSets[i].coordinate.first < bottomRight.area.maxLat && (*quadtree).dataSets[i].coordinate.first > bottomRight.area.minLat&& (*quadtree).dataSets[i].coordinate.second <bottomRight.area.maxLong && (*quadtree).dataSets[i].coordinate.second > bottomRight.area.minLong) {
							bottomRight.addDataSet((*quadtree).dataSets[i]);;
						}
					}

					// add the children to this node.
					topLeft.parent = quadtree;
					quadtree->setChild(0, &topLeft);
					topRight.parent = quadtree;
					quadtree->setChild(1, &topRight);
					bottomLeft.parent = quadtree;
					quadtree->setChild(2, &bottomLeft);
					bottomRight.parent = quadtree;
					quadtree->setChild(3, &bottomRight);
					quadtree->isPartitioned = true;
				}

				// insert entry into a child node
				for (int i = 0; i < (*quadtree).dataSets.size(); i++) {
					Quadtree topLeft = (*quadtree).getChild(0);
					insertIntoQuadtree(dataEntry, &topLeft, offset);
					Quadtree topRight = (*quadtree).getChild(1);
					insertIntoQuadtree(dataEntry, &topRight, offset);
					Quadtree bottomLeft = (*quadtree).getChild(2);
					insertIntoQuadtree(dataEntry, &bottomLeft, offset);
					Quadtree bottomRight = (*quadtree).getChild(3);
					insertIntoQuadtree(dataEntry, &bottomRight, offset);
				}
			}
			else { // if there's room, create a new dataset
				DataSet newDataSet(dataEntry.PRIM_LAT_DEC, dataEntry.PRIM_LONG_DEC);
				newDataSet.addOffset(offset);
				quadtree->dataSets.push_back(newDataSet);
			}
		}
	}
}

int main()
{
	vector<GISDataEntry> data;
	string line;
	ifstream outputFile("VA_Monterey.txt");
	Hashtable<string> hashtable = Hashtable<string>();
	float maxLat = 90;
	float minLat = -90;
	float maxLong = 180;
	float minLong = -180;
	Rectangle rect1 = Rectangle(maxLat, minLat, maxLong, minLong);
	Quadtree quadtree = Quadtree(4, rect1);
	if (outputFile.is_open())
	{
		int offset = 0;
		while (getline(outputFile, line))
		{
			if (offset != 0) {
				GISDataEntry dataEntry(line);
				data.push_back(dataEntry);
				string key = dataEntry.FEATURE_NAME + ' ' + dataEntry.STATE_ALPHA;
				hashtable.insert(key, offset);

				insertIntoQuadtree(dataEntry, &quadtree, offset);
			}
			offset++;
		}
	}
	outputFile.close();

	queue<tuple<string, string, float, float, vector<int>>> buffer;

	//Part 2: Retrieving data for all GIS records matching given geographic coordinates
	float latitude = 38.4353981;
	float longitude = -79.5533807;
	if (buffer.size() == 0 || latitude != get<2>(buffer.back()) || longitude != get<3>(buffer.back())) {
		if (buffer.size() == 15) {
			buffer.pop();
		}
		vector<int> offsets = quadtree.find(latitude, longitude);
		if (offsets.size() > 0) {
			tuple<string, string, float, float, vector<int>> newData("", "", latitude, longitude, offsets);
			buffer.push(newData);
			for (int i = 0; i < offsets.size(); i++) {
				int offset = get<4>(buffer.back())[i];
				data[offset - 1].print();
			}
		}
		else {
			cout << "not found";
		}
	}
	else {
		for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
			int offset = get<4>(buffer.back())[i];
			data[offset - 1].print();
		}
	}

	//Part 3: Retrieving data for all GIS records matching a given feature name and state
	string featureName = "Aguaje Draw";
	string state = "AZ";


	if (buffer.size() == 0 || featureName != get<0>(buffer.back()) || state != get<1>(buffer.back())) {
		if (buffer.size() == 15) {
			buffer.pop();
		}
		vector<int> offsets = hashtable.search(featureName + ' ' + state);
		if (offsets.size()>0) {
			tuple<string, string, float, float, vector<int>> newData(featureName, state, -1, -1, offsets);
			buffer.push(newData);
			for (int i = 0; i < offsets.size(); i++) {
				int offset = get<4>(buffer.back())[i];
				data[offset - 1].print();
			}
		}
		else {
			cout << "not found";
		}
	}
	else {
		for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
			int offset = get<4>(buffer.back())[i];
			data[offset - 1].print();
		}
	}


	//Part 4: Retrieving data for all GIS records that fall within a given (rectangular) geographic region centered on a geographic coordinate
	latitude = 34.5714281;
	longitude = -109.2203696;
	float halfHeight = 20;
	float halfWidth = 20;
	if (buffer.size() == 0 || latitude != get<2>(buffer.back()) || longitude != get<3>(buffer.back())) {
		if (buffer.size() == 15) {
			buffer.pop();
		}
		vector<int> offsets = quadtree.find(latitude, longitude, halfHeight, halfWidth);
		if (offsets.size() > 0) {
			tuple<string, string, float, float, vector<int>> newData("", "", latitude, longitude, offsets);
			buffer.push(newData);
			for (int i = 0; i < offsets.size(); i++) {
				int offset = get<4>(buffer.back())[i];
				data[offset - 1].print();
			}
		}
		else {
			cout << "not found";
		}
	}
	else {
		for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
			int offset = get<4>(buffer.back())[i];
			data[offset - 1].print();
		}
	}
	
	//Part 5: Displaying the in-memory indices in a human-readable manner
	hashtable.print();
	quadtree.print();

	return 0;
}

