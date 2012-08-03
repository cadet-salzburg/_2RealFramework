#ifndef QGLSKELETONWIDGET_H
#define QGLSKELETONWIDGET_H

#include <qgl.h>
#include "qglviewer.h"
#include "_2RealDatatypes.h"

class QGlSkeletonWidget : public QGLViewer
{
	Q_OBJECT

public:
	QGlSkeletonWidget(QWidget *parent=0);
	QGlSkeletonWidget(_2Real::Vec3 minSceneLimit, _2Real::Vec3 maxSceneLimit, QWidget *parent=0);
	~QGlSkeletonWidget();

	void updateSkeleton( const _2Real::Skeleton& skeleton  );
	void updateSkeletons( const std::vector<_2Real::Skeleton>& skeletons );
	void setSceneLimis(_2Real::Vec3 minSceneLimit, _2Real::Vec3 maxSceneLimit);
protected:

	virtual void init();
	virtual void draw();

private:
	std::vector<_2Real::Skeleton>	m_Skeletons;
	_2Real::Vec3					m_MinSceneLimit;
	_2Real::Vec3					m_MaxSceneLimit;
	bool							m_bIsGlobal;
};

#endif // QGLSKELETONWIDGET_H
