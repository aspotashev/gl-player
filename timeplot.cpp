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

	leftMargin = 10;
	topMargin = 0;
	rightMargin = 0;
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
	for (int i = 0; i < (int)xGrid.size(); i ++)
	{
		graphicsScene->removeItem(xGrid[i]);
		delete xGrid[i];
	}
	xGrid.clear();

	for (int i = 0; i < data.size(); i += 10)
	{
		xGrid.push_back(graphicsScene->addLine(
			i, 0, i, data_max,
			QPen(QColor(200, 200, 200))));
	}
}

void TimePlot::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);

//	painter.setBrush(Qt::blue);
//	painter.drawRect(-1000, -1000, 2000, 2000);

	painter.setPen(Qt::red);

	int xx = graphicsView->mapFromScene(0, 0).x();
	printf("x = %d\n", xx);

	painter.drawLine(
		leftMargin - 3 + xx, height() - bottomMargin,
		leftMargin - 3 + xx, height() - bottomMargin + 10);
}

