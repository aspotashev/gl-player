#include <QtGui>

class TimePlot : public QWidget
{
public:
	TimePlot(QWidget *parent = NULL);
	~TimePlot();

	void loadData(std::vector<float> data);
	void clear();

protected:
	virtual void resizeEvent(QResizeEvent *event);

	virtual void mousePressEvent(QMouseEvent *event)
	{
		printf("mmm\n");
	}

private:
	void resizeGraphicsViewToFit();

private:
	QGraphicsScene *graphicsScene;
	QGraphicsView *graphicsView;

	std::vector<float> data;
	float data_max;
};

