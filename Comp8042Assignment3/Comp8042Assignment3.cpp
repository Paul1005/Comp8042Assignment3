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

float formatCoordinate(string coordinate) {
	coordinate = coordinate.substr(0, coordinate.size() - 1);
	float coordinateNum = stof(coordinate);
	char direction = coordinate[coordinate.size() - 1];

	if (direction == 'S' || direction == 'E') {
		coordinateNum *= -1;
	}
	return coordinateNum;
}

vector<std::string> split(string strToSplit, char delimeter)
{
	stringstream ss(strToSplit);
	string item;
	vector<string> splittedStrings;
	while (getline(ss, item, delimeter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

int main()
{
	string databaseFileName = "db.txt";
	string commandScriptFileName = "Script01.txt";
	string logFileName = "Log01.txt";
	string recordFileName = "NM_All.txt";

	ofstream logFile(logFileName);

	vector<GISDataEntry> data;
	string line;
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
		while (getline(commandScriptFile, line))
		{
			if (lineNumber == 1 || lineNumber == 2 || lineNumber == 3) {
				logFile << line;
			}
			if (line[0] != ';') {
				vector<string> splitLine = split(line, ';');
				string command = splitLine[0];

				if (command == "world") {
					maxLat = formatCoordinate(splitLine[1]);
					minLat = formatCoordinate(splitLine[2]);
					maxLong = formatCoordinate(splitLine[3]);
					minLong = formatCoordinate(splitLine[4]);

					Rectangle rect1 = Rectangle(maxLat, minLat, maxLong, minLong);
					quadtree = Quadtree(4, rect1);

					logFile << "; ";
					logFile << line;
					logFile << endl;
					logFile << "GIS Program";
					logFile << endl;
					logFile << "script:" << commandScriptFileName << endl;
					logFile << "log:" << logFileName << endl;
					logFile << "Start time:" << time << endl;
					logFile << "Quadtree children are printed in the order SW  SE  NE  NW" << endl;

				}
				else if (command == "import") {
					ofstream importDatabaseFile(databaseFileName);
					recordFileName = splitLine[1];
					ifstream recordFile(recordFileName);
					if (recordFile.is_open())
					{
						int offset = 0;
						while (getline(recordFile, line))
						{
							if (offset != 0) {
								importDatabaseFile << line;
							}
							offset++;
						}
					}
					recordFile.close();

					ifstream exportDatabaseFile(databaseFileName);
					if (exportDatabaseFile.is_open())
					{
						int offset = 0;
						while (getline(exportDatabaseFile, line))
						{
							GISDataEntry dataEntry(line);
							data.push_back(dataEntry);
							string key = dataEntry.FEATURE_NAME + ' ' + dataEntry.STATE_ALPHA;
							hashtable.insert(key, offset);
							quadtree.insert(dataEntry, offset);
							offset++;
						}
					}
					exportDatabaseFile.close();
					cout << "Command" << commandNum << ":" << line;
					commandNum++;
				}
				else if (command == "debug") {
					string subcommand = splitLine[1];
					if (subcommand == "quad") {
						quadtree.print();
					}
					else if (subcommand == "hash") {
						hashtable.print();
					}
					else if (subcommand == "pool") {
						queue<tuple<string, string, float, float, vector<int>>> printbuffer = buffer;
						for (int i = 0; i < buffer.size(); i++)
						{
							cout << get<0>(printbuffer.back()) << endl;
							cout << get<1>(printbuffer.back()) << endl;
							cout << get<2>(printbuffer.back()) << endl;
							cout << get<3>(printbuffer.back()) << endl;
							for (int j = 0; j < get<4>(printbuffer.back()).size(); j++) {
								cout << get<4>(printbuffer.back())[j] << endl;
							}
							printbuffer.pop();
						}
					}
					cout << "Command" << commandNum << ":" << line;
					commandNum++;
				}
				else if (command == "quit") {
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
					cout << "Command" << commandNum << ":" << line;
					commandNum++;
				}
				else if (command == "what_is") {
					featureName = splitLine[1];
					state = splitLine[2];
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
					cout << "Command" << commandNum << ":" << line;
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
					cout << "Command" << commandNum << ":" << line;
					commandNum++;
				}
			}
			lineNumber++;
		}
	}

	return 0;
}

