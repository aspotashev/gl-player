#include <iostream>
#include <QKeyEvent>
#include <QtGui>
#include "mainwindow.h"
#include "simulationthread.h"
#include "fileformat.h"

MainWindow::MainWindow()
{
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

void MainWindow::slotFileOpen()
{
	fileName = QFileDialog::getOpenFileName(this,
		tr("Open saved file"), ".",
		tr("Recordings (*.bin)"));

	fileCutter->openFile(fileName.toLatin1());
}

