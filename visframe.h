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

class Edge
{
public:
	int a, b;

	Edge(int a, int b);
};

class VisFrame
{
public:
	void addVertex(float x, float y, float z, float r);
	int nVertices() const;
	const Vertex &vertex(int i);

	void addEdge(int a, int b);
	int nEdges() const;
	const Edge &edge(int i);
private:
	std::vector<Vertex> v;
	std::vector<Edge> e;
};

#endif
