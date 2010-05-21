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
	rightMargin = 15;
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

	graphicsView->translate(0, data_max);
	graphicsView->scale(1, -1);

	hideCurrentMark(); // remove (but not delete) the mark
	graphicsScene->clear(); // remove & delete all items from the scene
	for (int i = 0; i < (int)data.size() - 1; i ++)
	{
		graphicsScene->addLine(
			i,     data[i],
			i + 1, data[i + 1]); // hack, we should rather
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

void TimePlot::buildMarksList(qreal begin, qreal end, int nMax, std::vector<qreal> &res)
{
	printf("nMax = %d\n", nMax);

	qreal deltaMin = (end - begin) / nMax;

	int power10 = (int)log10(deltaMin * 1000000.0) - 6; // hack!
	qreal mantissa = deltaMin / pow10(power10);

	if (mantissa < 2.0)
	{
		mantissa = 2.0;
	}
	else if (mantissa < 5.0)
	{
		mantissa = 5.0;
	}
	else
	{
		mantissa = 10.0;
	}

	qreal delta = mantissa * pow10(power10);

	int beginInt = (int)ceil(begin / delta);
	int endInt = (int)floor(end / delta);

	for (int i = beginInt; i <= endInt; i ++)
	{
		res.push_back(i * delta);
	}
}

void TimePlot::configureGrid()
{
	if (data.size() == 0)
	{
		return;
	}

// x-axis
	for (int i = 0; i < (int)xGrid.size(); i ++)
	{
		graphicsScene->removeItem(xGrid[i]);
		delete xGrid[i];
	}
	xGrid.clear();

	xGridList.clear();
	buildMarksList(
		0, data.size(),
		(width() - leftMargin - rightMargin) / 50,
		xGridList);
	for (int i = 0; i < xGridList.size(); i ++)
	{
		qreal x = xGridList[i];
		xGrid.push_back(graphicsScene->addLine(
			x, 0, x, data_max,
			QPen(QColor(100, 100, 100, 100))));
	}

// y-axis
	for (int i = 0; i < (int)yGrid.size(); i ++)
	{
		graphicsScene->removeItem(yGrid[i]);
		delete yGrid[i];
	}
	yGrid.clear();

	yGridList.clear();
	buildMarksList(
		0, data_max,
		(height() - topMargin - bottomMargin) / 20,
		yGridList);
	for (int i = 0; i < yGridList.size(); i ++)
	{
		qreal y = yGridList[i];
		yGrid.push_back(graphicsScene->addLine(
			0, y, data.size() - 1, y,
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
		int x = graphicsView->mapFromScene(vx / 1.5, 0).x() + leftMargin - 16;

		painter.drawText(
			QRectF(x, height() - bottomMargin, x, height() - bottomMargin),
			Qt::AlignTop | Qt::AlignHCenter | Qt::TextDontClip,
			QString("%1").arg(vx));
	}

	for (int i = 0; i < (int)yGridList.size(); i ++)
	{
		qreal vy = yGridList[i];
		int y = graphicsView->mapFromScene(0, data_max - (data_max - vy) / 1.5).y() + topMargin - 3;

		painter.drawText(
			QRectF(0, y, leftMargin - 3, y),
			Qt::AlignRight | Qt::AlignVCenter | Qt::TextDontClip,
			QString("%1").arg(vy));
	}
}

void TimePlot::setZeroFrame(int i)
{
	zeroFrame = i;
}

void TimePlot::setEpsilonCoeff(float x)
{
	epsilonCoeff = x;
}

