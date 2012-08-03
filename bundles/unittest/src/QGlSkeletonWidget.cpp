#include "QGlSkeletonWidget.h"

QGlSkeletonWidget::QGlSkeletonWidget( QWidget *parent) : m_MinSceneLimit(-1), m_MaxSceneLimit(1), m_bIsGlobal(false), QGLViewer(parent) 
{
}


QGlSkeletonWidget::QGlSkeletonWidget(_2Real::Vec3 minSceneLimit, _2Real::Vec3 maxSceneLimit, QWidget *parent) : m_MinSceneLimit(minSceneLimit), m_MaxSceneLimit(maxSceneLimit), m_bIsGlobal(false), QGLViewer(parent)
{
}

QGlSkeletonWidget::~QGlSkeletonWidget()
{
}

void QGlSkeletonWidget::init()
{
	setSceneBoundingBox(qglviewer::Vec(m_MinSceneLimit[0], m_MinSceneLimit[1], m_MinSceneLimit[2]), qglviewer::Vec(m_MaxSceneLimit[0], m_MaxSceneLimit[1], m_MaxSceneLimit[2]));
	camera()->showEntireScene();
	setAxisIsDrawn(true);
}

void QGlSkeletonWidget::draw()
{
	makeCurrent();

	glClear( GL_COLOR_BUFFER_BIT );
	glColor4f(1,0,0,1);
	glPointSize(5);
	glDisable(GL_LIGHTING);

	for(int i=0; i<m_Skeletons.size(); i++)
	{
		std::vector<_2Real::RigidBody> rigidBodies = m_Skeletons[i].getRigidBodies();
		for(int j=0; j<rigidBodies.size(); j++)
		{
			std::vector<_2Real::Point> joints = rigidBodies[j].getPoints();
			for(int k=0; k<joints.size(); k++)
			{
				double x = -1 + 2 * (double(joints[k].x())); 
				double y = -1 + 2 * (double(joints[k].y())); 
				double z = double(joints[k].z());

				glBegin(GL_POINTS);
					glVertex3f(x,y,z);
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


void QGlSkeletonWidget::setSceneLimis(_2Real::Vec3 minSceneLimit, _2Real::Vec3 maxSceneLimit)
{
	m_MinSceneLimit = minSceneLimit;
	m_MaxSceneLimit = maxSceneLimit;
	setSceneBoundingBox(qglviewer::Vec(m_MinSceneLimit[0], m_MinSceneLimit[1], m_MinSceneLimit[2]), qglviewer::Vec(m_MaxSceneLimit[0], m_MaxSceneLimit[1], m_MaxSceneLimit[2]));
	camera()->showEntireScene();
}
