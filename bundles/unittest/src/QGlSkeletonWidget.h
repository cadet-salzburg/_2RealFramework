#ifndef QGLSKELETONWIDGET_H
#define QGLSKELETONWIDGET_H

#include <qgl.h>
#include "_2RealDatatypes.h"

class QGlSkeletonWidget : public QGLWidget
{
	Q_OBJECT

public:
	QGlSkeletonWidget( QWidget *parent=0);
	~QGlSkeletonWidget();

	void updateSkeleton( const _2Real::Skeleton& skeleton  );
	void updateSkeletons( const std::vector<_2Real::Skeleton>& skeletons );

protected:

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

private:
	std::vector<_2Real::Skeleton>	m_Skeletons;
};

#endif // QGLSKELETONWIDGET_H
