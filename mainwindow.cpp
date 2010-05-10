#include <assert.h>
#include <iostream>
#include <QKeyEvent>
#include <QtGui>
#include "mainwindow.h"
#include "fileformat.h"
#include "visframe.h"
#include "playbackslider.h"
#include "timeplot.h"

MainWindow::MainWindow()
{
	resize(500, 500);

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

	timePlot = new TimePlot();
	addDockWidget(Qt::BottomDockWidgetArea, timePlot);

	connect(playbackSlider, SIGNAL(valueChanged(int)), this, SLOT(slotGotoFrame(int)));
	connect(playbackSlider, SIGNAL(valueChanged(int)), timePlot, SLOT(moveCurrentMark(int)));
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
}

void MainWindow::loadScene(VisFrame *f)
{
	glWidget->setVisFrame(f);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	QMainWindow::closeEvent(event);
}

void MainWindow::rotateY(double dy)
{
	glWidget->rotateY(dy);
}

void MainWindow::openFile(const QString &fn)
{
	if (fileCutter != NULL)
	{
		closeFile();
	}

	fileName = fn;
	fileCutter = new FileFormat();
	fileCutter->openFile(fn);

	playbackSlider->setRange(0, fileCutter->nFrames() - 1);
	currentFrameIndex = 0;
	loadFrame(0);
	timePlot->loadData(fileCutter->loadPressureData());
	timePlot->moveCurrentMark(0);
}

void MainWindow::closeFile()
{
	delete fileCutter;
	fileCutter = NULL;

	timePlot->clear();
}

void MainWindow::slotFileOpen()
{
	QString fn = QFileDialog::getOpenFileName(this,
		tr("Open saved file"), ".",
		tr("Recordings (*.bin)"));

	if (!fn.isNull())
	{
		openFile(fn);
	}
}

void MainWindow::loadFrame(int index)
{
	if (fileCutter == NULL ||
		index < 0 ||
		index >= fileCutter->nVertices())
	{
		return;
	}

	std::vector<VertexStruct> v;
	std::vector<EdgeStruct> e;
	fileCutter->readFrame(index, v, e);

	VisFrame *a = new VisFrame;

	int nVertices = (int)v.size();
	for (int i = 0; i < nVertices; i ++)
	{
		a->addVertex(v[i].x, v[i].y, v[i].z, 0.1);
	}

	int nEdges = (int)e.size();
	for (int i = 0; i < nEdges; i ++)
	{
		a->addEdge(e[i].a, e[i].b);
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

void MainWindow::slotGotoFrame(int index)
{
	loadFrame(index);
}

