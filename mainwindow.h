#include <QMainWindow>
#include "glwidget.h"

class SimulationThread;
class VisFrame;
class FileFormat;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	virtual void keyPressEvent(QKeyEvent * event);
	virtual void closeEvent(QCloseEvent *event);

	void loadScene(VisFrame *f);
	void rotateY(double dy);
	void loadFrame(FileFormat *f, int index);
public slots:
	void slotStart();
	void slotFileOpen();
private:
	GLWidget *glWidget;
	QToolBar *mainToolbar;
	SimulationThread *thread;

	QAction *actionStart;
	QAction *actionFileOpen;

	QString fileName;
	FileFormat *fileCutter;
};

