#include <assert.h>
#include <iostream>
#include <QKeyEvent>
#include <QtGui>
#include "mainwindow.h"
#include "fileformat.h"
#include "visframe.h"
#include "playbackslider.h"
#include "timeplot.h"
#include "commentdock.h"

MainWindow::MainWindow()
{
	currentFrameIndex = 0;

	resize(500, 500);

	fileCutter = NULL;

	glWidget = new GLWidget(this);
	setCentralWidget(glWidget);
	glWidget->show();

//-------------------------------------

	mainToolbar = new QToolBar("main toolbar", this);
	mainToolbar->setIconSize(QSize(22, 22));

	actionFileOpen = new QAction(QIcon(":/images/document-open.png"), "open", this);
	mainToolbar->addAction(actionFileOpen);
	connect(actionFileOpen, SIGNAL(triggered()), this, SLOT(slotFileOpen()));

	actionVisibleBrokenEdges = new QAction(QIcon(":/images/insert-link.png"), "show broken edges", this);
	actionVisibleBrokenEdges->setCheckable(true);
	mainToolbar->addAction(actionVisibleBrokenEdges);
	connect(actionVisibleBrokenEdges, SIGNAL(toggled(bool)), glWidget, SLOT(setVisibleBrokenEdges(bool)));

	addToolBar(Qt::TopToolBarArea, mainToolbar);

//-------------------------------------

	playbackToolbar = new QToolBar("playback toolbar", this);

	actionPlaybackStart = new QAction(QIcon(":/images/media-playback-start.png"), "Play", this);
	playbackToolbar->addAction(actionPlaybackStart);
	connect(actionPlaybackStart, SIGNAL(triggered()), this, SLOT(slotPlaybackStart()));

	actionPlaybackPause = new QAction(QIcon(":/images/media-playback-pause.png"), "Pause", this);
	playbackToolbar->addAction(actionPlaybackPause);
	connect(actionPlaybackPause, SIGNAL(triggered()), this, SLOT(slotPlaybackPause()));

	playbackSlider = new PlaybackSlider(playbackToolbar);
	playbackSliderAction = playbackToolbar->addWidget(playbackSlider);
	addToolBar(Qt::BottomToolBarArea, playbackToolbar);

	timePlot = new TimePlot();
	addDockWidget(Qt::BottomDockWidgetArea, timePlot);
	timePlot->setFeatures(0);

	connect(playbackSlider, SIGNAL(valueChanged(int)), this, SLOT(slotGotoFrame(int)));
	connect(playbackSlider, SIGNAL(valueChanged(int)), timePlot, SLOT(moveCurrentMark(int)));

	actionPlaybackStart->setVisible(false);
	actionPlaybackPause->setVisible(false);

//-------------------------------

	connect(&playbackTimer, SIGNAL(timeout()), this, SLOT(slotPlaybackNextFrame()));

//-------------------------------

	setStatusBar(new QStatusBar());

//-------------------------------

	commentDock = new CommentDock;
	addDockWidget(Qt::RightDockWidgetArea, commentDock);
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
	switch(event->key())
	{
	case Qt::Key_Left:  glWidget->rotateY(-100); break;
	case Qt::Key_Right: glWidget->rotateY( 100); break;
	case Qt::Key_P:     enablePlaybackButtons(!isPlaybackButtonVisible()); break;
	case Qt::Key_L:     enablePlaybackSlider(!isPlaybackSliderVisible()); break;
	case Qt::Key_H:     (new QMessageBox(QMessageBox::Critical, "Info", "Visualizer"))->show(); break;
	default: break;
	}
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
	QFile file(fn + ".param.txt");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream in(&file);
		QString firstLine = in.readLine();
		QStringList firstLineArgs = firstLine.split(" ");

		timePlot->setZeroFrame(firstLineArgs[0].toInt());
		timePlot->setEpsilonCoeff(firstLineArgs[1].toFloat());
		timePlot->resizeGraphicsViewToFit();

		fileComment = in.readAll();
		commentDock->setText(fileComment);
	}
	else
	{
		timePlot->setZeroFrame(0);
		timePlot->setEpsilonCoeff(1.0);
		timePlot->resizeGraphicsViewToFit();

		fileComment = "<no info>";
		commentDock->setText(fileComment);
	}

//---------------------------

	if (fileCutter != NULL)
	{
		closeFile();
	}

	fileName = fn;
	fileCutter = new FileFormat();
	fileCutter->openFile(fn);

	playbackSlider->setRange(0, fileCutter->nFrames() - 1);
	currentFrameIndex = 0;
	slotGotoFrame(0);
	timePlot->loadData(fileCutter->loadPressureData());
	timePlot->moveCurrentMark(0);
	timePlot->resizeGraphicsViewToFit();
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

	currentFrameIndex = index;

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

bool MainWindow::isLastFrame() const
{
	return currentFrameIndex >= fileCutter->nFrames() - 1;
}

void MainWindow::slotGotoFrame(int index)
{
	loadFrame(index);
	playbackSlider->setValue(currentFrameIndex);
	statusBar()->showMessage(QString("Frame: %1").arg(currentFrameIndex));
}

int MainWindow::frameIndex() const
{
	return currentFrameIndex;
}

void MainWindow::slotPlaybackStart()
{
	playbackTimer.start(50);
}

void MainWindow::slotPlaybackPause()
{
	playbackTimer.stop();
}

void MainWindow::slotPlaybackNextFrame()
{
	if (isLastFrame())
	{
		slotPlaybackPause();
	}
	else
	{
		slotGotoFrame(++currentFrameIndex);
	}
}

void MainWindow::enablePlaybackButtons(bool enable)
{
	actionPlaybackStart->setVisible(enable);
	actionPlaybackPause->setVisible(enable);
}

bool MainWindow::isPlaybackButtonVisible()
{
	return actionPlaybackStart->isVisible();
}

void MainWindow::enablePlaybackSlider(bool enable)
{
	playbackSliderAction->setVisible(enable);
}

bool MainWindow::isPlaybackSliderVisible()
{
	return playbackSliderAction->isVisible();
}

