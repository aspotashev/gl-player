#include "mainwindow.h"

MainWindow::MainWindow()
{
	glWidget = new GLWidget;
	setCentralWidget(glWidget);
}

