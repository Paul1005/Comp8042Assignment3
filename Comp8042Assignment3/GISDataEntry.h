#include <string>
#include <iostream>

using namespace std;
#pragma once
class GISDataEntry
{ 
public:
	GISDataEntry(string line) {
		char delimiter = '|';

		int currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			FEATURE_ID = NULL;
		}
		else {
			FEATURE_ID = stoi(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			FEATURE_NAME = "";
		}
		else {
			FEATURE_NAME = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			FEATURE_CLASS = "";
		}
		else {
			FEATURE_CLASS = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			STATE_ALPHA = "";
		}
		else {
			STATE_ALPHA = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			STATE_NUMERIC = NULL;
		}
		else {
			STATE_NUMERIC = stoi(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			COUNTY_NAME = "";
		}
		else {
			COUNTY_NAME = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			COUNTY_NUMERIC = NULL;
		}
		else {
			COUNTY_NUMERIC = stoi(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			PRIMARY_LAT_DMS = "";
		}
		else {
			PRIMARY_LAT_DMS = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			PRIM_LONG_DMS = "";
		}
		else {
			PRIM_LONG_DMS = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			PRIM_LAT_DEC = NULL;
		}
		else {
			PRIM_LAT_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			PRIM_LONG_DEC = NULL;
		}
		else {
			PRIM_LONG_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			SOURCE_LAT_DMS = "";
		}
		else {
			SOURCE_LAT_DMS = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			SOURCE_LONG_DMS = "";
		}
		else {
			SOURCE_LONG_DMS = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			SOURCE_LAT_DEC = NULL;
		}
		else {
			SOURCE_LAT_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			SOURCE_LONG_DEC = NULL;
		}
		else {
			SOURCE_LONG_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			ELEV_IN_M = NULL;
		}
		else {
			ELEV_IN_M = stoi(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			ELEV_IN_FT = NULL;
		}
		else {
			ELEV_IN_FT = stoi(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			MAP_NAME = "";
		}
		else {
			MAP_NAME = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			DATE_CREATED = "";
		}
		else {
			DATE_CREATED = line.substr(0, currentPosition);
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			DATE_EDITED = "";
		}
		else {
			DATE_EDITED = line.substr(0, currentPosition);
		}
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

