#include <utility> 

using namespace std;

#pragma once
class Rectangle
{
public:
	Rectangle(float top, float bottom, float right, float left) {
		maxLat = top;
		minLat = bottom;
		maxLong = right;
		minLong = left;
	}

	float maxLat;
	float minLat;
	float maxLong;
	float minLong;
};

