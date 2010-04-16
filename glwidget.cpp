#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent)
{
	xRot = yRot = zRot = 0.0;
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
	glEnable(GL_COLOR_MATERIAL);
	glMaterialf(GL_FRONT, GL_DIFFUSE, 0.5);
}

void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GLUquadricObj *quad = gluNewQuadric();


//-------------------
	glLoadIdentity();

	glTranslated(0.0, 0.0, -10.0);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	glTranslated(3, 0.0, 0.0);

	glRotatef(50.0f, 1.0f, 1.0f, 1.0f);
	glScalef(0.5f, 1.0f, 1.0f);
	gluSphere(quad, 1.0, 100, 100);
//-------------------
	glLoadIdentity();

	glTranslated(0.0, 0.0, -10.0);
	glRotated(xRot / 16.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 16.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 16.0, 0.0, 0.0, 1.0);
	glTranslated(-0.9, 0.0, 0.0);

	glRotatef(-20.0f, -1.0f, 1.0f, 1.0f);
	glScalef(0.5f, 1.0f, 1.0f);
	gluSphere(quad, 1.0, 100, 100);
//-------------------


	glLoadIdentity();
	glTranslated(0.0, 0.0, -5.0);

//	glLineWidth(2.0);
//	glRectd(-2.0, -2.0, 2.0, 2.0);
//	glutWireCube(1.0);
}

void GLWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);




	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();
	gluPerspective(60, 1.0, 1.0, 100.0);

//	glOrtho(-1.5, +1.5, +1.5, -1.5, 0.0, 15.0);

	glMatrixMode(GL_MODELVIEW);
}

void GLWidget::rotateY(double dy)
{
	yRot += dy;
	updateGL();
}

