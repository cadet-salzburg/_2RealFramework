#include "QGlTextureImage.h"
#include "_2RealDatatypes.h"

using _2Real::ImageType;

#include <iostream>

QGlTextureImage::QGlTextureImage( QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {}
QGlTextureImage::~QGlTextureImage() {}

void QGlTextureImage::initializeGL() 
{
	makeCurrent();

	glGenTextures( 1, &m_iTexture );
	glBindTexture( GL_TEXTURE_2D, m_iTexture );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( 0, 1, 0, 1, 0, 1 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glColor4f( 1, 1, 1, 1 );
	glEnable( GL_TEXTURE_2D );
}

void QGlTextureImage::resizeGL(int w, int h)
{
	makeCurrent();
	glViewport( 0, 0, w, h );
}

void QGlTextureImage::paintGL()
{
	makeCurrent();

	glClear( GL_COLOR_BUFFER_BIT );
	glBegin( GL_QUADS );
		glTexCoord2i(0,1); glVertex2i(0,0);
		glTexCoord2i(0,0); glVertex2i(0,1);
		glTexCoord2i(1,0); glVertex2i(1,1);
		glTexCoord2i(1,1); glVertex2i(1,0);
	glEnd();

	glFlush();
}

void QGlTextureImage::checkGLState( std::string const& msg )
{
	GLenum status = glGetError();
	if ( status != GL_NO_ERROR )
	{
		std::cout << "the GL is in error state: " << status << " " << std::endl;
	}
}

void QGlTextureImage::updateTexture( const unsigned int width, const unsigned int height, const unsigned int channels, ImageType const& type, void const* data )
{
	makeCurrent();

	if ( ( width > 0 ) && ( height > 0 ) )
	{
		GLint iTextureFormat, iPixelFormat, iDataType;

		if ( channels == 1 )			iTextureFormat = iPixelFormat = GL_LUMINANCE;
		else if ( channels == 3 )		iTextureFormat = iPixelFormat = GL_RGB;
		else if ( channels == 4 )		iTextureFormat = iPixelFormat = GL_RGBA;
		else if ( channels == 2 )
		{
			std::cout << "2 channel images cannot be converted to gl textures" << std::endl;
			return;
		}
		else
		{
			std::cout << "invalid number of channels" << std::endl;
			return;
		}

		if ( type == ImageType::UNSIGNED_BYTE )			iDataType = GL_UNSIGNED_BYTE;
		else if ( type == ImageType::UNSIGNED_SHORT )	iDataType = GL_UNSIGNED_SHORT;
		else if ( type == ImageType::FLOAT )			iDataType = GL_FLOAT;
		else if ( type == ImageType::DOUBLE)
		{
			std::cout << "DOUBLE images cannot be converted to gl textures" << std::endl;
			return;
		}
		else
		{
			std::cout << "invalid image type" << std::endl;
			return;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, iTextureFormat, width, height, 0, iPixelFormat, iDataType, data );

		checkGLState( ", encountered after pixel transfer" );
	}
	else
	{
		std::cout << "image has invalid size" << std::endl;
		return;
	}

	update();
}