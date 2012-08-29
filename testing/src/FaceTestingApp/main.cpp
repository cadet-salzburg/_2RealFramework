/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2012 Fachhochschule Salzburg GmbH

		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "_2RealApplication.h"

#include <iostream>
#include <sstream>
#include <map>
#include <list>

#include "GL/glew.h"
#include "GL/wglew.h"
#include "SDL.h"

#include "windows.h"
#include "GL/GL.h"
#include "GL/GLU.h"

#include "Poco/Mutex.h"

/*
#ifndef _UNIX
	#include "vld.h"
#endif
*/
#include <sstream>

#include "windows.h"

using std::string;
using std::cout;
using std::endl;
using std::cin;

using _2Real::Exception;

using _2Real::app::Engine;
using _2Real::app::BundleInfo;
using _2Real::app::BlockInfo;
using _2Real::app::BlockHandle;
using _2Real::app::BundleHandle;
using _2Real::app::InletHandle;
using _2Real::app::OutletHandle;
using _2Real::app::AppData;
using _2Real::ImageT;

using Poco::ScopedLock;
using Poco::FastMutex;

using namespace _2Real;



bool exiting	=	false;

bool run		=	true;


SDL_GLContext context;
SDL_Window *window	=	NULL;

Engine &engine = Engine::instance();

BundleHandle oniBundle;
BundleHandle camBundle;
BundleHandle ftBundle;

BlockHandle camera;

BlockHandle rgb;
BlockHandle depth;

BlockHandle faceCast;
BlockHandle faceFeatures;

OutletHandle camOutImg;

OutletHandle rgbOutImg;
OutletHandle depthOutImg;


namespace _2Real
{
	inline const Vec3 &white()
	{
		static Vec3 w( 1.0f, 1.0f, 1.0f );
		return w;
	}

	inline const Vec3 &black()
	{
		static Vec3 b( 0.0f, 0.0f, 0.0f );
		return b;
	}

	inline const Vec3 &red()
	{
		static Vec3 r( 1.0f, 0.0f, 0.0f );
		return r;
	}

	inline const Vec3 &green()
	{
		static Vec3 g( 0.0f, 1.0f, 0.0f );
		return g;
	}

	inline const Vec3 &blue()
	{
		static Vec3 b( 0.0f, 0.0f, 1.0f );
		return b;
	}

	inline const Vec3 &cyan()
	{
		static Vec3 c( 0.0f, 1.0f, 1.0f );
		return c;
	}

	inline const Vec3 &magenta()
	{
		static Vec3 m( 1.0f, 0.0f, 1.0f );
		return m;
	}

	inline const Vec3 &yellow()
	{
		static Vec3 y( 1.0f, 1.0f, 0.0f );
		return y;
	}

	inline const Vec3 &darkgrey()
	{
		static Vec3 dg( 0.25f, 0.25f, 0.25f );
		return dg;
	}

	inline const Vec3 &grey()
	{
		static Vec3 g( 0.5f, 0.5f, 0.5f );
		return g;
	}

	inline const Vec3 &lightgrey()
	{
		static Vec3 lg( 0.75f, 0.75f, 0.75f );
		return lg;
	}

	void drawAxes( double scale )
	{
		glPushAttrib( GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT );
		{
			glDisable( GL_LIGHTING );
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_COLOR_MATERIAL );

			glBegin( GL_LINES );
			{
				glColor3f( 1.0, 0.0, 0.0 );
				glVertex3d( 0.0, 0.0, 0.0 );
				glVertex3d( scale, 0.0, 0.0 );

				glColor3f( 0.0, 1.0, 0.0 );
				glVertex3d( 0.0, 0.0, 0.0 );
				glVertex3d( 0.0, scale, 0.0 );

				glColor3f( 0.0, 0.0, 1.0 );
				glVertex3d( 0.0, 0.0, 0.0 );
				glVertex3d( 0.0, 0.0, scale );
			}
			glEnd();
		}
		glPopAttrib();
	}
}

struct ImageData
{
	unsigned char	*data;

	unsigned int	dataSize;

	unsigned int	w;
	unsigned int	h;
	unsigned int	d;

	bool			dirty;
};

