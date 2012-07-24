#include "QGlTextureImage.h"

QGlTextureImage::QGlTextureImage( QWidget *parent)
	:  QGLWidget(QGLFormat(QGL::SampleBuffers), parent), m_bIsTextureGenerated(false)
{
}

QGlTextureImage::~QGlTextureImage()
{
}

void QGlTextureImage::initializeGL() 
{
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
	makeCurrent();
	glViewport( 0, 0, w, h );
}

void QGlTextureImage::paintGL()
{
	makeCurrent();						// this is important for qt so things happen in the correct threaded opengl context
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

void QGlTextureImage::updateTexture(int w, int h, int channels, int bitsPerPixel, void* pTexture)
{
	int iInternalTextureFormat;
	int iTextureFormat;
	int iTextureType;

	// this is important for qt so things happen in the correct threaded opengl context
	makeCurrent();

	if(w>0 && h>0 && channels>0 && (bitsPerPixel==8 || bitsPerPixel==16))
	{
		glEnable(GL_TEXTURE_2D);

		if(m_iWidth != w || m_iHeight != h || m_iChannels != channels || m_iBitsPerPixel != bitsPerPixel)		// settings changes so we need to allocate a new texture
		{
			if(m_bIsTextureGenerated)
			{
				glDeleteTextures(1, &m_iTexture);		// delete old texture and try to generate new due to boolean flag
				m_bIsTextureGenerated = false;
			}
		}
		// get opengl texture format
		if(channels==1)
		{
			iTextureFormat = GL_LUMINANCE;
			if(bitsPerPixel == 8)
			{
				iInternalTextureFormat = GL_LUMINANCE8;
			}
			else if(bitsPerPixel == 16)
			{
				iInternalTextureFormat = GL_LUMINANCE16;
			}
		}
		else if(channels==3)
		{
			iTextureFormat = GL_RGB;
			if(bitsPerPixel == 8)
			{
				iInternalTextureFormat = GL_RGB8;
			}
			else if(bitsPerPixel == 16)
			{
				iInternalTextureFormat = GL_RGB16;
			}
		}
		else if(channels==4)
		{
			iTextureFormat = GL_RGBA;
			if(bitsPerPixel == 8)
			{
				iInternalTextureFormat = GL_RGBA8;
			}
			else if(bitsPerPixel == 16)
			{
				iInternalTextureFormat = GL_RGBA16;
			}
		}
		// set data type
		if(bitsPerPixel == 8)
		{
			iTextureType = GL_UNSIGNED_BYTE;
		}
		else if(bitsPerPixel == 16)
		{
			iTextureType = GL_UNSIGNED_SHORT;
		}
		
		// generate texture if not already generated
		if(!m_bIsTextureGenerated)
		{
			m_iWidth = w;
			m_iHeight = h;
			m_iChannels = channels;
			m_iBitsPerPixel = bitsPerPixel;

			glGenTextures(1, &m_iTexture);
			glBindTexture(GL_TEXTURE_2D,m_iTexture);       
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   
			glTexImage2D(GL_TEXTURE_2D, 0, iInternalTextureFormat, w, h, 0, iTextureFormat, iTextureType, NULL );    
			m_bIsTextureGenerated = true;
		}
		// update texture
		glBindTexture(GL_TEXTURE_2D,m_iTexture); 
		glTexImage2D(GL_TEXTURE_2D, 0, iInternalTextureFormat, w, h, 0, iTextureFormat, iTextureType, pTexture );    

		glDisable(GL_TEXTURE_2D);
	}

	update();
}