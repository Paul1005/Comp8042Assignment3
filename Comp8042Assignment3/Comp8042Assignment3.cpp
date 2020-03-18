// Comp8042Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Hashtable.h"
#include "GISDataEntry.h"

using namespace std;

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
	if (outputFile.is_open())
	{
		int firstLine = true;
		while (getline(outputFile, line))
		{
			if (firstLine) {
				firstLine = false;
			}
			else {
				GISDataEntry dataEntry(line);
				data.push_back(dataEntry);
				string key = dataEntry.COUNTY_NAME + dataEntry.STATE_ALPHA;
				hashtable.insert(key);
			}
		}
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