class Receiver
{
public:
	Receiver( unsigned int initialWidth, unsigned int initialHeight, unsigned int initialDepth )
	{
		m_colors.push_back( red() );
		m_colors.push_back( green() );
		m_colors.push_back( blue() );
		m_colors.push_back( cyan() );
		m_colors.push_back( magenta() );
		m_colors.push_back( yellow() );

		m_ImageData.data = NULL;
		m_ImageData.dirty = false;

		glGenTextures( 1, &m_Texture );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_Texture );

		this->updateDimensions( initialWidth, initialHeight, initialDepth );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );

		glDisable( GL_DEPTH_TEST );

		glClearColor( 0.0f, 0.1f, 0.2f, 1.0f );
	}

	virtual ~Receiver()
	{
		if( m_ImageData.data )
		{
			delete m_ImageData.data;
			m_ImageData.data = NULL;
		}

		glDeleteTextures( 1, &m_Texture );
	}

	virtual void onEvent( const SDL_Event &event )
	{}

	virtual	void display()	=	0;

protected:
	GLuint		m_Texture;
	ImageData	m_ImageData;

	Poco::FastMutex		m_Access;

	std::vector< _2Real::Vec3 >		m_colors;


	void updateDimensions( unsigned int width, unsigned int height, unsigned int bpp )
	{
		if( m_ImageData.data )
			delete m_ImageData.data;

		m_ImageData.w = width;
		m_ImageData.h = height;
		m_ImageData.d = bpp / 8;

		std::cout << "set texture dimensions to " << m_ImageData.w << " x " << m_ImageData.h << " x " << m_ImageData.d << std::endl;

		m_ImageData.dataSize = m_ImageData.w * m_ImageData.h * m_ImageData.d;
		m_ImageData.data = new unsigned char[m_ImageData.dataSize];

		//TODO: set format according to color channel order and bpp
//		if( m_ImageData.d == 3 )
			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA32F, m_ImageData.w, m_ImageData.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
//		else
//			glTexImage2D( GL_TEXTURE_2D, 0, GL_LUMINANCE, m_ImageData.w, m_ImageData.h, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, NULL );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		//glGenerateMipmap( GL_TEXTURE_2D );
	}

	void updateTexture()
	{
		//TODO: use gl formats according to image properties
		if( m_ImageData.d == 3 )
			glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_ImageData.w, m_ImageData.h, GL_RGB, GL_UNSIGNED_BYTE, m_ImageData.data );
		else
		{
			static unsigned char data[640*480*3];

			unsigned char *dataPtr = data;
			unsigned short *imgPtr = (unsigned short*)m_ImageData.data;
			for( int j = 0; j < m_ImageData.w; j++ )
				for( int i = 0; i < m_ImageData.h; i++ )
				{
					*( dataPtr++ ) = *imgPtr / 40;
					*( dataPtr++ ) = *imgPtr / 40;
					*( dataPtr++ ) = *imgPtr / 40;

					imgPtr++;
				}

			glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_ImageData.w, m_ImageData.h, GL_RGB, GL_UNSIGNED_BYTE, data );
			//glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, m_ImageData.w, m_ImageData.h, GL_LUMINANCE, GL_UNSIGNED_SHORT, m_ImageData.data );
		}

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glGenerateMipmap( GL_TEXTURE_2D );
	}

	void drawRect( float x0, float y0, float x1, float y1 )
	{
		x0 = ( x0 - 0.5f ) * 2.0f;	//lower left is -1/-1
		y0 = ( 0.5f - y0 ) * 2.0f;
		x1 = ( x1 - 0.5f ) * 2.0f;	//upper right is 1/1
		y1 = ( 0.5f - y1 ) * 2.0f;

		glPushAttrib( GL_ALL_ATTRIB_BITS );
		{
			glEnable( GL_COLOR_MATERIAL );
			glDisable( GL_TEXTURE_2D );

			glBegin( GL_LINE_LOOP );
			{
				glVertex3f( x0, y0, 0.0f );
				glVertex3f( x0, y1, 0.0f );
				glVertex3f( x1, y1, 0.0f );
				glVertex3f( x1, y0, 0.0f );
			}
			glEnd();
		}
		glPopAttrib();
	}

	void drawRect( const _2Real::Vec2 &v0, const _2Real::Vec2 &v1 )
	{
		drawRect( v0[0], v0[1], v1[0], v1[1] );
	}

	void updateImage( const _2Real::Image &img )
	{
		m_Access.lock();

		//TODO: handle (changes in) image type
		//TODO: handle (changes in) bpp
		//TODO: handle (changes in) byte order
		
		if( img.getWidth() != m_ImageData.w || img.getHeight() != m_ImageData.h || img.getBitsPerPixel() / 8 != m_ImageData.d ) //TODO: this return 8 when in fact it should return 24
			//TODO: weirdly, this does not work. if i call updateDimensions twice in the constructor everything's fine, but if the second call 
			// is due to a change of image dimensions here, texturing stops working. and gDebugger even crashes at this point.
			this->updateDimensions( img.getWidth(), img.getHeight(), img.getBitsPerPixel() ); //TODO: this return 8 when in fact it should return 24

		if( img.getByteSize() != m_ImageData.dataSize )
			std::cerr << "ERROR: received image with unexpected data size" << std::endl;
		else
		{
			memcpy( m_ImageData.data, img.getData(), img.getByteSize() );
			m_ImageData.dirty = true;
		}

		m_Access.unlock();
	}
};

