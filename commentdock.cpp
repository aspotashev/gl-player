#include <QVBoxLayout>
#include "commentdock.h"

CommentDock::CommentDock(QWidget *parent):
	QDockWidget(parent)
{
	widget = new QLabel(this);
	widget->show();

//	setTitleBarWidget(new QWidget(this));

	widget->setWordWrap(true);
	widget->setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

void CommentDock::setText(QString s)
{
	widget->setText(s);
}

QSize CommentDock::sizeHint() const
{
	return QSize(130, 100);
}

void CommentDock::resizeEvent(QResizeEvent *event)
{
	widget->move(2, 20);
	widget->resize(width() - 4, height() - 20);
}

