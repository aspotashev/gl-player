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
	virtual QSize sizeHint() const;
	virtual void paintEvent(QPaintEvent *event);

private:
	void resizeGraphicsViewToFit();
	void hideCurrentMark();

private:
	QGraphicsScene *graphicsScene;
	QGraphicsView *graphicsView;

	QGraphicsRectItem *currentMark;
	int currentMarkPos;
	bool currentMarkVisible;

	std::vector<float> data;
	float data_max;

	int leftMargin;
	int topMargin;
	int rightMargin;
	int bottomMargin;
};

