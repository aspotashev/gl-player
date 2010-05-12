#include "visframe.h"

Vertex::Vertex(float x, float y, float z, float r)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = r;
}

//---------------------------------

Edge::Edge(int a, int b)
{
	this->a = a;
	this->b = b;
}

//---------------------------------

void VisFrame::addVertex(float x, float y, float z, float r)
{
	v.push_back(Vertex(x, y, z, r));
}

int VisFrame::nVertices() const
{
	return (int)v.size();
}

const Vertex &VisFrame::vertex(int i)
{
	return v[i];
}

void VisFrame::addEdge(int a, int b)
{
	e.push_back(Edge(a, b));
}

int VisFrame::nEdges() const
{
	return (int)e.size();
}

const Edge &VisFrame::edge(int i)
{
	return e[i];
}

std::vector<Edge> VisFrame::edges()
{
	return e;
}

