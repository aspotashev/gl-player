#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
{
}

GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
	qglClearColor(QColor::fromCmykF(0.39, 0.39, 0.0, 0.0).dark());
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLUquadricObj *quad = gluNewQuadric();

	glLoadIdentity();
	glRotatef(50.0f, 1.0f, 1.0f, 1.0f);
	glScalef(0.5f, 1.0f, 1.0f);
	gluSphere(quad, 1.0, 100, 100);
}

void GLWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);
//	glMatrixMode(GL_MODELVIEW);
}

