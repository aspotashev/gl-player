#include <assert.h>
#include <stdio.h>
#include <algorithm>
#include <set>
#include <QtGui>
#include "glwidget.h"
#include "visframe.h"

GLWidget::GLWidget(QWidget *parent)
{
	xRot = yRot = zRot = 0.0;
	zTrans = 0.0;
	scene = NULL;
	initialScene = NULL;

	connect(this, SIGNAL(needsUpdate()),
		this, SLOT(updateGL()));
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

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1, 1, 1);

	GLUquadricObj *quad = gluNewQuadric();


	VisFrame *s = scene;
	for (int i = 0; i < (s ? s->nVertices() : 0); i ++)
	{
		Vertex v = s->vertex(i);

		glLoadIdentity();

		glTranslated(0.0, 0.0, -15.0 + zTrans);
		glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
		glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
		glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

		glTranslatef(v.x - 4.5, v.y - 4.5, v.z - 4.5);

		gluSphere(quad, v.r, 5, 5);
	}


	glLoadIdentity();

	glTranslated(0.0, 0.0, -15.0 + zTrans);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);

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

			assert(e.a >= 0 && e.a < s->nVertices());
			assert(e.b >= 0 && e.b < s->nVertices());

			glVertex3f(s->vertex(e.a).x, s->vertex(e.a).y, s->vertex(e.a).z);
			glVertex3f(s->vertex(e.b).x, s->vertex(e.b).y, s->vertex(e.b).z);
		}
	}
	glEnd();


	glColor3f(0.2, 0.5, 0.5);
	glBegin(GL_POLYGON);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f( 9.5f, -0.5f, 0.0f);
	glVertex3f( 9.5f,  9.5f, 0.0f);
	glVertex3f(-0.5f,  9.5f, 0.0f);
	glEnd();


	glLoadIdentity();
	glTranslated(0.0, 0.0, -5.0);

//	glLineWidth(2.0);
//	glRectd(-2.0, -2.0, 2.0, 2.0);
//	glutWireCube(1.0);
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);


	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60, (double)width/(double)height, 1.0, 100.0);


//	glOrtho(-1.5, +1.5, +1.5, -1.5, 0.0, 15.0);

	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::rotateY(double dy)
{
	yRot += dy;
	emit needsUpdate();
//	updateGL();
}

void GLWidget::setVisFrame(VisFrame *f)
{
	if (!initialScene)
	{
		initialScene = f;
	}

	scene = f;

//	updateScene();
	emit needsUpdate();
}

void GLWidget::updateScene()
{
	// rebuild call lists here
	updateGL();

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		clickPoint = event->pos();
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton)
	{
		rotateY(2.5*(event->x() - clickPoint.x()));
		clickPoint = event->pos();
	}
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	zTrans += event->delta()/100;
	emit needsUpdate();
}

