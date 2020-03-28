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

void insertIntoQuadtree(GISDataEntry dataEntry, Quadtree<DataSet>& quadtree, int offset) {
	if (dataEntry.PRIM_LAT_DEC < quadtree.area.maxLat && dataEntry.PRIM_LAT_DEC > quadtree.area.minLat&& dataEntry.PRIM_LONG_DEC <quadtree.area.maxLong && dataEntry.PRIM_LONG_DEC > quadtree.area.minLong) { // if it fits in the designated area
		bool wasAddedToExistingDataSet = false;
		for (int i; i < quadtree.dataSets.size(); i++) { // see if we can add it to an existing data point
			if (quadtree.dataSets[i].coordinate.first == dataEntry.PRIM_LAT_DEC && quadtree.dataSets[i].coordinate.second == dataEntry.PRIM_LONG_DEC) {
				quadtree.dataSets[i].addOffset(offset);
				wasAddedToExistingDataSet = true;
				break;
			}
		}

		if (!wasAddedToExistingDataSet) { // if we can't
			if (quadtree.dataSets.size() == quadtree.K) { // if we already have K data points
				if (quadtree.getChildren().size() == 0) { // if node hasn't already been partitioned
					// create quadtree children
					float top = quadtree.area.maxLat;
					float bottom = quadtree.area.minLat;
					float left = quadtree.area.minLong;
					float right = quadtree.area.maxLong;
					
					Rectangle topLeftRectangle(top, top / 2, right / 2, left);
					Quadtree<DataSet> topLeft(4, topLeftRectangle);
					
					Rectangle topRightRectangle(top, top / 2, right, right / 2);
					Quadtree<DataSet> topRight(4, topRightRectangle);
					
					Rectangle bottomLeftRectangle(top / 2, bottom, right / 2, left);
					Quadtree<DataSet> bottomLeft(4, bottomLeftRectangle);
					
					Rectangle bottomRightRectangle(top / 2, bottom, right, right / 2);
					Quadtree<DataSet> bottomRight(4, bottomRightRectangle);

					for (int i = 0; i < quadtree.dataSets.size(); i++) { // insert each of our old datasets into the children
						if (quadtree.dataSets[i].coordinate.first < topLeft.area.maxLat && quadtree.dataSets[i].coordinate.first > topLeft.area.minLat&& quadtree.dataSets[i].coordinate.second <topLeft.area.maxLong && quadtree.dataSets[i].coordinate.second > topLeft.area.minLong) {
							topLeft.addDataSet(quadtree.dataSets[i]);
						}
						else if (quadtree.dataSets[i].coordinate.first < topRight.area.maxLat && quadtree.dataSets[i].coordinate.first > topRight.area.minLat&& quadtree.dataSets[i].coordinate.second <topRight.area.maxLong && quadtree.dataSets[i].coordinate.second > topRight.area.minLong) {
							topRight.addDataSet(quadtree.dataSets[i]);;
						}
						else if (quadtree.dataSets[i].coordinate.first < bottomLeft.area.maxLat && quadtree.dataSets[i].coordinate.first > bottomLeft.area.minLat&& quadtree.dataSets[i].coordinate.second <bottomLeft.area.maxLong && quadtree.dataSets[i].coordinate.second > bottomLeft.area.minLong) {
							bottomLeft.addDataSet(quadtree.dataSets[i]);;
						}
						else if (quadtree.dataSets[i].coordinate.first < bottomRight.area.maxLat && quadtree.dataSets[i].coordinate.first > bottomRight.area.minLat&& quadtree.dataSets[i].coordinate.second <bottomRight.area.maxLong && quadtree.dataSets[i].coordinate.second > bottomRight.area.minLong) {
							bottomRight.addDataSet(quadtree.dataSets[i]);;
						}
					}

					// add the children to this node.
					quadtree.setChild(0, topLeft);
					quadtree.setChild(1, topRight);
					quadtree.setChild(2, bottomLeft);
					quadtree.setChild(3, bottomRight);
				}

				// insert entry into a child node
				for (int i; i < quadtree.dataSets.size(); i++) {
					Quadtree<DataSet> topLeft = quadtree.getChild(0);
					insertIntoQuadtree(dataEntry, topLeft, offset);
					Quadtree<DataSet> topRight = quadtree.getChild(1);
					insertIntoQuadtree(dataEntry, topRight, offset);
					Quadtree<DataSet> bottomLeft = quadtree.getChild(2);
					insertIntoQuadtree(dataEntry, bottomLeft, offset);
					Quadtree<DataSet> bottomRight = quadtree.getChild(3);
					insertIntoQuadtree(dataEntry, bottomRight, offset);
				}

			}
			else { // if there's room, create a new dataset
					DataSet newDataSet(dataEntry.PRIM_LAT_DEC, dataEntry.PRIM_LONG_DEC);
					newDataSet.addOffset(offset);
					quadtree.dataSets.push_back(newDataSet);
				}
			}
		}
	}

	int main()
	{
		//Part 1: Importing new GIS records into the database le
		/*ofstream importFile;
		importFile.open("New_GIS.txt", ofstream::app);
		//importFile << "FEATURE_ID|FEATURE_NAME|FEATURE_CLASS|STATE_ALPHA|STATE_NUMERIC|COUNTY_NAME|COUNTY_NUMERIC|PRIMARY_LAT_DMS|PRIM_LONG_DMS|PRIM_LAT_DEC|PRIM_LONG_DEC|SOURCE_LAT_DMS|SOURCE_LONG_DMS|SOURCE_LAT_DEC|SOURCE_LONG_DEC|ELEV_IN_M|ELEV_IN_FT|MAP_NAME|DATE_CREATED|DATE_EDITED.\n";
		importFile << "401|Aguaje Draw|Valley|AZ|04|Apache|001|343417N|1091313W|34.5714281|-109.2203696|344308N|1085826W|34.7188|-108.9739|1750|5741|Kearn Lake|02/08/1980|01/14/2008.\n";
		importFile.close();*/

		// put records in a vector off our GISDataEntry class
		vector<GISDataEntry> data;
		string line;
		ifstream outputFile("New_GIS.txt");
		Hashtable<string> hashtable = Hashtable<string>();
		float maxLat = 90;
		float minLat = -90;
		float maxLong = 180;
		float minLong = -180;
		Rectangle rect1 = Rectangle(maxLat, minLat, maxLong, minLong);
		Quadtree<DataSet> quadtree = Quadtree<DataSet>(4, rect1);
		if (outputFile.is_open())
		{
			int offset = 0;
			while (getline(outputFile, line))
			{
				if (offset != 0) {
					GISDataEntry dataEntry(line);
					data.push_back(dataEntry);
					string key = dataEntry.COUNTY_NAME + dataEntry.STATE_ALPHA;
					hashtable.insert(key, offset);

					insertIntoQuadtree(dataEntry, quadtree, offset);
				}
				offset++;
			}
		}

		queue<GISDataEntry> buffer;
		if (buffer.size() <= 15) {
			//buffer.push(something);
		}
		else {
			buffer.pop();
		}


		//Part 2: Retrieving data for all GIS records matching given geographic coordinates
		string latitude = "343417N";
		string longtitude = "1091313W";
		for (GISDataEntry entry : data) {
			if (entry.PRIMARY_LAT_DMS == latitude && entry.PRIM_LONG_DMS == longtitude) {
				cout << "Entry: " << '\n';
				entry.print();
			}
		}

		//Part 3: Retrieving data for all GIS records matching a given feature name and state
		string featureName = "Aguaje Draw";
		string state = "AZ";
		for (GISDataEntry entry : data) {
			if (entry.FEATURE_NAME == featureName && entry.STATE_ALPHA == state) {
				cout << "Entry: " << '\n';
				entry.print();
			}
		}

		//Part 4: Retrieving data for all GIS records that fall within a given (rectangular) geographic region

		//Part 5: Displaying the in-memory indices in a human-readable manner
	}

