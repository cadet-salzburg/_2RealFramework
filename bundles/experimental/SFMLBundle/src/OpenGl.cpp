/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
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
			//std::cout << "---------------" << std::endl;
			//std::cout << src << std::endl;

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

			std::cout << s->mInfoLog << std::endl;

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

			p->mIsLinked = ( linked == 1 );

			GLint logSize;
			glGetProgramiv( p->mHandle, GL_INFO_LOG_LENGTH, &logSize );
			if ( logSize > 0 )
			{
				char *buffer = new char[ logSize ];
				glGetProgramInfoLog( p->mHandle, logSize, nullptr, buffer );
				p->mInfoLog = buffer;
				delete [] buffer;
			}

			std::cout << p->mInfoLog << std::endl;

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
				//std::cout << "ATTRIBUTE " << attribName << " " << attribLocation << std::endl;

				p->mActiveAttributes[ attribName ] = ProgramObj::ActiveInput( attribLocation, size, type );
			}

			for ( int i=0; i<numUniforms; ++i )
			{
				glGetActiveUniform( p->mHandle, i, maxUniformLength, nullptr, &size, &type, buffer );
				uniformName = buffer;
				uniformLocation = glGetUniformLocation( p->mHandle, buffer );
				//std::cout << "UNIFORM: " << uniformName << " " << uniformLocation << " " << size << " " << type << std::endl;

				p->mActiveUniforms[ uniformName ] = ProgramObj::ActiveInput( uniformLocation, size, type );
			}

			delete [] buffer;
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		RenderWindow::RenderWindow( RenderSettings const& s, RessourceManager const& mgr ) :
			mSfWindow( sf::VideoMode( s.width, s.height, s.colorBits ), s.title, sf::Style::Resize, sf::ContextSettings( s.depthBits, s.stencilBits, s.aaSamples, s.glMajor, s.glMinor ) ),
			mSfSettings( mSfWindow.getSettings() ),
			mRenderer( mgr ), mIsKeyboardEnabled( false ), mIsMouseEnabled( false ),
			mIsMiddleMouseButtonDown( false ), mIsLeftMouseButtonDown( false ), mIsRightMouseButtonDown( false )
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
					ResizeEvent r;
					r.width = ev.size.width;
					r.height = ev.size.height;
					mResizeEvent.notify( r );
				}
				else if ( ev.type == sf::Event::MouseButtonPressed )
				{
					MouseEvent e;
					e.x = ev.mouseButton.x;
					e.y = ev.mouseButton.y;

					if ( ev.mouseButton.button == sf::Mouse::Left )
					{
						e.action = LEFT_BUTTON_PRESSED;
						mIsLeftMouseButtonDown = true;
					}
					else if ( ev.mouseButton.button == sf::Mouse::Right )
					{
						e.action = RIGHT_BUTTON_PRESSED;
						mIsRightMouseButtonDown = true;
					}
					else if ( ev.mouseButton.button == sf::Mouse::Middle )
					{
						e.action = MIDDLE_BUTTON_PRESSED;
						mIsMiddleMouseButtonDown = true;
					}

					mMouseEvent.notify( e );
				}
				else if ( ev.type == sf::Event::MouseButtonReleased )
				{
					MouseEvent e;
					e.x = ev.mouseButton.x;
					e.y = ev.mouseButton.y;

					if ( ev.mouseButton.button == sf::Mouse::Left )
					{
						e.action = LEFT_BUTTON_RELEASED;
						mIsLeftMouseButtonDown = false;
					}
					else if ( ev.mouseButton.button == sf::Mouse::Right )
					{
						e.action = RIGHT_BUTTON_RELEASED;
						mIsRightMouseButtonDown = false;
					}
					else if ( ev.mouseButton.button == sf::Mouse::Middle )
					{
						e.action = MIDDLE_BUTTON_RELEASED;
						mIsMiddleMouseButtonDown = false;
					}

					mMouseEvent.notify( e );
				}
				else if ( ev.type == sf::Event::MouseMoved )
				{
					MouseEvent e;
					e.action = MOVED;
					e.x = ev.mouseMove.x;
					e.y = ev.mouseMove.y;

					mMouseEvent.notify( e );

					if ( mIsRightMouseButtonDown )
					{
						MouseEvent drag;
						drag.action = RIGHT_DRAG;
						drag.x = ev.mouseMove.x;
						drag.y = ev.mouseMove.y;
						mMouseEvent.notify( drag );
					}
					if ( mIsLeftMouseButtonDown )
					{
						MouseEvent drag;
						drag.action = LEFT_DRAG;
						drag.x = ev.mouseMove.x;
						drag.y = ev.mouseMove.y;
						mMouseEvent.notify( drag );
					}
					if ( mIsMiddleMouseButtonDown )
					{
						MouseEvent drag;
						drag.action = MIDDLE_DRAG;
						drag.x = ev.mouseMove.x;
						drag.y = ev.mouseMove.y;
						mMouseEvent.notify( drag );
					}
				}
				else if ( ev.type == sf::Event::MouseWheelMoved )
				{
					MouseEvent e;
					if ( ev.mouseWheel.delta > 0 ) e.action = WHEEL_UP;
					else e.action = WHEEL_DOWN;
					e.x = ev.mouseWheel.x;
					e.y = ev.mouseWheel.y;

					mMouseEvent.notify( e );
				}
				else if ( ev.type == sf::Event::MouseEntered )
				{
				}
				else if ( ev.type == sf::Event::MouseLeft )
				{
				}
				else if ( ev.type == sf::Event::KeyPressed )
				{
					KeyEvent k;
					k.wasPressed = true;
					k.key = getKey( ev );
					mKeyEvent.notify( k );
				}
				else if ( ev.type == sf::Event::KeyReleased )
				{
					KeyEvent k;
					k.wasPressed = false;
					k.key = getKey( ev );
					mKeyEvent.notify( k );
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