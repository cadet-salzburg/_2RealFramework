#include "OpenGl.h"

#include <iostream>

namespace _2Real
{
	namespace gl
	{
		Context::Context( RenderSettings const& s, RessourceManager const& mgr ) :
			mManager( mgr ),
			mGlewContext( new GLEWContext ),
			mSfContext( sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ), s.width, s.height )
		{
			GLenum success = glewContextInit( mGlewContext.get() );
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

		BufferObj * Context::createBufferObj() const
		{
			BufferObj *b = new BufferObj( mManager );
			glGenBuffers( 1, &b->mHandle );
			return b;
		}

		ProgramObj * Context::createProgramObj() const
		{
			ProgramObj *p = new ProgramObj( mManager );
			p->mHandle = glCreateProgram();
			return p;
		}

		ShaderObj * Context::createShaderObj( const GLenum type, std::string const& src ) const
		{
			ShaderObj *s = new ShaderObj( mManager );
			s->mHandle = glCreateShader( type );

			const char * shaderSrc = src.c_str();

			s->mSource = src;
			glShaderSource( s->mHandle, 1, &shaderSrc, nullptr );
			glCompileShader( s->mHandle );

			GLint compiled;
			glGetShaderiv( s->mHandle, GL_COMPILE_STATUS, &compiled );

			GLint size;
			glGetShaderiv( s->mHandle, GL_INFO_LOG_LENGTH, &size );
			if ( size > 0 )
			{
				char *buffer = new char[ size ];
				glGetShaderInfoLog( s->mHandle, size, nullptr, buffer );
				s->mInfoLog = buffer;
				delete [] buffer;
			}

			return s;
		}

		TextureObj * Context::createTextureObj() const
		{
			TextureObj *t = new TextureObj( mManager );
			glGenTextures( 1, &t->mHandle );
			return t;
		}

		void Context::linkProgram( ProgramObj *p ) const
		{
			glLinkProgram( p->mHandle );

			GLint linked;
			glGetProgramiv( p->mHandle, GL_LINK_STATUS, &linked );

			p->mIsLinked = static_cast< bool >( linked );

			GLint logSize;
			glGetProgramiv( p->mHandle, GL_INFO_LOG_LENGTH, &logSize );
			if ( logSize > 0 )
			{
				char *buffer = new char[ logSize ];
				glGetProgramInfoLog( p->mHandle, logSize, nullptr, buffer );
				p->mInfoLog = buffer;
				delete [] buffer;
			}

			GLint numAttribs, maxAttribLength, attribLocation, numUniforms, maxUniformLength, uniformLocation, size;
			GLenum type;
			std::string attribName, uniformName;

			glGetProgramiv( p->mHandle, GL_ACTIVE_ATTRIBUTES, &numAttribs );
			glGetProgramiv( p->mHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttribLength );
			glGetProgramiv( p->mHandle, GL_ACTIVE_UNIFORMS, &numUniforms );
			glGetProgramiv( p->mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLength );

			char * buffer = new char[ max( maxAttribLength, maxUniformLength ) ];

			for ( int i=0; i<numAttribs; ++i )
			{
				glGetActiveAttrib( p->mHandle, i, maxAttribLength, nullptr, &size, &type, buffer );
				attribName = buffer;
				attribLocation = glGetAttribLocation( p->mHandle, buffer );
				std::cout << "ATTRIBUTE " << attribName << " " << attribLocation << std::endl;

				p->mActiveAttributes[ attribName ] = ProgramObj::ActiveVar( attribLocation, size, type );
			}

			for ( int i=0; i<numUniforms; ++i )
			{
				glGetActiveUniform( p->mHandle, i, maxUniformLength, nullptr, &size, &type, buffer );
				uniformName = buffer;
				uniformLocation = glGetUniformLocation( p->mHandle, buffer );
				std::cout << "UNIFORM: " << uniformName << " " << uniformLocation << " " << size << " " << type << std::endl;

				p->mActiveUniforms[ uniformName ] = ProgramObj::ActiveVar( uniformLocation, size, type );
			}

			delete [] buffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		RenderWindow::RenderWindow( RenderSettings const& s, RessourceManager const& mgr ) :
			mSfWindow( sf::VideoMode( s.width, s.height, s.colorBits ), s.title, sf::Style::Resize, sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ) ),
			mSfSettings( mSfWindow.getSettings() ),
			mRenderer( mgr )
		{
		}

		void RenderWindow::processEvents()
		{
			sf::Event ev;
			while ( mSfWindow.pollEvent( ev ) )
			{
				if ( ev.type == sf::Event::Resized )
				{
					mRenderer.setViewport( ev.size.width, ev.size.height );
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		Renderer::Renderer( RessourceManager const& mgr ) : mGlewContext( new GLEWContext ), mManager( mgr )
		{
			GLenum success = glewContextInit( mGlewContext.get() );
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
	}
}