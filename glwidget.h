#include <QGLWidget>

class VisFrame;

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget(QWidget *parent = NULL);
	~GLWidget();

	void rotateY(double dy);

	void setVisFrame(VisFrame *f);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void updateScene();

signals:
	void needsUpdate();

private:
	double xRot, yRot, zRot;
	VisFrame *scene;
};

