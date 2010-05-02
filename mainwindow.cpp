#include <assert.h>
#include <iostream>
#include <QKeyEvent>
#include <QtGui>
#include "mainwindow.h"
#include "simulationthread.h"
#include "fileformat.h"
#include "visframe.h"
#include "playbackslider.h"

MainWindow::MainWindow()
{
	fileCutter = NULL;

	glWidget = new GLWidget;
	setCentralWidget(glWidget);

	mainToolbar = new QToolBar("main toolbar", this);

	actionStart = new QAction("start", this);
	mainToolbar->addAction(actionStart);
	connect(actionStart, SIGNAL(triggered()), this, SLOT(slotStart()));

	actionFileOpen = new QAction("open", this);
	mainToolbar->addAction(actionFileOpen);
	connect(actionFileOpen, SIGNAL(triggered()), this, SLOT(slotFileOpen()));

	addToolBar(Qt::TopToolBarArea, mainToolbar);

	playbackToolbar = new QToolBar("playback toolbar", this);
	playbackSlider = new PlaybackSlider(playbackToolbar);
	playbackToolbar->addWidget(playbackSlider);
	addToolBar(Qt::BottomToolBarArea, playbackToolbar);

//--------------------------
	thread = new SimulationThread(this);
	thread->start();
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	switch(event->key())
	{
	case Qt::Key_Left:  glWidget->rotateY(-100); break;
	case Qt::Key_Right: glWidget->rotateY( 100); break;
	default: break;
	}
}

void MainWindow::slotStart() {
	printf("start\n");
	thread->enableSim();
}

void MainWindow::loadScene(VisFrame *f)
{
	glWidget->setVisFrame(f);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	thread->terminate();
	QMainWindow::closeEvent(event);
}

void MainWindow::rotateY(double dy)
{
	glWidget->rotateY(dy);
}

void MainWindow::openFile(const QString &fn)
{
	assert(fileCutter == NULL);

	fileName = fn;
	fileCutter = new FileFormat();
	fileCutter->openFile(fn.toLatin1());
}

void MainWindow::slotFileOpen()
{
	QString fn = QFileDialog::getOpenFileName(this,
		tr("Open saved file"), ".",
		tr("Recordings (*.bin)"));

	openFile(fn);
}

void MainWindow::loadFrame(int index)
{
	std::vector<VertexStruct> v;
	std::vector<EdgeStruct> e;
	fileCutter->readFrame(index, v, e);

	VisFrame *a = new VisFrame;

	int nVertices = (int)v.size();
	for (int i = 0; i < nVertices; i ++)
	{
		printf("(%f, %f, %f)\n", v[i].x, v[i].y, v[i].z);
		a->addVertex(v[i].x, v[i].y, v[i].z, 0.1);
	}

	loadScene(a);
}

void MainWindow::loadNextFrame()
{
	if (currentFrameIndex < fileCutter->nFrames() - 1)
	{
		loadFrame(++currentFrameIndex);
	}
	else
	{
		assert(0);
	}
}