class FaceFeatureReceiver : public Receiver
{
public:
	FaceFeatureReceiver() : 
		Receiver( 320, 240, 8 )
	{
		m_FaceData.dirty = false;
	}

	virtual ~FaceFeatureReceiver()
	{}

	void drawFaceRects()
	{
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		{
			for( std::vector< _2Real::FaceDesc >::const_iterator it = m_FaceData.faces.begin(); it != m_FaceData.faces.end(); ++it )
			{
				glColor3fv( m_colors[it->faceID() % m_colors.size()].data() );

				glLineWidth( 4.0f );
				drawRect( it->faceRegion().getP0(), it->faceRegion().getP1() );

				Vec2 faceSize( it->faceRegion().getP1() - it->faceRegion().getP0() );

				glLineWidth( 3.0f );
				if( it->eyeLeft().isValid() )
				{
					drawRect( 
						it->faceRegion().getP0() + Vec2( it->eyeLeft().region().getP0()[0] * faceSize[0], it->eyeLeft().region().getP0()[1] * faceSize[1] ),
						it->faceRegion().getP0() + Vec2( it->eyeLeft().region().getP1()[0] * faceSize[0], it->eyeLeft().region().getP1()[1] * faceSize[1] ) );
				}
				if( it->eyeRight().isValid() )
				{
					drawRect( 
						it->faceRegion().getP0() + Vec2( it->eyeRight().region().getP0()[0] * faceSize[0], it->eyeRight().region().getP0()[1] * faceSize[1] ),
						it->faceRegion().getP0() + Vec2( it->eyeRight().region().getP1()[0] * faceSize[0], it->eyeRight().region().getP1()[1] * faceSize[1] ) );
				}

				glLineWidth( 2.0f );
				if( it->nose().isValid() )
				{
					drawRect( 
						it->faceRegion().getP0() + Vec2( it->nose().region().getP0()[0] * faceSize[0], it->nose().region().getP0()[1] * faceSize[1] ),
						it->faceRegion().getP0() + Vec2( it->nose().region().getP1()[0] * faceSize[0], it->nose().region().getP1()[1] * faceSize[1] ) );
				}

				glLineWidth( 1.0f );
				if( it->mouth().isValid() )
				{
					drawRect( 
						it->faceRegion().getP0() + Vec2( it->mouth().region().getP0()[0] * faceSize[0], it->mouth().region().getP0()[1] * faceSize[1] ),
						it->faceRegion().getP0() + Vec2( it->mouth().region().getP1()[0] * faceSize[0], it->mouth().region().getP1()[1] * faceSize[1] ) );
				}
			}
		}
		glPopAttrib();
	}

	void receiveFaces( AppData const &data )
	{
		m_Access.lock();

		m_FaceData.faces = data.getData< std::vector< _2Real::FaceDesc > >();
		m_FaceData.dirty = true;
		std::cout << "received " << m_FaceData.faces.size() << " faces" << std::endl;

		m_Access.unlock();
	}

	void receiveImage( AppData const &data )
	{
		this->updateImage( data.getData< _2Real::Image >() );
	}

