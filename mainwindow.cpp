#include <iostream>
#include <QKeyEvent>
#include <QtGui>
#include "mainwindow.h"
#include "simulationthread.h"

MainWindow::MainWindow()
{
	glWidget = new GLWidget;
	setCentralWidget(glWidget);

	mainToolbar = new QToolBar("main toolbar", this);
	actionStart = new QAction("start", this);
	mainToolbar->addAction(actionStart);

	connect(actionStart, SIGNAL(triggered()), this, SLOT(start()));

	addToolBar(Qt::TopToolBarArea, mainToolbar);

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

void MainWindow::start()
{
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

