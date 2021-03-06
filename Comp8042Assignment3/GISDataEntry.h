#include <string>
#include <iostream>
#include <fstream>

using namespace std;
#pragma once
class GISDataEntry
{ 
public:
	GISDataEntry(string line) {
		char delimiter = '|';

		int currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			FEATURE_ID = -1;
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
			STATE_NUMERIC = -1;
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
			COUNTY_NUMERIC = -1;
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
			PRIM_LAT_DEC = -1;
		}
		else {
			PRIM_LAT_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			PRIM_LONG_DEC = -1;
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
			SOURCE_LAT_DEC = -1;
		}
		else {
			SOURCE_LAT_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			SOURCE_LONG_DEC = -1;
		}
		else {
			SOURCE_LONG_DEC = stof(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			ELEV_IN_M = -1;
		}
		else {
			ELEV_IN_M = stoi(line.substr(0, currentPosition));
		}

		line = line.substr(currentPosition + 1);

		currentPosition = line.find(delimiter);
		if (currentPosition == 0) {
			ELEV_IN_FT = -1;
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

	void printlong(ofstream* logFile) {
		*logFile << "FEATURE_ID: " << FEATURE_ID << '\n';
		*logFile << "FEATURE_NAME: " << FEATURE_NAME << '\n';
		*logFile << "FEATURE_CLASS: " << FEATURE_CLASS << '\n';
		*logFile << "STATE_ALPHA: " << STATE_ALPHA << '\n';
		*logFile << "STATE_NUMERIC: " << STATE_NUMERIC << '\n';
		*logFile << "COUNTY_NAME: " << COUNTY_NAME << '\n';
		*logFile << "COUNTY_NUMERIC: " << COUNTY_NUMERIC << '\n';
		*logFile << "PRIMARY_LAT_DMS: " << PRIMARY_LAT_DMS << '\n';
		*logFile << "PRIM_LONG_DMS: " << PRIM_LONG_DMS << '\n';
		*logFile << "PRIM_LAT_DEC: " << PRIM_LAT_DEC << '\n';
		*logFile << "PRIM_LONG_DEC: " << PRIM_LONG_DEC << '\n';
		*logFile << "SOURCE_LAT_DMS: " << SOURCE_LAT_DMS << '\n';
		*logFile << "SOURCE_LONG_DMS: " << SOURCE_LONG_DMS << '\n';
		*logFile << "SOURCE_LAT_DEC: " << SOURCE_LAT_DEC << '\n';
		*logFile << "SOURCE_LONG_DEC: " << SOURCE_LONG_DEC << '\n';
		*logFile << "ELEV_IN_M: " << ELEV_IN_M << '\n';
		*logFile << "ELEV_IN_FT: " << ELEV_IN_FT << '\n';
		*logFile << "MAP_NAME: " << MAP_NAME << '\n';
		*logFile << "DATE_CREATED: " << DATE_CREATED << '\n';
		*logFile << "DATE_EDITED: " << DATE_EDITED << '\n' << '\n';
	}

	void printwhat_is(ofstream* logFile) {
		//11390:  Ouray  (107d 44m 21s West, 37d 57m 24s North)
		*logFile  << COUNTY_NAME << "  (" << PRIM_LONG_DMS << ", " << PRIMARY_LAT_DMS << ")" << endl;
	}

	void printwhat_is_at(ofstream* logFile) {
		//39239:  Radium Springs Swimming Pool  Ouray  CO
		*logFile << FEATURE_NAME << "  " << COUNTY_NAME << "  " << STATE_ALPHA << endl;
	}

	void printwhat_is_in(ofstream* logFile) {
		//57123: Ouray  CO(107d 40m 17s West, 38d 1m 22s North)
		*logFile << COUNTY_NAME << " " << STATE_ALPHA << "(" << PRIM_LONG_DMS << ", " << PRIMARY_LAT_DMS << ")" << endl;
	}
};

