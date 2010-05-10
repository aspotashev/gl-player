#include <assert.h>
#include "timeplot.h"

TimePlot::TimePlot(QWidget *parent):
	QDockWidget(parent)
{
	data_max = 0;
	resize(500, 100);

	graphicsScene = new QGraphicsScene();

	graphicsView = new QGraphicsView(graphicsScene, this);
	resizeGraphicsViewToFit();
	graphicsView->show();

	currentMark = NULL;
}

TimePlot::~TimePlot()
{
}

void TimePlot::loadData(std::vector<float> data)
{
	this->data = data;

	// check that all points >= 0, calculate maximum
	for (int i = 0; i < (int)data.size(); i ++)
	{
		assert(data[i] >= 0);
		data_max = std::max(data_max, data[i]);
	}

	for (int i = 0; i < (int)data.size() - 1; i ++)
	{
		graphicsScene->addLine(
			i,     data_max - data[i],
			i + 1, data_max - data[i + 1]); // hack, we should rather
							// reverse the coordinate system
	}

	graphicsView->setSceneRect(0, 0, data.size() - 1, data_max);

	resizeGraphicsViewToFit();

	graphicsView->setFrameStyle(QFrame::NoFrame);


	moveCurrentMark(50);
}

void TimePlot::clear()
{
	data = std::vector<float>();
}

void TimePlot::resizeEvent(QResizeEvent *event)
{
	resizeGraphicsViewToFit();
}

void TimePlot::mousePressEvent(QMouseEvent *event)
{
}

void TimePlot::resizeGraphicsViewToFit()
{
	graphicsView->resize(width(), height());
	graphicsView->fitInView(0, 0, data.size() - 1, data_max);
}

void TimePlot::moveCurrentMark(int val)
{
	if (!currentMark)
	{
		QColor color = QColor(255, 0, 0);
		currentMark = graphicsScene->addRect(
			0, 0, 1, 1,
			QPen(color), QBrush(color));
	}

	graphicsScene->removeItem(currentMark);

	currentMarkPos = val;
	qreal halfWidth = 0.2;
	currentMark->setRect(val - halfWidth, 0, 2*halfWidth, data_max);
	graphicsScene->addItem(currentMark);
}

