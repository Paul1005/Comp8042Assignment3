#include <array>
using namespace std;
// based on https://codereview.stackexchange.com/questions/154226/simple-quadtree-template-implementation 
#pragma once
template <typename T>
class Quadtree
{
public:
	typedef std::array<Quadtree, 4> Children; // class scope typedef

	T data; // the template data
	Quadtree* parent;

	Quadtree(T _data) // constructor
	{
		data = _data;
	}

	void setChildren(Children nodes)
	{
		children = &nodes;
	}

	void setChild(int _index, Quadtree node)
	{
		children[_index] = &node;
	}

	Quadtree getChild(int _index) // keeps syntax consistent with std::array
	{
		return (*children)[_index];
	}

private:
	Children* children;
};

