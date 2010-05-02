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
	virtual void closeEvent(QCloseEvent *event);

	void loadScene(VisFrame *f);
	void rotateY(double dy);
public slots:
	void start();
private:
	GLWidget *glWidget;
	QToolBar *mainToolbar;
	SimulationThread *thread;

	QAction *actionStart;
};

