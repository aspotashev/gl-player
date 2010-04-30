#include "simulationthread.h"
#include "visframe.h"
#include "mainwindow.h"

SimulationThread::SimulationThread(MainWindow *mw)
{
	this->mw = mw;
}

void SimulationThread::run()
{
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

