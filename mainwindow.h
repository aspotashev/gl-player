#include <QMainWindow>
#include "glwidget.h"

class SimulationThread;
class VisFrame;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	virtual void keyPressEvent(QKeyEvent * event);

	void loadScene(VisFrame *f);
public slots:
	void start();
private:
	GLWidget *glWidget;
	QToolBar *mainToolbar;
	QAction *actionStart;
	SimulationThread *thread;
};

