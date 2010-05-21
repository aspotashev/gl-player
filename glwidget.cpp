#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <algorithm>
#include <set>
#include <QtGui>
#include "glwidget.h"
#include "visframe.h"
#include "mainwindow.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

GLWidget::GLWidget(QWidget *parent):
	QGLWidget(parent)
{
	mainWindow = (MainWindow *)parent;

	enableCalllist = false;
	callListUptodate = false;

	xRot = yRot = zRot = 0.0;
	xTrans = yTrans = 0.0;
	zTrans = -15.0;
	scene = NULL;
	initialScene = NULL;
	visibleBrokenEdges = false;

	setAutoFillBackground(false);

	connect(this, SIGNAL(needsUpdate()),
		this, SLOT(updateGL()));
	connect(this, SIGNAL(needsUpdate()),
		this, SLOT(update()));
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
	qglClearColor(QColor::fromCmykF(0.39, 0.39, 0.0, 0.0).dark());
	glClearColor(0.3, 0.3, 0.5, 0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
//	glEnable(GL_LIGHTING);
//	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_DIFFUSE, 0.5);
}

void GLWidget::paintBrokenEdges()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	if (initialScene && scene)
	{
		std::vector<Edge> a0 = initialScene->edges();
		std::vector<Edge> b0 = scene->edges();
		std::set<Edge> a;
		std::set<Edge> b;

//		std::copy(a0.begin(), a0.end(), a.begin());
		for (int i = 0; i < (int)a0.size(); i ++)
		{
			a.insert(Edge(a0[i].a, a0[i].b));
		}

//		std::copy(b0.begin(), b0.end(), b.begin());
		for (int i = 0; i < (int)b0.size(); i ++)
		{
			b.insert(Edge(b0[i].a, b0[i].b));
		}

		std::vector<Edge> brokenEdges;
		for (std::set<Edge>::iterator i = a.begin();
			i != a.end();
			i ++)
		{
			if (b.find(*i) == b.end())
			{
				brokenEdges.push_back(*i);
			}
		}

		for (int i = 0; i < (int)brokenEdges.size(); i ++)
		{
			Edge e = brokenEdges[i];

			assert(e.a >= 0 && e.a < scene->nVertices());
			assert(e.b >= 0 && e.b < scene->nVertices());

			glVertex3f(scene->vertex(e.a).x, scene->vertex(e.a).y, scene->vertex(e.a).z);
			glVertex3f(scene->vertex(e.b).x, scene->vertex(e.b).y, scene->vertex(e.b).z);
		}
	}
	glEnd();
}

void GLWidget::generateCallList()
{
	if (enableCalllist)
	{
		glNewList(1, GL_COMPILE_AND_EXECUTE);
	}


	glColor3f(1, 1, 1);

	GLUquadricObj *quad = gluNewQuadric();


	glPushMatrix();

	VisFrame *s = scene;
	for (int i = 0; i < (s ? s->nVertices() : 0); i ++)
	{
		Vertex v = s->vertex(i);

		glPushMatrix();

		glTranslatef(v.x - 4.5, v.y - 4.5, v.z - 4.5);
		gluSphere(quad, v.r, 5, 5);

		glPopMatrix();
	}


	glPopMatrix();


	glTranslatef(-4.5, -4.5, -4.5);

	// draw existing edges
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_LINES);
	for (int i = 0; i < (s ? s->nEdges() : 0); i ++)
	{
		Edge e = s->edge(i);

		assert(e.a >= 0 && e.a < s->nVertices());
		assert(e.b >= 0 && e.b < s->nVertices());

		glVertex3f(s->vertex(e.a).x, s->vertex(e.a).y, s->vertex(e.a).z);
		glVertex3f(s->vertex(e.b).x, s->vertex(e.b).y, s->vertex(e.b).z);
	}
	glEnd();

	if (visibleBrokenEdges)
	{
		paintBrokenEdges();
	}

	glColor3f(0.2, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f( 9.5f, -0.5f, 0.0f);
	glVertex3f( 9.5f,  9.5f, 0.0f);
	glVertex3f(-0.5f,  9.5f, 0.0f);
	glEnd();


	if (enableCalllist)
	{
		glEndList();
	}
}

void GLWidget::paintGL()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	resizeGL(width(), height());
//	setupViewport(width(), height());


	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glLoadIdentity();
	glTranslated(xTrans, yTrans, zTrans);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

	if (!callListUptodate || !enableCalllist)
	{
		generateCallList();
		callListUptodate = true;
	}
	else
	{
		glCallList(1);
	}


//	glLoadIdentity();
//	glTranslated(0.0, 0.0, -5.0);

//	glLineWidth(2.0);
//	glRectd(-2.0, -2.0, 2.0, 2.0);
//	glutWireCube(1.0);




	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glPopMatrix();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
	return; // disable the crappiest renderer

	clock_t start_time = clock();

	paintGL();

	QPainter painter(this);
	QColor panelBg(0, 0, 0, 100);
	QSize panelSize(120, 50);

	painter.setBrush(QBrush(panelBg));
	painter.setPen(QPen(panelBg));
	painter.drawRoundedRect(5, 5, panelSize.width(), panelSize.height(), 5, 5);

	painter.setPen(QPen(QColor(255, 0, 0)));
	painter.setFont(QFont("sans-serif", 16));
	painter.drawText(5, 5, 125, 55, Qt::AlignCenter | Qt::TextWordWrap, QString("Frame: %1").arg(mainWindow->frameIndex()));
	painter.end();


	int ticks = (int)(clock() - start_time);
//	printf("paintGL took %lf seconds\n", ticks, (double)ticks / CLOCKS_PER_SEC);
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60, (double)width/(double)height, 1.0, 300.0);


//	glOrtho(-1.5, +1.5, +1.5, -1.5, 0.0, 15.0);

	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::rotateY(double dy)
{
	yRot += dy;
	emit needsUpdate();
}

void GLWidget::translateXY(double dx, double dy)
{
	xTrans += dx;
	yTrans += dy;
	emit needsUpdate();
}

void GLWidget::setVisFrame(VisFrame *f)
{
	if (!initialScene)
	{
		initialScene = f;
	}

	scene = f;

	callListUptodate = false;
	emit needsUpdate();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		clickPointL = event->pos();
	}

	if (event->buttons() == Qt::RightButton)
	{
		clickPointR = event->pos();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		rotateY(2.5*(event->x() - clickPointL.x()));
		clickPointL = event->pos();
	}

	if (event->buttons() == Qt::RightButton)
	{
		translateXY(
			0.01 *(event->x() - clickPointR.x()),
			-0.01*(event->y() - clickPointR.y()));
		clickPointR = event->pos();
	}
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	zTrans += event->delta()/100;
	emit needsUpdate();
}

void GLWidget::setVisibleBrokenEdges(bool visible)
{
	visibleBrokenEdges = visible;
	callListUptodate = false;
	emit needsUpdate();
}

void GLWidget::showEvent(QShowEvent *event)
{
}

