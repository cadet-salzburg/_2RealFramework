#ifndef QGLTEXTUREIMAGE_H
#define QGLTEXTUREIMAGE_H

#include <qgl.h>
#include "qmutex.h"

class QGlTextureImage : public QGLWidget
{
	Q_OBJECT

public:
	QGlTextureImage(QMutex* mutex, QWidget *parent=0);
	~QGlTextureImage();

	void updateTexture(int w, int h, int channels, unsigned char* pTexture);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
	

private:
	GLuint	m_iTexture;
	bool	m_bIsTextureGenerated;
	QMutex*	m_Mutex;
};

#endif // QGLTEXTUREIMAGE_H