	virtual void display()
	{
		glClear( GL_COLOR_BUFFER_BIT );

		m_Access.lock();
		if( m_ImageData.dirty )
		{
			updateTexture();
			m_ImageData.dirty = false;
		}
		m_Access.unlock();

		glColor3fv( white().data() );
		glBegin( GL_QUADS );
		{
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 0.0f );
		}
		glEnd();

		m_Access.lock();

		drawFaceRects();
		m_FaceData.dirty = false;

		m_Access.unlock();
	}

private:
	struct FaceData
	{
		std::vector< _2Real::FaceDesc >	faces;

		bool			dirty;
	};

	FaceData			m_FaceData;
};

class FaceCastReceiver : public Receiver
{
public:
	FaceCastReceiver() :
		Receiver( 640, 480, 16 ),
		m_mouseButtonFlags( 0 ),
		m_oldMouseButtonFlags( 0 ),
		m_mouseX( 0 ),
		m_mouseY( 0 ),
		m_oldMouseX( 0 ),
		m_oldMouseY( 0 ),
		m_fov( 60.0 ),
		m_ar( 1.0 ),
		m_windowWidth( 0 ),
		m_windowHeight( 0 ),
		m_motionSpeed( 5.0f ),
		m_rotationSpeed( 180.0f ),
		m_trackballTrans( 0.0f, 0.0f, -5.0f ),
		m_trackballRot( 0.0f, 0.0f, 0.0f ),
		m_clipNear( 0.005 ),
		m_clipFar( 500.0 )
	{
		m_FaceData.dirty = false;
	}

	virtual ~FaceCastReceiver()
	{}

	void draw3DFaces()
	{
		glPushAttrib( GL_ALL_ATTRIB_BITS );
		glMatrixMode( GL_PROJECTION );
		glPushMatrix();
		{
			glLoadIdentity();
			gluPerspective( m_fov, m_ar, m_clipNear, m_clipFar );

			glMatrixMode( GL_MODELVIEW );
			glLoadIdentity();

			glTranslatef( m_trackballTrans[0], m_trackballTrans[1], m_trackballTrans[2] );
			glRotatef( m_trackballRot[0], 1.0f, 0.0f, 0.0f );
			glRotatef( m_trackballRot[1], 0.0f, 1.0f, 0.0f );
			glRotatef( m_trackballRot[2], 0.0f, 0.0f, 1.0f );

			glPointSize( 3.0 );

			drawAxes( 1.0f );

			glDisable( GL_LIGHTING );
			glDisable( GL_TEXTURE_2D );
			glEnable( GL_COLOR_MATERIAL );

			Poco::ScopedLock<Poco::FastMutex> lock( m_Access );
			for( std::vector<FaceCast>::const_iterator it =  m_FaceData.faces.begin(); it != m_FaceData.faces.end(); ++it )
			{
				const std::vector<Vec3> &vertices = it->getVertices();
				const std::vector<Vec3> &normals = it->getNormals();
				const std::vector<unsigned int> &indices = it->getIndices();
	
				if( vertices.size() != normals.size() )
					std::cerr << "vertex and normal array size do not match" << std::endl;

				Vec3 color( m_colors[it->faceID() % m_colors.size()] );

				std::vector<Vec3>::const_iterator vIt = vertices.begin();
				std::vector<unsigned int>::const_iterator iIt = indices.begin();
				
				std::cout << "drawing " << vertices.size() << " vertices" << std::endl;
				glColor3fv( color.data() );
				glBegin( GL_POINTS );
				for( vIt; vIt != vertices.end() && iIt != indices.end(); ++vIt, ++iIt )
					if( *iIt != ~0x00 )
						glVertex3f( ( *vIt )[0], ( *vIt )[1], ( *vIt )[2] );
				glEnd();

				color[0] = 1.0f - color[0];
				color[1] = 1.0f - color[1];
				color[2] = 1.0f - color[2];

				vIt = vertices.begin();

				/*
				std::cout << "drawing " << normals.size() << " normals" << std::endl;
				glColor3fv( color.data() );
				glBegin( GL_LINES );
				for( std::vector<Vec3>::const_iterator nIt = normals.begin(); nIt != normals.end() && vIt != vertices.end(); ++nIt, ++vIt )
				{
					glVertex3fv( vIt->data() );
					glVertex3fv( Vec3( *vIt + ( *nIt * 0.01 ) ).data() );
				}
				glEnd();
				*/
			}
		}
		glMatrixMode( GL_PROJECTION_MATRIX );
		glPopMatrix();
		glPopAttrib();
	}

