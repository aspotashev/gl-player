#include <QMainWindow>
#include <QTimer>
#include "glwidget.h"

class VisFrame;
class FileFormat;
class PlaybackSlider;
class TimePlot;
class CommentDock;

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
	void slotPlaybackStart();
	void slotPlaybackPause();
	void slotPlaybackNextFrame();
private:
	bool isLastFrame() const;
	void enablePlaybackButtons(bool enable);
	bool isPlaybackButtonVisible();
	void enablePlaybackSlider(bool enable);
	bool isPlaybackSliderVisible();



	GLWidget *glWidget;
	QToolBar *mainToolbar;
	QToolBar *playbackToolbar;

	QAction *actionFileOpen;
	QAction *actionVisibleBrokenEdges;
	QAction *actionPlaybackStart;
	QAction *actionPlaybackPause;

	QString fileName;
	FileFormat *fileCutter;

	int currentFrameIndex;

	PlaybackSlider *playbackSlider;
	QAction *playbackSliderAction;
	TimePlot *timePlot;

	QTimer playbackTimer;

	int zeroFrame;
	QString fileComment;

	CommentDock *commentDock;
};

