#ifndef QGLTEXTUREIMAGE_H
#define QGLTEXTUREIMAGE_H

#include <qgl.h>

class QGlTextureImage : public QGLWidget
{
	Q_OBJECT

public:
	QGlTextureImage( QWidget *parent=0);
	~QGlTextureImage();

	void updateTexture(int w, int h, int channels, int bitsPerPixel, void* pTexture);

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
	

private:
	GLuint	m_iTexture;
	int		m_iWidth;
	int		m_iHeight;
	int		m_iChannels;
	int		m_iBitsPerPixel;
	bool	m_bIsTextureGenerated;
};

#endif // QGLTEXTUREIMAGE_H