	void receiveFaces( AppData const &data )
	{
		m_Access.lock();

		m_FaceData.faces = data.getData< std::vector< FaceCast > >();
		m_FaceData.dirty = true;
		std::cout << "received " << m_FaceData.faces.size() << " faces" << std::endl;

		m_Access.unlock();
	}

	void receiveImage( AppData const &data )
	{
		if( this->m_showRGB )
		{
			std::cout << "updating image" << std::endl;
			this->updateImage( data.getData< _2Real::Image >() );
		}
	}

	void receiveDepth( AppData const &data )
	{
		if( !this->m_showRGB )
			this->updateImage( data.getData< _2Real::Image >() );
	}

	virtual void display()
	{
		if( m_mouseButtonFlags & 0x01 << 1 )
		{
			m_trackballRot[1] += (float)( m_mouseX - m_oldMouseX ) / m_windowWidth * m_rotationSpeed;
			m_trackballRot[0] += (float)( m_mouseY - m_oldMouseY ) / m_windowWidth * m_rotationSpeed;
		}

		if( m_mouseButtonFlags & 0x01 << 2 )
		{
			m_trackballTrans[2] -= (float)( m_mouseY - m_oldMouseY ) / m_windowWidth * m_motionSpeed;
		}

		if( m_mouseButtonFlags & 0x01 << 3 )
		{
			m_trackballTrans[0] += (float)( m_mouseX - m_oldMouseX ) / m_windowWidth * m_motionSpeed;
			m_trackballTrans[1] -= (float)( m_mouseY - m_oldMouseY ) / m_windowWidth * m_motionSpeed;
		}

		m_oldMouseX = m_mouseX;
		m_oldMouseY = m_mouseY;
		m_oldMouseButtonFlags = m_mouseButtonFlags;

		glClear( GL_COLOR_BUFFER_BIT );

		m_Access.lock();
		if( m_ImageData.dirty )
		{
			updateTexture();
			m_ImageData.dirty = false;
		}
		m_Access.unlock();

		/*
		glColor3fv( white().data() );
		glBegin( GL_QUADS );
		{
			glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 0.0f );
			glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 0.0f );
			glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 0.0f );
		}
		glEnd();
		*/

		draw3DFaces();
		m_FaceData.dirty = false;
	}

	virtual void onEvent( const SDL_Event &event )
	{
		Receiver::onEvent( event );

		if( event.type == SDL_MOUSEMOTION )
		{
			m_mouseX = event.motion.x;
			m_mouseY = event.motion.y;
		}
		if( event.type == SDL_MOUSEBUTTONDOWN )
		{
			m_mouseButtonFlags |= 0x01 << event.button.button;

			m_mouseX = event.motion.x;
			m_mouseY = event.motion.y;
		}
		else if( event.type == SDL_MOUSEBUTTONUP )
		{
			m_mouseButtonFlags &= ~( 0x01 << event.button.button );

			m_mouseX = event.motion.x;
			m_mouseY = event.motion.y;
		}
		else if( event.type == SDL_KEYUP )
		{
			switch( event.key.keysym.scancode )
			{
			case SDL_SCANCODE_SPACE:
				m_showRGB = !m_showRGB;
				break;
			}
		}
		else if( event.type == SDL_WINDOWEVENT )
		{
			switch( event.window.event )
			{
			case SDL_WINDOWEVENT_SHOWN:
				{
					int w, h;
					SDL_GetWindowSize( ::window, &w, &h );
					m_ar = (float)w / h;
					m_windowWidth = w;
					m_windowHeight = h;
				}
				break;
			case SDL_WINDOWEVENT_RESIZED:
				m_ar = (float)event.window.data1 / event.window.data2;
				m_windowWidth = event.window.data1;
				m_windowHeight = event.window.data2;
				break;
			}
		}
	}

private:
	struct FaceData
	{
		std::vector< _2Real::FaceCast >	faces;

		bool			dirty;
	};

	bool		m_showRGB;
	FaceData	m_FaceData;

