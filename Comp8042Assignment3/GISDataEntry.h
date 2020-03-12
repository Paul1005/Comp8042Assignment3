#include <string>
#include <iostream>

using namespace std;
#pragma once
class GISDataEntry
{ //401|Aguaje Draw|Valley|AZ|04|Apache|001|343417N|1091313W|34.5714281|-109.2203696|344308N|1085826W|34.7188|-108.9739|1750|5741|Kearn Lake|02/08/1980|01/14/2008.
public:
	GISDataEntry(string line) {
		char delimiter = '|';
		//int numOfDelims = 19;

		int currentPosition = line.find(delimiter);
		FEATURE_ID = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		FEATURE_NAME = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		FEATURE_CLASS = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		STATE_ALPHA = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		STATE_NUMERIC = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		COUNTY_NAME = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		COUNTY_NUMERIC = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		PRIMARY_LAT_DMS = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		PRIM_LONG_DMS = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		PRIM_LAT_DEC = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		PRIM_LONG_DEC = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		SOURCE_LAT_DMS = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		SOURCE_LONG_DMS = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		SOURCE_LAT_DEC = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		SOURCE_LONG_DEC = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		ELEV_IN_M = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		ELEV_IN_FT = stoi(line.substr(0, currentPosition));

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		MAP_NAME = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		DATE_CREATED = line.substr(0, currentPosition);

		line = line.substr(currentPosition + 1);
		currentPosition = line.find(delimiter);
		DATE_EDITED = line.substr(0, currentPosition);
	}

	int FEATURE_ID;
	string FEATURE_NAME;
	string FEATURE_CLASS;
	string STATE_ALPHA;
	int STATE_NUMERIC;
	string COUNTY_NAME;
	int COUNTY_NUMERIC;
	string PRIMARY_LAT_DMS;
	string PRIM_LONG_DMS;
	float PRIM_LAT_DEC;
	float PRIM_LONG_DEC;
	string SOURCE_LAT_DMS;
	string SOURCE_LONG_DMS;
	float SOURCE_LAT_DEC;
	float SOURCE_LONG_DEC;
	int ELEV_IN_M;
	int ELEV_IN_FT;
	string MAP_NAME;
	string DATE_CREATED;
	string DATE_EDITED;

	void print() {
		//FEATURE_ID|FEATURE_NAME|FEATURE_CLASS|STATE_ALPHA|STATE_NUMERIC|COUNTY_NAME|COUNTY_NUMERIC|PRIMARY_LAT_DMS|PRIM_LONG_DMS|PRIM_LAT_DEC|PRIM_LONG_DEC|SOURCE_LAT_DMS|SOURCE_LONG_DMS|SOURCE_LAT_DEC|SOURCE_LONG_DEC|ELEV_IN_M|ELEV_IN_FT|MAP_NAME|DATE_CREATED|DATE_EDITED.
		cout << "FEATURE_ID: " << FEATURE_ID << '\n';
		cout << "FEATURE_NAME: " << FEATURE_NAME << '\n';
		cout << "FEATURE_CLASS: " << FEATURE_CLASS << '\n';
		cout << "STATE_ALPHA: " << STATE_ALPHA << '\n';
		cout << "STATE_NUMERIC: " << STATE_NUMERIC << '\n';
		cout << "COUNTY_NAME: " << COUNTY_NAME << '\n';
		cout << "COUNTY_NUMERIC: " << COUNTY_NUMERIC << '\n';
		cout << "PRIMARY_LAT_DMS: " << PRIMARY_LAT_DMS << '\n';
		cout << "PRIM_LONG_DMS: " << PRIM_LONG_DMS << '\n';
		cout << "PRIM_LAT_DEC: " << PRIM_LAT_DEC << '\n';
		cout << "PRIM_LONG_DEC: " << PRIM_LONG_DEC << '\n';
		cout << "SOURCE_LAT_DMS: " << SOURCE_LAT_DMS << '\n';
		cout << "SOURCE_LONG_DMS: " << SOURCE_LONG_DMS << '\n';
		cout << "SOURCE_LAT_DEC: " << SOURCE_LAT_DEC << '\n';
		cout << "SOURCE_LONG_DEC: " << SOURCE_LONG_DEC << '\n';
		cout << "ELEV_IN_M: " << ELEV_IN_M << '\n';
		cout << "ELEV_IN_FT: " << ELEV_IN_FT << '\n';
		cout << "MAP_NAME: " << MAP_NAME << '\n';
		cout << "DATE_CREATED: " << DATE_CREATED << '\n';
		cout << "DATE_EDITED: " << DATE_EDITED << '\n' << '\n';
	}
};

