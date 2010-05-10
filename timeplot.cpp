#include <assert.h>
#include "timeplot.h"

TimePlot::TimePlot(QWidget *parent):
	QWidget(parent)
{
	data_max = 0;
	resize(500, 100);

	graphicsScene = new QGraphicsScene();

	graphicsView = new QGraphicsView(graphicsScene, this);
	resizeGraphicsViewToFit();
	graphicsView->show();
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

