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
	currentMarkVisible = false;

	leftMargin = 40;
	topMargin = 0;
	rightMargin = 0;
	bottomMargin = 20;
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

	hideCurrentMark(); // remove (but not delete) the mark
	graphicsScene->clear(); // remove & delete all items from the scene
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

	configureGrid();
}

void TimePlot::hideCurrentMark()
{
	if (currentMark && currentMarkVisible)
	{
		graphicsScene->removeItem(currentMark);
		currentMarkVisible = false;
	}
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
		currentMarkVisible = true;
	}

	hideCurrentMark();

	currentMarkPos = val;
	qreal halfWidth = 0.2;
	currentMark->setRect(val - halfWidth, 0, 2*halfWidth, data_max);

	graphicsScene->addItem(currentMark);
	currentMarkVisible = true;
}

QSize TimePlot::sizeHint() const
{
	return QSize(500, 100);
}

void TimePlot::configureGrid()
{
// x-axis
	for (int i = 0; i < (int)xGrid.size(); i ++)
	{
		graphicsScene->removeItem(xGrid[i]);
		delete xGrid[i];
	}
	xGrid.clear();

	xGridList.clear();
	for (int i = 0; i < data.size(); i += 50)
	{
		xGridList.push_back(i);
		xGrid.push_back(graphicsScene->addLine(
			i, 0, i, data_max,
			QPen(QColor(200, 200, 200))));
	}

// y-axis
	for (int i = 0; i < (int)yGrid.size(); i ++)
	{
		graphicsScene->removeItem(yGrid[i]);
		delete yGrid[i];
	}
	yGrid.clear();

	yGridList.clear();
	for (int i = 0; i < data_max; i += 1)
	{
		yGridList.push_back(i);
		yGrid.push_back(graphicsScene->addLine(
			0, i, data.size() - 1, i,
			QPen(QColor(200, 200, 200))));
	}

	update();
}

void TimePlot::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

	painter.setPen(Qt::red);

	for (int i = 0; i < (int)xGridList.size(); i ++)
	{
		qreal vx = xGridList[i];
		int x = graphicsView->mapFromScene(vx / 1.5, 0).x() + leftMargin - 6;

		painter.drawText(
			QRectF(x, height() - bottomMargin, x, height() - bottomMargin),
			Qt::AlignTop | Qt::AlignHCenter | Qt::TextDontClip,
			QString("%1").arg(vx));
	}

	for (int i = 0; i < (int)yGridList.size(); i ++)
	{
		qreal vy = yGridList[i];
		int y = graphicsView->mapFromScene(0, vy / 1.5).y() + topMargin;
		printf("y = %d\n", y);

		painter.drawText(
			QRectF(0, y, leftMargin, y),
			Qt::AlignRight | Qt::AlignVCenter | Qt::TextDontClip,
			QString("%1qq").arg(vy));
	}
}

