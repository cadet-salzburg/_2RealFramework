#include "OpenGl.h"

#include <iostream>

namespace _2Real
{
	namespace gl
	{
		GlRessourceManager::GlRessourceManager( RenderSettings const& s ) :
			m_CreationSettings( s ),
			m_SfContext( sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ), s.width, s.height ),
			m_GlewContext( new GLEWContext() )
		{
			GLenum success = glewContextInit( m_GlewContext.get() );
			if ( success != GLEW_OK )
			{
				throw std::exception( "could not create GLEW context" );
			}
			else
			{
				success = glewInit();
				if ( success != GLEW_OK )
				{
					throw std::exception( "could not initialize GLEW" );
				}
			}
		}

		void GlRessourceManager::cleanUp()
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

			for ( std::list< GLuint >::iterator it = m_DeletedTextures.begin(); it != m_DeletedTextures.end(); ++it )
			{
				GLuint tmp = *it;
				for ( std::list< TextureObj* >::iterator tIt = m_Textures.begin(); tIt != m_Textures.end(); )
				{
					if ( ( *tIt )->m_Handle == tmp )
					{
						( *tIt )->invalidate();
						tIt = m_Textures.erase( tIt );
					}
					else ++tIt;
				}
				glDeleteTextures( 1, &tmp );
			}

			m_DeletedTextures.clear();

			for ( std::list< GLuint >::iterator it = m_DeletedBuffers.begin(); it != m_DeletedBuffers.end(); ++it )
			{
				GLuint tmp = *it;
				for ( std::list< BufferObj* >::iterator bIt = m_Buffers.begin(); bIt != m_Buffers.end(); )
				{
					if ( ( *bIt )->m_Handle == tmp )
					{
						( *bIt )->invalidate();
						bIt = m_Buffers.erase( bIt );
					}
					else ++bIt;
				}
				glDeleteBuffers( 1, &tmp );
			}