	unsigned int	m_mouseButtonFlags;
	unsigned int	m_oldMouseButtonFlags;

	int			m_mouseX;
	int			m_mouseY;

	int			m_oldMouseX;
	int			m_oldMouseY;

	double		m_fov;
	double		m_ar;

	unsigned int	m_windowWidth;
	unsigned int	m_windowHeight;

	double		m_motionSpeed;
	double		m_rotationSpeed;

	Vec3		m_trackballTrans;
	Vec3		m_trackballRot;

	double		m_clipNear;
	double		m_clipFar;
};

Receiver *receiver	=	NULL;


void initSDLGL()
{
	SDL_Init( SDL_INIT_VIDEO );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	window = SDL_CreateWindow( "yay", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	context = SDL_GL_CreateContext( window );
	SDL_GL_MakeCurrent( window, context );

	string error = SDL_GetError();
	cout << error << endl;

	GLenum err = glewInit();
	if ( err != GLEW_OK )
	{
		cout << "glew error" << endl;
	}

	SDL_GL_SetSwapInterval( 1 );
}

void setupFeatures()
{
	engine.setBaseDirectory( "E:\\work\\source\\_2RealFramework\\bundles\\bin" );

	camBundle = engine.loadBundle( "CameraCaptureBundle" );

	camera = camBundle.createBlockInstance( "CameraCaptureBlock" );
	camera.setUpdateRate( 30.0 );

	InletHandle inWidth = camera.getInletHandle( "Width" );
	InletHandle inHeight = camera.getInletHandle( "Height" );
	InletHandle inFPS = camera.getInletHandle( "Fps" );

	camOutImg = camera.getOutletHandle( "ImageData" );

	OutletHandle camOutWidth = camera.getOutletHandle( "Width" );
	OutletHandle camOutHeight = camera.getOutletHandle( "Height" );

	ftBundle = engine.loadBundle( "FaceTrackingBundle" );

	faceFeatures = ftBundle.createBlockInstance( "FaceFeaturesBlock" );
	faceFeatures.setUpdateRate( 30.0f );

	InletHandle ffInImg = faceFeatures.getInletHandle( "image_in" );

	faceFeatures.getInletHandle( "use_eyes" ).setValue( true );
	faceFeatures.getInletHandle( "use_nose" ).setValue( true );
	faceFeatures.getInletHandle( "use_mouth" ).setValue( true );

	faceFeatures.getInletHandle( "haar_detection_downscale" ).setValue( 0.5f );

	ffInImg.setUpdatePolicy( InletHandle::OR_NEWER_DATA );

	if ( !ffInImg.link( camOutImg ) )
	{
		std::cerr << "WARNING: conversion failed, trying to link with conversion" << std::endl;
		ffInImg.linkWithConversion( camOutImg );
		std::cerr << "succeeded!" << std::endl;
	}

	receiver = new FaceFeatureReceiver();

	faceFeatures.getOutletHandle( "face_out" ).registerToNewData( *( (FaceFeatureReceiver*)receiver ), &FaceFeatureReceiver::receiveFaces );

	camOutImg.registerToNewData( *( (FaceFeatureReceiver*)receiver ), &FaceFeatureReceiver::receiveImage );

	camera.setup();
	camera.start();

	faceFeatures.setup();
	faceFeatures.start();
}

void setupCast()
{
	engine.setBaseDirectory( "E:\\work\\source\\_2RealFramework\\bundles\\bin" );

	oniBundle = engine.loadBundle( "KinectOpenNIBundle" );

	rgb = oniBundle.createBlockInstance( "KinectOpenNIRgbBlock" );
	depth = oniBundle.createBlockInstance( "KinectOpenNIDepthBlock" );

	rgb.setUpdateRate( 30.0 );
	depth.setUpdateRate( 30.0 );

	rgbOutImg = rgb.getOutletHandle( "ImageData" );
	depthOutImg = depth.getOutletHandle( "ImageData" );

	ftBundle = engine.loadBundle( "FaceTrackingBundle" );

	faceCast = ftBundle.createBlockInstance( "FaceCastBlock" );
	faceCast.setUpdateRate( 30.0f );
	
	InletHandle fcInImg = faceCast.getInletHandle( "image_in" );
	InletHandle fcInDepth = faceCast.getInletHandle( "depth_in" );

	fcInImg.setUpdatePolicy( InletHandle::OR_NEWER_DATA );

	if( !fcInImg.link( rgbOutImg ) )
	{
		std::cerr << "WARNING: conversion failed, trying to link with conversion" << std::endl;
		fcInImg.linkWithConversion( rgbOutImg );
		std::cerr << "succeeded!" << std::endl;
	}

	if( !fcInDepth.link( depthOutImg ) )
	{
		std::cerr << "WARNING: conversion failed, trying to link with conversion" << std::endl;
		fcInDepth.linkWithConversion( depthOutImg );
		std::cerr << "succeeded!" << std::endl;
	}

	receiver = new FaceCastReceiver();

	//rgbOutImg.registerToNewData( *( (FaceCastReceiver*)receiver ), &FaceCastReceiver::receiveImage );
	//depthOutImg.registerToNewData( *( (FaceCastReceiver*)receiver ), &FaceCastReceiver::receiveDepth );
	faceCast.getOutletHandle( "face_out" ).registerToNewData( *( (FaceCastReceiver*)receiver ), &FaceCastReceiver::receiveFaces );

	rgb.setup();
	rgb.start();

	depth.setup();
	depth.start();

	faceCast.setup();
	faceCast.start();
}

void cleanup()
{
	if( camera.isValid() )
		camera.stop();
	if( rgb.isValid() )
		rgb.stop();
	if( depth.isValid() )
		depth.stop();
	if( faceFeatures.isValid() )
		faceFeatures.stop();
	if( faceCast.isValid() )
		faceCast.stop();

	if( receiver )
	{
		if( faceFeatures.isValid() )
			faceFeatures.getOutletHandle( "face_out" ).unregisterFromNewData( *( (FaceFeatureReceiver*)receiver ), &FaceFeatureReceiver::receiveFaces );
		if( camOutImg.isValid() )
			camOutImg.unregisterFromNewData( *( (FaceFeatureReceiver*)receiver ), &FaceFeatureReceiver::receiveImage );

		if( faceCast.isValid() )
			faceCast.getOutletHandle( "face_out" ).unregisterFromNewData( *( (FaceCastReceiver*)receiver ), &FaceCastReceiver::receiveFaces );
		if( rgbOutImg.isValid() )
			rgbOutImg.unregisterFromNewData( *( (FaceCastReceiver*)receiver ), &FaceCastReceiver::receiveImage );
		if( depthOutImg.isValid() )
			depthOutImg.unregisterFromNewData( *( (FaceCastReceiver*)receiver ), &FaceCastReceiver::receiveDepth );

		delete receiver;
		receiver = NULL;
	}

	SDL_GL_DeleteContext( context );
	if( window )
	{
		SDL_DestroyWindow( window );
		window = NULL;
	}

	engine.safeConfig( "img_test.xml" );

	if( camBundle.isValid() )
		camBundle.unload();
	if( oniBundle.isValid() )
		oniBundle.unload();
	if( ftBundle.isValid() )
		ftBundle.unload();
}

void __cdecl onExit()
{
	exiting = true;

	std::cout << "on exit called" << std::endl;

	cleanup();
}

void __cdecl onTerminate()
{
	std::cerr << "process terminated" << std::endl;
	abort();
}

void __cdecl onUnexpected()
{
	std::cerr << "an unexpected error occured" << std::endl;
}

int main( int argc, char *argv[] )
{
	atexit( onExit );
	std::set_terminate( onTerminate );
	std::set_unexpected( onUnexpected );

	try
	{
		initSDLGL();

		//setupFeatures();
		setupCast();

		SDL_Event ev;
		while( ::run )
		{
			while( SDL_PollEvent( &ev ) )
			{
				if( receiver )
					receiver->onEvent( ev );

				switch ( ev.type )
				{
				case SDL_KEYDOWN:
					if( ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
						run = false;
					break;
				case SDL_QUIT:
					run = false;

					break;
				default:
					break;
				}
			}

			if( receiver )
				receiver->display();

			SDL_GL_SwapWindow( window );
		}
	}
	catch ( Exception &e )
	{
		cout << e.what() << " " << e.message() << endl;

#ifdef _DEBUG
		getchar();
#endif
	}

	return 0;
}