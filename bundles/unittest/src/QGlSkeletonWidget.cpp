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

	for(int i=0; i<m_Skeletons.size(); i++)
	{
		std::vector<_2Real::RigidBody> rigidBodies = m_Skeletons[i].getRigidBodies();
		for(int j=0; j<rigidBodies.size(); j++)
		{
			std::vector<_2Real::Point> joints = rigidBodies[j].getPoints();
			for(int k=0; k<joints.size(); k++)
			{
				double x = double(joints[k].x()) / 640.0; 
				double y = double(joints[k].y()) / 480.0; 

				glBegin(GL_POINTS);
					glVertex2f(x,y);
				glEnd();
			}
		}
	}
	glFlush();
}

void QGlSkeletonWidget::updateSkeleton( const _2Real::Skeleton& skeleton  )
{
	m_Skeletons.clear();
	m_Skeletons.push_back(skeleton);
	update();
}

void QGlSkeletonWidget::updateSkeletons( const std::vector<_2Real::Skeleton>& skeletons  )
{
	m_Skeletons.clear();
	m_Skeletons = skeletons;
	update();
}