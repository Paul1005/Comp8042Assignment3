// Comp8042Assignment3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>        
#include <stdio.h>
#include <string>
#include <sstream> 
#include <time.h>
#include <algorithm> 
#include <cctype>
#include <locale>
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

//taken from https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start (in place)
static inline void ltrim(string& s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch) {
		return !std::isspace(ch);
		}));
}

// trim from end (in place)
static inline void rtrim(string& s) {
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch) {
		return !isspace(ch);
		}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(string& s) {
	ltrim(s);
	rtrim(s);
}

// taken from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
vector<string> split(const string& s, char delimiter)
{
	vector<string> tokens;
	string token;
	istringstream tokenStream(s);
	while (getline(tokenStream, token, delimiter))
	{
		trim(token);
		tokens.push_back(token);
	}
	return tokens;
}

int main(int argc, char** argv)
{
	string databaseFileName = argv[1];
	string commandScriptFileName = argv[2];
	string logFileName = argv[3];

	ofstream logFile(logFileName);

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
				vector<string> splitLine = split(scriptLine, '\t');
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
					time_t now = time(0);
					logFile << "Start time: " << now << endl;
					logFile << "Quadtree children are printed in the order SW  SE  NE  NW" << endl;
					logFile << "--------------------------------------------------------------------------------" << endl;
				}
				else if (command == "import") {
					ofstream importDatabaseFile(databaseFileName);
					string recordFileName = splitLine[1];
					ifstream recordFile(recordFileName);
					string recordLine;
					int offset = 0;
					if (recordFile.is_open())
					{
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
						offset = 0;
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
					logFile << endl;
					logFile << "Imported Features by name: " << offset << endl;
					logFile << "--------------------------------------------------------------------------------" << endl;
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
					logFile << "--------------------------------------------------------------------------------" << endl;
				}
				else if (command == "quit") {
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					logFile << endl;
					logFile << "Terminating execution of commands." << endl;
					time_t now = time(0);
					logFile << "End time: " << now << endl;
					commandScriptFile.close();
					logFile << "--------------------------------------------------------------------------------" << endl;
					return 0;
				}
				else if (command == "what_is_at") {
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
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
								logFile << offset << ": ";
								data[offset].printwhat_is_at(&logFile);
							}
						}
						else {
							logFile << "not found" << endl;
						}
					}
					else {
						for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
							int offset = get<4>(buffer.back())[i];
							logFile << offset << ": ";
							data[offset].printwhat_is_at(&logFile);
						}
					}
					commandNum++;
					logFile << "--------------------------------------------------------------------------------" << endl;
				}
				else if (command == "what_is") {
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
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
								logFile << offset << ": " ;
								data[offset].printwhat_is(&logFile);
							}
						}
						else {
							logFile << "not found" << endl;
						}
					}
					else {
						for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
							int offset = get<4>(buffer.back())[i];
							logFile << offset << ": ";
							data[offset].printwhat_is(&logFile);
						}
					}
					commandNum++;
					logFile << "--------------------------------------------------------------------------------" << endl;
				}
				else if (command == "what_is_in") {
					logFile << "Command " << commandNum << ":\t" << scriptLine << endl;
					string filter = "";
					int position = 1;
					bool isLong = false;
					if (splitLine[position] == "-long") {
						isLong = true;
						position++;
					}
					else if (splitLine[position] == "-filter") {
						position++;
						filter = splitLine[position];
						position++;
					}
					latitude = formatCoordinate(splitLine[position]);
					position++;
					longitude = formatCoordinate(splitLine[position]);
					position++;
					halfHeight = stof(splitLine[position]);
					position++;
					halfWidth = stof(splitLine[position]);
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
								if (filter == "pop") {
									if (data[offset].FEATURE_CLASS == "Populated Place") {
										logFile << offset << ": ";
										data[offset].printwhat_is_in(&logFile);
									}
								}
								else if (filter == "water") {
									if (data[offset].FEATURE_CLASS == "Arroyo" ||
										data[offset].FEATURE_CLASS == "Bay" ||
										data[offset].FEATURE_CLASS == "Bend" ||
										data[offset].FEATURE_CLASS == "Canal" ||
										data[offset].FEATURE_CLASS == "Channel" ||
										data[offset].FEATURE_CLASS == "Falls" ||
										data[offset].FEATURE_CLASS == "Glacier" ||
										data[offset].FEATURE_CLASS == "Gut" ||
										data[offset].FEATURE_CLASS == "Harbor" ||
										data[offset].FEATURE_CLASS == "Lake" ||
										data[offset].FEATURE_CLASS == "Rapids" ||
										data[offset].FEATURE_CLASS == "Reservoir" ||
										data[offset].FEATURE_CLASS == "Sea" ||
										data[offset].FEATURE_CLASS == "Spring" ||
										data[offset].FEATURE_CLASS == "Stream" ||
										data[offset].FEATURE_CLASS == "Swamp" ||
										data[offset].FEATURE_CLASS == "Well") {
										logFile << offset << ": ";
										data[offset].printwhat_is_in(&logFile);
									}
								}
								else if (filter == "structure") {
									if (data[offset].FEATURE_CLASS == "Airport" ||
										data[offset].FEATURE_CLASS == "Bridge" ||
										data[offset].FEATURE_CLASS == "Building" ||
										data[offset].FEATURE_CLASS == "Church" ||
										data[offset].FEATURE_CLASS == "Dam" ||
										data[offset].FEATURE_CLASS == "Hospital" ||
										data[offset].FEATURE_CLASS == "Levee" ||
										data[offset].FEATURE_CLASS == "Park" ||
										data[offset].FEATURE_CLASS == "Post Office" ||
										data[offset].FEATURE_CLASS == "School" ||
										data[offset].FEATURE_CLASS == "Tower" ||
										data[offset].FEATURE_CLASS == "Tunnel") {
										logFile << offset << ": ";
										data[offset].printwhat_is_in(&logFile);
									}
								}
								else {
									logFile << offset << ": ";
									if (isLong) {
										data[offset].printlong(&logFile);
									}
									else {
										data[offset].printwhat_is_in(&logFile);
									}
								}
							}
						}
						else {
							logFile << "not found" << endl;
						}
					}
					else {
						for (int i = 0; i < get<4>(buffer.back()).size(); i++) {
							int offset = get<4>(buffer.back())[i];
							if (filter == "pop") {
								if (data[offset].FEATURE_CLASS == "Populated Place") {
									logFile << offset << ": ";
									data[offset].printwhat_is_in(&logFile);
								}
							}
							else if (filter == "water") {
								if (data[offset].FEATURE_CLASS == "Arroyo" ||
									data[offset].FEATURE_CLASS == "Bay" ||
									data[offset].FEATURE_CLASS == "Bend" ||
									data[offset].FEATURE_CLASS == "Canal" ||
									data[offset].FEATURE_CLASS == "Channel" ||
									data[offset].FEATURE_CLASS == "Falls" ||
									data[offset].FEATURE_CLASS == "Glacier" ||
									data[offset].FEATURE_CLASS == "Gut" ||
									data[offset].FEATURE_CLASS == "Harbor" ||
									data[offset].FEATURE_CLASS == "Lake" ||
									data[offset].FEATURE_CLASS == "Rapids" ||
									data[offset].FEATURE_CLASS == "Reservoir" ||
									data[offset].FEATURE_CLASS == "Sea" ||
									data[offset].FEATURE_CLASS == "Spring" ||
									data[offset].FEATURE_CLASS == "Stream" ||
									data[offset].FEATURE_CLASS == "Swamp" ||
									data[offset].FEATURE_CLASS == "Well") {
									logFile << offset << ": ";
									data[offset].printwhat_is_in(&logFile);
								}
							}
							else if (filter == "structure") {
								if (data[offset].FEATURE_CLASS == "Airport" ||
									data[offset].FEATURE_CLASS == "Bridge" ||
									data[offset].FEATURE_CLASS == "Building" ||
									data[offset].FEATURE_CLASS == "Church" ||
									data[offset].FEATURE_CLASS == "Dam" ||
									data[offset].FEATURE_CLASS == "Hospital" ||
									data[offset].FEATURE_CLASS == "Levee" ||
									data[offset].FEATURE_CLASS == "Park" ||
									data[offset].FEATURE_CLASS == "Post Office" ||
									data[offset].FEATURE_CLASS == "School" ||
									data[offset].FEATURE_CLASS == "Tower" ||
									data[offset].FEATURE_CLASS == "Tunnel") {
									logFile << offset << ": ";
									data[offset].printwhat_is_in(&logFile);
								}
							}
							else {
								logFile << offset << ": ";
								if (isLong) {
									data[offset].printlong(&logFile);
								}
								else {
									data[offset].printwhat_is_in(&logFile);
								}
							}
						}
					}
					logFile << "--------------------------------------------------------------------------------" << endl;
					commandNum++;
				}
			}
			lineNumber++;
		}
	}
	logFile.close();
	return 0;
}

