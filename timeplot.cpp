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

	leftMargin = 50;
	topMargin = 10;
	rightMargin = 10;
	bottomMargin = 10;
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
	graphicsView->setFrameShadow(QFrame::Plain);
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
	graphicsView->move(-3 + leftMargin, -3 + topMargin);
	graphicsView->resize(
		width() + 6 - leftMargin - rightMargin,
		height() + 6 - topMargin - bottomMargin);

	graphicsView->fitInView(0, 0, data.size() - 1, data_max);
}

void TimePlot::moveCurrentMark(int val)
{
	if (data.size() == 0)
	{
		return;
	}

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

QSize TimePlot::sizeHint() const
{
	return QSize(500, 100);
}

void TimePlot::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setPen(Qt::blue);
	painter.setBrush(Qt::blue);
	painter.drawRect(-1000, -1000, 2000, 2000);
}

