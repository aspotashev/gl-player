#include <QDockWidget>
#include <QTextEdit>

class CommentDock : public QDockWidget
{
public:
	CommentDock(QWidget *parent = NULL);

	void setText(QString s);

private:
	QTextEdit *widget;
};

