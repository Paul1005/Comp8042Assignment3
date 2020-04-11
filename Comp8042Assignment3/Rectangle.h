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

	Rectangle() {

	}

	float getMaxLat() {
		return maxLat;
	}

	float getMinLat() {
		return minLat;
	}

	float getMaxLong() {
		return maxLong;
	}

	float getMinLong() {
		return minLong;
	}

private:
	float maxLat;
	float minLat;
	float maxLong;
	float minLong;
};

