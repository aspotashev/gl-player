#include <QGLWidget>

class GLWidget : public QGLWidget
{
	Q_OBJECT
public:
	GLWidget(QWidget *parent = NULL);
	~GLWidget();
};

