#include <QGLWidget>

class VisFrame;

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget(QWidget *parent = NULL);
	~GLWidget();

	void rotateY(double dy);
	void translateXY(double dx, double dy);

	void setVisFrame(VisFrame *f);

public slots:
	void setVisibleBrokenEdges(bool visible);

protected:
	void initializeGL();
	void paintBrokenEdges();
	void paintGL();
	void resizeGL(int width, int height);

	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

private:
	void generateCallList();

signals:
	void needsUpdate();

private:
	double xRot, yRot, zRot;
	double xTrans, yTrans, zTrans;
	VisFrame *scene;
	VisFrame *initialScene;

	QPoint clickPointL; // left button
	QPoint clickPointR; // right button

	bool visibleBrokenEdges;
	bool enableCalllist;
	bool callListUptodate;
};

