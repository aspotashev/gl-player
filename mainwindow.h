#include <QMainWindow>
#include "glwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	virtual void keyPressEvent(QKeyEvent * event);
private:
	GLWidget *glWidget;
};

