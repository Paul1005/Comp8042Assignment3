#include <vector>

using namespace std;

#pragma once
class DataSet
{
public:
	pair<float, float> coordinate;
	vector<int> offsets;

	DataSet(pair<float, float> currentCoordinate) : coordinate(currentCoordinate.first, currentCoordinate.second){

	}

	DataSet(float first, float second) : coordinate(first, second) {

	}

	void addOffset(int offset) {
		offsets.push_back(offset);
	}
};

