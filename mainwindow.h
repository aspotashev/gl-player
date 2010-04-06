#include <QMainWindow>
#include "glwidget.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

private:
	GLWidget *glWidget;
};

