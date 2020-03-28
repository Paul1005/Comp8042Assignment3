#include <array>
#include "Rectangle.h"

using namespace std;
// based on https://codereview.stackexchange.com/questions/154226/simple-quadtree-template-implementation 
#pragma once
template <typename T>
class Quadtree
{
public:
	int K;
	typedef array<Quadtree, 4> Children; // class scope typedef

	Rectangle area; 
	vector<T> dataSets;

	Quadtree* parent;

	Quadtree(int currentK, Rectangle currentArea) // constructor
	{
		K = currentK;
		area = currentArea;
	}

	void addDataSet(T dataSet)
	{
		if (dataSets.size() < 4) {
			dataSets.push_back(dataSet);
		}
		else {
			cout << "is full";
		}

	}

	void setChildren(Children nodes)
	{
		children = &nodes;
	}

	void setChild(int _index, Quadtree node)
	{
		children[_index] = &node;
	}

	Quadtree& getChild(int _index)
	{
		return children[_index];
	}

	Children getChildren() {
		return Children;
	}

private:
	Children* children;
};

