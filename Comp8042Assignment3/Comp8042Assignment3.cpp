// Comp8042Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>        
#include <stdio.h>
#include <string.h>
#include "Hashtable.h"
#include "GISDataEntry.h"
#include "Quadtree.h"
#include "DataSet.h"

using namespace std;

float formatCoordinate(string coordinate) {
	char direction = coordinate[coordinate.size() - 1];
	coordinate = coordinate.substr(0, coordinate.size() - 1);
	float coordinateNum = stof(coordinate);

	if (direction == 'S' || direction == 'W') {
		coordinateNum *= -1;
	}
	return coordinateNum;
}

vector<string> split(const string str, const string delim)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == string::npos) pos = str.length();
		string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

int main()
{
	string databaseFileName = "db.txt";
	string commandScriptFileName = "DemoScript01.txt";
	string logFileName = "Log01.txt";
	string recordFileName = "NM_All.txt";

	ofstream logFile;
	logFile.open(logFileName);

	vector<GISDataEntry> data;
	string scriptLine;
	ifstream commandScriptFile(commandScriptFileName);
	float maxLat;
	float minLat;
	float maxLong;
	float minLong;
	Quadtree quadtree;
	Hashtable<string> hashtable = Hashtable<string>();
	float latitude;
	float longitude;
	string featureName;
	string state;
	float halfHeight;
	float halfWidth;
	queue<tuple<string, string, float, float, vector<int>>> buffer;
	int lineNumber = 1;
	if (commandScriptFile.is_open())
	{
		int commandNum = 1;
		while (getline(commandScriptFile, scriptLine))
		{
			if (lineNumber == 1 || lineNumber == 2 || lineNumber == 3) {
				logFile << scriptLine << endl;
			}
			if (scriptLine[0] != ';') {
				vector<string> splitLine = split(scriptLine, "\t");
				string command = splitLine[0];

				if (command == "world") {
					minLong = formatCoordinate(splitLine[1]);
					maxLong = formatCoordinate(splitLine[2]);
					minLat = formatCoordinate(splitLine[3]);
					maxLat = formatCoordinate(splitLine[4]);

					Rectangle rect1 = Rectangle(maxLat, minLat, maxLong, minLong);
					quadtree = Quadtree(4, rect1);

					logFile << scriptLine << endl;
					logFile << endl;
					logFile << "GIS Program" << endl;
					logFile << endl;
					logFile << "dbFile:\t\t" << databaseFileName << endl;
					logFile << "script:\t\t" << commandScriptFileName << endl;
					logFile << "log:\t\t" << logFileName << endl;
					logFile << "Start time: " << time << endl;
					logFile << "Quadtree children are printed in the order SW  SE  NE  NW" << endl;

				}
				else if (command == "import") {
					ofstream importDatabaseFile(databaseFileName);
					recordFileName = splitLine[1];
					ifstream recordFile(recordFileName);
					string recordLine;
					if (recordFile.is_open())
					{
						int offset = 0;
						while (getline(recordFile, recordLine))
						{
							if (offset != 0) {
								importDatabaseFile << recordLine << endl;
							}
							offset++;
						}
					}
					recordFile.close();

					ifstream exportDatabaseFile(databaseFileName);
					string databaseLine;
					if (exportDatabaseFile.is_open())
					{
						int offset = 0;
						while (getline(exportDatabaseFile, databaseLine))
						{
							GISDataEntry dataEntry(databaseLine);
							data.push_back(dataEntry);
							string key = dataEntry.FEATURE_NAME + ' ' + dataEntry.STATE_ALPHA;
							hashtable.insert(key, offset);
							quadtree.insert(dataEntry, offset);
							offset++;
						}
					}
					exportDatabaseFile.close();
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					commandNum++;
				}
				else if (command == "debug") {
					string subcommand = splitLine[1];
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					if (subcommand == "quad") {
						quadtree.print(&logFile);
					}
					else if (subcommand == "hash") {
						hashtable.print(&logFile);
					}
					else if (subcommand == "pool") {
						queue<tuple<string, string, float, float, vector<int>>> printbuffer = buffer;
						for (int i = 0; i < buffer.size(); i++)
						{
							logFile << get<0>(printbuffer.back()) << endl;
							logFile << get<1>(printbuffer.back()) << endl;
							logFile << get<2>(printbuffer.back()) << endl;
							logFile << get<3>(printbuffer.back()) << endl;
							for (int j = 0; j < get<4>(printbuffer.back()).size(); j++) {
								logFile << get<4>(printbuffer.back())[j] << endl;
							}
							printbuffer.pop();
						}
					}
					commandNum++;
				}
				else if (command == "quit") {
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					commandScriptFile.close();
					return 0;
				}
				else if (command == "what_is_at") {
					latitude = formatCoordinate(splitLine[1]);
					longitude = formatCoordinate(splitLine[2]);
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
								data[offset - 1].print(&logFile);
							}
						}
						else {
							cout << "not found" << endl;
						}
					}
					else {
						for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
							int offset = get<4>(buffer.back())[i];
							data[offset - 1].print(&logFile);
						}
					}
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					commandNum++;
				}
				else if (command == "what_is") {
					featureName = splitLine[1];
					state = splitLine[2];
					if (buffer.size() == 0 || featureName != get<0>(buffer.back()) || state != get<1>(buffer.back())) {
						if (buffer.size() == 15) {
							buffer.pop();
						}
						vector<int> offsets = hashtable.search(featureName + ' ' + state);
						if (offsets.size() > 0) {
							tuple<string, string, float, float, vector<int>> newData(featureName, state, -1, -1, offsets);
							buffer.push(newData);
							for (int i = 0; i < offsets.size(); i++) {
								int offset = get<4>(buffer.back())[i];
								data[offset - 1].print(&logFile);
							}
						}
						else {
							cout << "not found" << endl;
						}
					}
					else {
						for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
							int offset = get<4>(buffer.back())[i];
							data[offset - 1].print(&logFile);
						}
					}
					logFile << "Command " << commandNum << ":\t" << scriptLine;
					commandNum++;
				}
				else if (command == "what_is_in") {
					latitude = formatCoordinate(splitLine[1]);
					longitude = formatCoordinate(splitLine[2]);
					halfHeight = stof(splitLine[3]);
					halfWidth = stof(splitLine[4]);
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
								data[offset - 1].print(&logFile);
							}
						}
						else {
							cout << "not found" << endl;
						}
					}
					else {
						for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
							int offset = get<4>(buffer.back())[i];
							data[offset - 1].print(&logFile);
						}
					}
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					commandNum++;
				}
			}
			lineNumber++;
		}
	}
	logFile.close();
	return 0;
}

