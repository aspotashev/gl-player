#include <QVBoxLayout>
#include "commentdock.h"

CommentDock::CommentDock(QWidget *parent):
	QDockWidget(parent)
{
	widget = new QLabel(this);
	widget->show();

	setTitleBarWidget(new QWidget(this));

//	widget->setReadOnly(true);
}

void CommentDock::setText(QString s)
{
	widget->setText(s);
}

QSize CommentDock::sizeHint() const
{
	return QSize(130, 100);
}

