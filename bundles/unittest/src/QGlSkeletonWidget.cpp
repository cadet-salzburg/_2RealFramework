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
	glColor4f(1,1,1,1);
	glPointSize(5);

	std::vector<_2Real::RigidBody> rigidBodies = m_Skeleton.getRigidBodies();
	for(int i=0; i<rigidBodies.size(); i++)
	{
		std::vector<_2Real::Point> joints = rigidBodies[i].getPoints();
		for(int j=0; j<joints.size(); j++)
		{
			double x = double(joints[j].x()) / 640.0; 
			double y = double(joints[j].y()) / 480.0; 

			glBegin(GL_POINTS);
				glVertex2f(x,y);
			glEnd();
		}
	}
	glFlush();
}

void QGlSkeletonWidget::updateSkeleton( const _2Real::Skeleton& skeleton  )
{
	m_Skeleton = skeleton;
	update();
}