			m_DeletedBuffers.clear();
		}

		GlRessourceManager::~GlRessourceManager()
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

			for ( std::list< GLuint >::iterator it = m_DeletedTextures.begin(); it != m_DeletedTextures.end(); ++it )
			{
				GLuint tmp = *it;
				for ( std::list< TextureObj* >::iterator tIt = m_Textures.begin(); tIt != m_Textures.end(); )
				{
					if ( ( *tIt )->m_Handle == tmp )
					{
						( *tIt )->invalidate();
						tIt = m_Textures.erase( tIt );
					}
					else ++tIt;
				}
				glDeleteTextures( 1, &tmp );
			}

			m_DeletedTextures.clear();

			for ( std::list< GLuint >::iterator it = m_DeletedBuffers.begin(); it != m_DeletedBuffers.end(); ++it )
			{
				GLuint tmp = *it;
				for ( std::list< BufferObj* >::iterator bIt = m_Buffers.begin(); bIt != m_Buffers.end(); )
				{
					if ( ( *bIt )->m_Handle == tmp )
					{
						( *bIt )->invalidate();
						bIt = m_Buffers.erase( bIt );
					}
					else ++bIt;
				}
				glDeleteBuffers( 1, &tmp );
			}

			m_DeletedBuffers.clear();

			for ( std::list< TextureObj* >::iterator it = m_Textures.begin(); it != m_Textures.end(); ++it )
			{
				( *it )->invalidate();
				glDeleteTextures( 1, &( ( *it )->m_Handle ) );
			}

			m_Textures.clear();

			for ( std::list< BufferObj* >::iterator it = m_Buffers.begin(); it != m_Buffers.end(); ++it )
			{
				( *it )->invalidate();
				glDeleteBuffers( 1, &( ( *it )->m_Handle ) );
			}

			m_Buffers.clear();
		}

		Texture GlRessourceManager::createTexture( const GLenum target )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

			GLuint handle;
			glGenTextures( 1, &handle );
			Texture t( new TextureObj( *this, handle, target ) );
			m_Textures.push_back( t.get() );
			return t;
		}

		Buffer GlRessourceManager::createBuffer( const GLenum usageHint )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );

			GLuint handle;
			glGenBuffers( 1, &handle );
			Buffer b( new BufferObj( *this, handle, usageHint) );
			m_Buffers.push_back( b.get() );
			return b;
		}

		void GlRessourceManager::destroyTexture( TextureObj &texture )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );
			m_DeletedTextures.push_back( texture.m_Handle );
		}

		void GlRessourceManager::destroyBuffer( BufferObj &buffer )
		{
			Poco::ScopedLock< Poco::FastMutex > lock( m_Mutex );
			m_DeletedBuffers.push_back( buffer.m_Handle );
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		RenderWindow::RenderWindow( RenderSettings const& s ) :
			m_CreationSettings( s ),
			m_SfWindow( sf::VideoMode( s.width, s.height, s.colorBits ), s.title, sf::Style::Resize, sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ) ),
			m_SfSettings( m_SfWindow.getSettings() ),
			m_Renderer()
		{
			//std::cout << "----------WINDOW SETTINGS----------" << std::endl;
			//std::cout << "opengl version:\t\t" << m_SfSettings.majorVersion << "." << m_SfSettings.minorVersion << std::endl;
			//std::cout << "antialiasing samples:\t" << m_SfSettings.antialiasingLevel << std::endl;
		}

		void RenderWindow::processEvents()
		{
			sf::Event ev;
			while ( m_SfWindow.pollEvent( ev ) )
			{
				if ( ev.type == sf::Event::Resized )
				{
					m_Renderer.setViewport( ev.size.width, ev.size.height );
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		RenderContext::RenderContext( RenderSettings const& s ) :
			m_CreationSettings( s ),
			m_SfContext( sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ), s.width, s.height ),
			m_Renderer( s.width, s.height )
		{
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		Renderer::Renderer() :
			m_GlewContext( new GLEWContext )
		{
			GLenum success = glewContextInit( m_GlewContext.get() );
			if ( success != GLEW_OK )
			{
				throw std::exception( "could not create GLEW context" );
			}
			else
			{
				success = glewInit();
				if ( success != GLEW_OK )
				{
					throw std::exception( "could not initialize GLEW" );
				}
			}

			// create tex units
			GLint val = 0;
			glGetIntegerv( GL_MAX_TEXTURE_COORDS, &val );		// TODO: not sure if this is the correct value to grab, the api doc is a bit unclear :/

			for ( unsigned int i=0; i<=val; ++i )
			{
				m_TextureUnits.push_back( TextureUnit( i, m_GlewContext ) );
			}

			// create buffer targets
			m_BufferTargets.push_back( BufferTarget( GL_ARRAY_BUFFER, m_GlewContext ) );
			m_BufferTargets.push_back( BufferTarget( GL_ELEMENT_ARRAY_BUFFER, m_GlewContext ) );
			m_BufferTargets.push_back( BufferTarget( GL_PIXEL_UNPACK_BUFFER, m_GlewContext ) );
			m_BufferTargets.push_back( BufferTarget( GL_PIXEL_PACK_BUFFER, m_GlewContext ) );

			// create simplest of shaders
			// std::string vertexSrc = "#version 330 core\nin vec3 position;\nvoid main()\n{\ngl_Position = vec4( position.xyz, 1.0 );\n}\n";
			// std::string fragmentSrc = "#version 330 core\nout vec4 fragColor;\nvoid main()\n{\nfragColor = vec4( 0.5, 0.5, 0.5, 1.0 );\n}\n";

			std::string vertexSrc = "#version 330 core\nin vec3 position;\nin vec2 texcoord0;\nout vec2 fTexcoord0;\nvoid main()\n{\nfTexcoord0 = texcoord0;\ngl_Position = vec4( position.xyz, 1.0 );\n}\n";
			std::string fragmentSrc = "#version 330 core\nuniform sampler2D texture0;\nin vec2 fTexcoord0;\nout vec4 fragColor;\nvoid main()\n{\nfragColor = texture( texture0, fTexcoord0 );\n}\n";

			ShaderObj vertexShader( GL_VERTEX_SHADER, m_GlewContext, vertexSrc );
			ShaderObj fragmentShader( GL_FRAGMENT_SHADER, m_GlewContext, fragmentSrc );

			std::cout << vertexShader.getShaderInfo() << " " << fragmentShader.getShaderInfo() << std::endl;

			ProgramObj *prog = new ProgramObj( m_GlewContext );
			prog->attachShader( vertexShader );
			prog->attachShader( fragmentShader );
			prog->link();
			prog->validate();

			std::cout << prog->getProgramInfo() << std::endl;

			m_Programs[ "default" ] = prog;

			//GLchar const* vP = vert.c_str();
			//GLchar const* fP = frag.c_str();

			//GLuint prog;
			//prog = glCreateProgram();

			//GLuint v = glCreateShader( GL_VERTEX_SHADER );
			//glShaderSource( v, 1, &vP, nullptr );
			//glCompileShader( v );

			//GLint vCompiled;
			//glGetProgramiv( prog, GL_LINK_STATUS, &vCompiled );
			//if ( vCompiled )
			//{
			//	std::cout << "vertex shader compiled" << std::endl;
			//}
			//else
			//{
			//	GLint logLength = 0;
			//	GLsizei sz = 0;

			//	glGetShaderiv( v, GL_INFO_LOG_LENGTH, &logLength );

			//	if ( logLength > 1)
			//	{
			//		GLchar *log = new GLchar[ logLength ];
			//		glGetShaderInfoLog ( v, logLength, &sz, log );
			//		std::cout << "vertex shader log:\n" << log << std::endl;
			//		delete [] log;
			//	}
			//}

			//GLuint f = glCreateShader( GL_FRAGMENT_SHADER );
			//glShaderSource( f, 1, &fP, nullptr );
			//glCompileShader( f );

			//GLint fCompiled;
			//glGetProgramiv( prog, GL_LINK_STATUS, &fCompiled );
			//if ( fCompiled )
			//{
			//	std::cout << "f compiled" << std::endl;
			//}
			//else
			//{
			//	GLint logLength = 0;
			//	GLsizei sz = 0;

			//	glGetShaderiv( f, GL_INFO_LOG_LENGTH, &logLength );

			//	if ( logLength > 1)
			//	{
			//		GLchar *log = new GLchar[ logLength ];
			//		glGetShaderInfoLog ( f, logLength, &sz, log );
			//		std::cout << "vertex shader log:\n" << log << std::endl;
			//		delete [] log;
			//	}
			//}

			//glAttachShader( prog, v );
			//glAttachShader( prog, f );

			//glBindAttribLocation( prog, 0, "position" );

			//glLinkProgram( prog );

			//GLint linked;
			//glGetProgramiv( prog, GL_LINK_STATUS, &linked );
			//if ( linked )
			//{
			//	std::cout << "linked" << std::endl;
			//}

			//m_Shaders.push_back( prog );

			//std::cout << "PROGS: " << m_Shaders[0] << " " << m_Shaders.size() << std::endl;
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		GLenum TextureObj::getTextureFormat( const ImageChannelOrder order )
		{
			if ( order == ImageChannelOrder::RGBA )		return GL_RGBA;
			else if ( order == ImageChannelOrder::RGB )	return GL_RGB;
			else if ( order == ImageChannelOrder::R )	return GL_RED;
			else if ( order == ImageChannelOrder::A )	return GL_RED;
			else
			{
				std::cout << "unknown image channel order, defaulting to RGBA" << std::endl;
				return GL_RGBA;
			}
		}
	}
}