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
	mw->openFile(QString("temp.bin"));
	mw->loadFrame(0);

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

//		mw->loadFrame(30*(++index));

//		mw->rotateY(5.0);
	}
}

void SimulationThread::enableSim()
{
	sim_lock.release();
}

