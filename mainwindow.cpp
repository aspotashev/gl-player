#include <iostream>
#include "mainwindow.h"

MainWindow::MainWindow()
{
	glWidget = new GLWidget;
	setCentralWidget(glWidget);
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
}

