#include "QGlSkeletonWidget.h"

QGlSkeletonWidget::QGlSkeletonWidget( QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) 
{
}

QGlSkeletonWidget::~QGlSkeletonWidget()
{
}

void QGlSkeletonWidget::initializeGL() 
{
	makeCurrent();
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, 1, 1, 0, 0, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glColor4f( 1, 1, 1, 1 );
}

void QGlSkeletonWidget::resizeGL(int w, int h)
{
	makeCurrent();
	glViewport( 0, 0, w, h );
}

void QGlSkeletonWidget::paintGL()
{
	makeCurrent();

	glClear( GL_COLOR_BUFFER_BIT );
	glPointSize(5);
	std::vector<_2Real::Point> joints = m_Skeleton.getJointPositions();
	glColor4f(1,1,1,1);
	for(int i=0; i<joints.size(); i++)
	{
		double x = double(joints[i].x()) / 640.0; 
		double y = double(joints[i].y()) / 480.0; 

		glBegin(GL_POINTS);
			glVertex2f(x,y);
		glEnd();
	}

	glFlush();
}

void QGlSkeletonWidget::updateSkeleton( const _2Real::Skeleton& skeleton  )
{
	m_Skeleton = skeleton;
	update();
}