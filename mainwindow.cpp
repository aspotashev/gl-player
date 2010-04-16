#include <iostream>
#include <QKeyEvent>
#include "mainwindow.h"

MainWindow::MainWindow()
{
	glWidget = new GLWidget;
	setCentralWidget(glWidget);
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

