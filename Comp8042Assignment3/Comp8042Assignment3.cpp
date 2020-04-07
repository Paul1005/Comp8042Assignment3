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

int main()
{
	vector<GISDataEntry> data;
	string line;
	ifstream outputFile("VA_Monterey.txt");
	Hashtable<string> hashtable = Hashtable<string>();
	float maxLat = 38.6;
	float minLat = 38.4;
	float maxLong = -79.4;
	float minLong = -79.6;
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
				quadtree.insert(dataEntry, offset);
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
			cout << "not found" << endl;
		}
	}
	else {
		for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
			int offset = get<4>(buffer.back())[i];
			data[offset - 1].print();
		}
	}

	//Part 3: Retrieving data for all GIS records matching a given feature name and state
	string featureName = "Asbury Church";
	string state = "VA";


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
			cout << "not found" << endl;
		}
	}
	else {
		for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
			int offset = get<4>(buffer.back())[i];
			data[offset - 1].print();
		}
	}


	//Part 4: Retrieving data for all GIS records that fall within a given (rectangular) geographic region centered on a geographic coordinate
	latitude = 38.4353981;
	longitude = -79.5533807;
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
			cout << "not found" << endl;
		}
	}
	else {
		for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
			int offset = get<4>(buffer.back())[i];
			data[offset - 1].print();
		}
	}
	
	//Part 5: Displaying the in-memory indices in a human-readable manner
	//hashtable.print();
	quadtree.print();

	return 0;
}

