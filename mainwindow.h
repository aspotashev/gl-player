#include <QMainWindow>
#include "glwidget.h"

class VisFrame;
class FileFormat;
class PlaybackSlider;
class TimePlot;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow();

	virtual void keyPressEvent(QKeyEvent * event);
	virtual void closeEvent(QCloseEvent *event);

	void loadScene(VisFrame *f);
	void rotateY(double dy);
	void loadFrame(int index);
	int frameIndex() const;
	void loadNextFrame();
	void openFile(const QString &fn);
	void closeFile();
public slots:
	void slotFileOpen();
	void slotGotoFrame(int index);
private:
	GLWidget *glWidget;
	QToolBar *mainToolbar;
	QToolBar *playbackToolbar;

	QAction *actionFileOpen;
	QAction *actionVisibleBrokenEdges;

	QString fileName;
	FileFormat *fileCutter;

	int currentFrameIndex;

	PlaybackSlider *playbackSlider;
	TimePlot *timePlot;
};

