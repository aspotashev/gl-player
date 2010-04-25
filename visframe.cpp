#include "visframe.h"

Vertex::Vertex(float x, float y, float z, float r)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
}

void VisFrame::addVertex(float x, float y, float z, float r)
{
	v.push_back(Vertex(x, y, z, r));
}

int VisFrame::nVertex() const
{
	return (int)v.size();
}

const Vertex &VisFrame::vertex(int i)
{
	return v[i];
}

