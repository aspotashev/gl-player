#include <stdio.h>
#include "simulationthread.h"
#include "visframe.h"
#include "mainwindow.h"
#include "fileformat.h"

SimulationThread::SimulationThread(MainWindow *mw)
{
	this->mw = mw;
}

void SimulationThread::run()
{
	FileFormat *f = new FileFormat();
	f->openFile("../temp.bin");

	std::vector<VertexStruct> v;
	std::vector<EdgeStruct> e;
	f->readFrame(0, v, e);

	int nVertices = (int)v.size();
	for (int i = 0; i < nVertices; i ++)
	{
		printf("(%f, %f, %f)\n", v[i].x, v[i].y, v[i].z);
	}


	float vx = 0.0;

	while (1)
	{
//		if (sim_lock.available())
//		{
			VisFrame *a = new VisFrame;
			a->addVertex(vx, 0, 0, 0.5);
			a->addVertex(-vx, 0, 0, 0.5);
			mw->loadScene(a);
			vx += 0.1;
//		}

		printf("t");
		fflush(stdout);
		msleep(200);
	}
}

void SimulationThread::enableSim()
{
	sim_lock.release();
}

