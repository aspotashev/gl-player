#include <QVBoxLayout>
#include "commentdock.h"

CommentDock::CommentDock(QWidget *parent):
	QDockWidget(parent)
{
	widget = new QTextEdit(this);
	widget->show();

	widget->setReadOnly(true);
}

void CommentDock::setText(QString s)
{
	widget->setPlainText(s);
}

