#include "QGlTextureImage.h"

QGlTextureImage::QGlTextureImage( QMutex* mutex, QWidget *parent)
	:  QGLWidget(QGLFormat(QGL::SampleBuffers), parent), m_Mutex(mutex), m_bIsTextureGenerated(false)
{
}

QGlTextureImage::~QGlTextureImage()
{
}

void QGlTextureImage::initializeGL() 
{
	//QMutexLocker locker(m_Mutex);
	makeCurrent();
	glClearColor (0.0,0.0,0.0,1.0);
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();        
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();        
}

void QGlTextureImage::resizeGL(int w, int h)
{
	QMutexLocker locker(m_Mutex);
	makeCurrent();
	glViewport( 0, 0, w, h );
}

void QGlTextureImage::paintGL()
{
//	QMutexLocker locker(m_Mutex);
	makeCurrent();
	glClear (GL_COLOR_BUFFER_BIT);       
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();    

	glOrtho(0,1,0,1,0,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();        
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D,m_iTexture); 
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);   
		glTexCoord2i(0,1); glVertex2i(0,0);
		glTexCoord2i(0,0); glVertex2i(0,1);
		glTexCoord2i(1,0); glVertex2i(1,1);
		glTexCoord2i(1,1); glVertex2i(1,0);
	glEnd();         
	glFlush();
}

void QGlTextureImage::updateTexture(int w, int h, int channels, unsigned char* pTexture)
{
//	m_Mutex->lock();
	makeCurrent();
	int iTextureFormat;

	if(channels==1)
	{
		iTextureFormat = GL_LUMINANCE;
	}
	else if(channels==3)
	{
		iTextureFormat = GL_RGB;
	}
	else if(channels==4)
	{
		iTextureFormat = GL_RGBA;
	}

	glEnable(GL_TEXTURE_2D);
	if(!m_bIsTextureGenerated)
	{
		glGenTextures(1, &m_iTexture);
		glBindTexture(GL_TEXTURE_2D,m_iTexture);       
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   
		glTexImage2D(GL_TEXTURE_2D, 0, iTextureFormat, w, h, 0, iTextureFormat, GL_UNSIGNED_BYTE, NULL );    
		m_bIsTextureGenerated = true;
	}
	// update texture
	glBindTexture(GL_TEXTURE_2D,m_iTexture); 
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, iTextureFormat, GL_UNSIGNED_BYTE, pTexture);    

	glDisable(GL_TEXTURE_2D);
//	m_Mutex->unlock();
	repaint();
}