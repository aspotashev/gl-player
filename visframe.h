#ifndef __VISFRAME_H__
#define __VISFRAME_H__

#include <vector>
#include "visframe.h"

class Vertex
{
public:
	float x, y, z, r;

	Vertex(float x, float y, float z, float r);
};

class VisFrame
{
public:
	void addVertex(float x, float y, float z, float r);
	int nVertex() const;
	const Vertex &vertex(int i);
private:
	std::vector<Vertex> v;
};

#endif
