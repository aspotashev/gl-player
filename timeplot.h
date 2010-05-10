#include <QtGui>

class TimePlot : public QDockWidget
{
	Q_OBJECT

public:
	TimePlot(QWidget *parent = NULL);
	~TimePlot();

	void loadData(std::vector<float> data);
	void clear();

public slots:
	void moveCurrentMark(int val);

protected:
	virtual void resizeEvent(QResizeEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);

private:
	void resizeGraphicsViewToFit();

private:
	QGraphicsScene *graphicsScene;
	QGraphicsView *graphicsView;

	QGraphicsRectItem *currentMark;
	int currentMarkPos;

	std::vector<float> data;
	float data_max;
};
