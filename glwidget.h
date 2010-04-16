#include <QGLWidget>

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget(QWidget *parent = NULL);
	~GLWidget();

	void rotateY(double dy);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

private:
	double xRot, yRot, zRot;
};

