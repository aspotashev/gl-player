#include <stdio.h>
#include "simulationthread.h"
#include "visframe.h"
#include "mainwindow.h"
#include "fileformat.h"

SimulationThread::SimulationThread(MainWindow *mw)
{
	this->mw = mw;
}

void SimulationThread::loadFrame(FileFormat *f, int index)
{
	std::vector<VertexStruct> v;
	std::vector<EdgeStruct> e;
	f->readFrame(index, v, e);

	VisFrame *a = new VisFrame;

	int nVertices = (int)v.size();
	for (int i = 0; i < nVertices; i ++)
	{
		printf("(%f, %f, %f)\n", v[i].x, v[i].y, v[i].z);
		a->addVertex(v[i].x, v[i].y, v[i].z, 0.1);
	}

	mw->loadScene(a);
}

void SimulationThread::run()
{
	FileFormat *f = new FileFormat();
	f->openFile("temp.bin");

	loadFrame(f, 0);

	float vx = 0.0;

	int index = 0;

	while (1)
	{
//		if (sim_lock.available())
//		{

//		}

		printf("t");
		fflush(stdout);
		msleep(2000);

		loadFrame(f, 30*(++index));

//		mw->rotateY(5.0);
	}
}

void SimulationThread::enableSim()
{
	sim_lock.release();
}

