#include <QThread>
#include <QSemaphore>

class MainWindow;

class SimulationThread : public QThread
{
public:
	SimulationThread(MainWindow *mw);
	virtual void run();
	void enableSim();
private:
	MainWindow *mw;
	QSemaphore sim_lock; // simulation enabled if semaphore = 1
};

