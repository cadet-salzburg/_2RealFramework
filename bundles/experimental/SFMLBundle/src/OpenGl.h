#pragma once

#include "gl.h"
#include "Datatypes.h"
#include "RessourceManager.h"

#include "helpers\_2RealCallback.h"
#include "helpers\_2RealEvent.h"

#include <map>

namespace _2Real
{
	namespace gl
	{

		template< typename T > GLenum getGLEnumeration();
		template< > inline GLenum getGLEnumeration< float >()			{ return GL_FLOAT; }
		template< > inline GLenum getGLEnumeration< double >()			{ return GL_DOUBLE; }
		template< > inline GLenum getGLEnumeration< char >()			{ return GL_BYTE; }
		template< > inline GLenum getGLEnumeration< unsigned char >()	{ return GL_UNSIGNED_BYTE; }
		template< > inline GLenum getGLEnumeration< short >()			{ return GL_SHORT; }
		template< > inline GLenum getGLEnumeration< unsigned short >()	{ return GL_UNSIGNED_SHORT; }
		template< > inline GLenum getGLEnumeration< int >()				{ return GL_INT; }
		template< > inline GLenum getGLEnumeration< unsigned int >()	{ return GL_UNSIGNED_INT; }

		class Camera
		{
		public:
			static Mat4 lookAt( Vec3 const& eye, Vec3 const& target, Vec3 const& up )
			{
				Vec3 f = target - eye;
				f.normalize();

				Vec3 u = up;
				u.normalize();

				Vec3 s = f.cross( u );
				s.normalize();

				u = s.cross( f );

				float e0 = eye.dot( s );
				float e1 = eye.dot( u );
				float e2 = eye.dot( f );

				Mat4 result;
				result <<	s.x(),	u.x(),	-f.x(),	0, s.y(),	u.y(),	-f.y(),	0,
							s.z(),	u.z(),	-f.z(),	0, -e0,	-e1,	e2,	1.0f;

				return result;
			}

			static Mat4 perspective( const float fov, const float a, const float zn, const float zf )
			{
				float f = 1.0f /( tan( fov*M_PI/360.f ) );
				float s1 = ( zf+zn ) / ( zn-zf );
				float s2 = ( 2*zf*zn ) / ( zn-zf );

				Mat4 perspective;

				perspective <<	f/a,	0.0f,	0.0f,	0.0f,
								0.0f,	f,		0.0f,	0.0f,
								0.0f,	0.0f,	s1,		-1.0f,
								0.0f,	0.0f,	s2,		0.0f;

				return perspective;
			}
		};

		class Context
		{

		public:

			Context( RenderSettings const& settings, RessourceManager const& mgr );

			GlewContext const& getGlewContext() const { return mGlewContext; }

			void setActive( const bool active ) { mSfContext.setActive( active ); }
			void finish() { glFinish(); }
			void flush() { glFlush(); }

			void useProgram( ProgramObj *p )
			{
				glUseProgram( p->mHandle );
			}

			void resetProgram()
			{
				glUseProgram( 0 );
			}

			void setUniformSampler( const GLint location, const int unit )
			{
				glUniform1i( location, unit );
			}

			void setUniformMat4( const GLint location, Mat4 const& mat )
			{
				glUniformMatrix4fv( location, 1, true, mat.data() );
			}

			void setUniformMat3( const GLint location, Mat3 const& mat )
			{
				glUniformMatrix3fv( location, 1, true, mat.data() );
			}

			void setUniformVec2( const GLint location, Vec2 const& vec )
			{
				glUniform2f( location, vec.x(), vec.y() );
			}

			template< typename T >
			void updateBuffer( BufferObj *& buffer, std::vector< T > const& data, const GLenum usageHint )
			{
				if ( data.empty() ) return;

				const GLenum t = getGLEnumeration< T >();
				const unsigned int e = data.size();
				const size_t s = e * sizeof( T );

				bool createNewStorage = ( ( t != buffer->mDatatype ) || ( e != buffer->mElementCount ) );

				glBindBuffer( buffer->mTarget, buffer->mHandle );
				if ( createNewStorage )		glBufferData( buffer->mTarget, data.size() * sizeof( T ), &data[ 0 ], usageHint );
				else						glBufferSubData( buffer->mTarget, 0, data.size() * sizeof( T ), &data[ 0 ] );
				glBindBuffer( buffer->mTarget, 0 );

				buffer->mDatatype = t;
				buffer->mElementCount = e;
				buffer->mSizeInBytes = s;
			}

			void updateBuffer( BufferObj *& buffer, Image const& img, const GLenum usageHint )
			{
				ImageType imageType = img.getImageType();
				GLenum t = GL_UNSIGNED_BYTE;
				if ( imageType == ImageType::UNSIGNED_BYTE )			t = GL_UNSIGNED_BYTE;
				else if ( imageType == ImageType::UNSIGNED_SHORT )		t = GL_UNSIGNED_SHORT;
				else if ( imageType == ImageType::FLOAT )				t = GL_FLOAT;
				else if ( imageType == ImageType::DOUBLE )				t = GL_DOUBLE;

				const unsigned int e = img.getWidth() * img.getHeight() * img.getNumberOfChannels();
				const size_t s = img.getByteSize();

				bool createNewStorage = ( ( t != buffer->mDatatype ) || ( e != buffer->mElementCount ) );

				glBindBuffer( buffer->mTarget, buffer->mHandle );
				if ( createNewStorage )		glBufferData( buffer->mTarget, s, img.getData(), usageHint );
				else						glBufferSubData( buffer->mTarget, 0, s, img.getData() );
				glBindBuffer( buffer->mTarget, 0 );

				buffer->mDatatype = t;
				buffer->mElementCount = e;
				buffer->mSizeInBytes = s;
			}

			template< typename T >
			void updateTexture( TextureObj *& texture, ImageT< T > const& img, const GLenum texTarget, TextureObj::Settings const& s = TextureObj::Settings() )
			{
				if ( texTarget != GL_TEXTURE_2D )
				{
					throw std::exception( "texture object is not 2d!" );
				}

				const GLenum type = getGLEnumeration< T >();
				const unsigned int w = img.getWidth();
				const unsigned int h = img.getHeight();
				const GLenum imageFormat = TextureObj::getTextureFormat( img.getChannelOrder() );

				bool createNewStorage = ( ( w != texture->mWidth ) || ( h != texture->mHeight ) || ( s.format != texture->mSettings.format ) );

				glBindTexture( GL_TEXTURE_2D, texture->mHandle );
				if ( createNewStorage )
				{
					glTexImage2D( GL_TEXTURE_2D, 0, s.format, w, h, 0, imageFormat, type, img.getData() );
				}
				else
				{
					glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, imageFormat, type, img.getData() );
				}

				if ( texture->mSettings != s )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s.wrapS );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s.wrapT );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, s.wrapR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s.minFilter );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s.magFilter );
					texture->mSettings = s;
				}

				texture->mWidth = w;
				texture->mHeight = h;
				texture->mFormat = imageFormat;
				texture->mDatatype = type;

				// TODO: depending on min filter
				glGenerateMipmap( GL_TEXTURE_2D );

				glBindTexture( GL_TEXTURE_2D, 0 );
			}

			void updateTexture( TextureObj *& texture, Image const& img, const GLenum texTarget, TextureObj::Settings const& s = TextureObj::Settings() )
			{
				if ( texTarget != GL_TEXTURE_2D )
				{
					throw std::exception( "texture object is not 2d!" );
				}

				const _2Real::ImageType imageType = img.getImageType();
				GLenum type = GL_UNSIGNED_BYTE;
				if ( imageType == ImageType::UNSIGNED_BYTE )			type = GL_UNSIGNED_BYTE;
				else if ( imageType == ImageType::UNSIGNED_SHORT )		type = GL_UNSIGNED_SHORT;
				else if ( imageType == ImageType::FLOAT )				type = GL_FLOAT;
				else if ( imageType == ImageType::DOUBLE )				type = GL_DOUBLE;

				const unsigned int w = img.getWidth();
				const unsigned int h = img.getHeight();
				const GLenum imageFormat = TextureObj::getTextureFormat( img.getChannelOrder() );

				bool createNewStorage = ( ( w != texture->mWidth ) || ( h != texture->mHeight ) || ( s.format != texture->mSettings.format ) );

				glBindTexture( GL_TEXTURE_2D, texture->mHandle );
				if ( createNewStorage )
				{
					glTexImage2D( GL_TEXTURE_2D, 0, s.format, w, h, 0, imageFormat, type, img.getData() );
				}
				else
				{
					glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, imageFormat, type, img.getData() );
				}

				if ( texture->mSettings != s )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s.wrapS );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s.wrapT );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, s.wrapR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s.minFilter );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s.magFilter );
					texture->mSettings = s;
				}

				texture->mWidth = w;
				texture->mHeight = h;
				texture->mFormat = imageFormat;
				texture->mDatatype = type;

				// TODO: depending on min filter
				glGenerateMipmap( GL_TEXTURE_2D );

				glBindTexture( GL_TEXTURE_2D, 0 );
			}

			void attachShader( ProgramObj * p, ShaderObj * s ) const
			{
				glAttachShader( p->mHandle, s->mHandle );
				p->mShaders.push_back( s );
			}

			void linkProgram( ProgramObj *p ) const;

			BufferObj * createBufferObj() const;
			ProgramObj * createProgramObj() const;
			ShaderObj * createShaderObj( const GLenum type, std::string const& src ) const;
			TextureObj *createTextureObj() const;

		protected:

			RessourceManager		const& mManager;
			GlewContext				mGlewContext;
			sf::Context				mSfContext;

		};

		class Renderer
		{

		public:

			Renderer( RessourceManager const& mgr );

			GlewContext const& getGlewContext() const { return mGlewContext; }

			void setViewport( const unsigned int w, const unsigned int h )
			{
				glViewport( 0, 0, w, h );
			}

			void render( RenderData const& data )
			{
				glEnable( GL_DEPTH_TEST );
				glDepthFunc( GL_LEQUAL );

				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

				glPointSize( 10.0f );
				glEnable( GL_POINT_SMOOTH );
				glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );

				glLineWidth( 3.0f );
				glEnable( GL_LINE_SMOOTH );
				glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

				if ( data.mProgram.get() == nullptr ) return;

				data.mProgram->mLock.readLock();
				glUseProgram( data.mProgram->mHandle );

				for ( RenderData::Attributes::const_iterator it = data.mAttributes.begin(); it != data.mAttributes.end(); ++it )
				{
					RenderData::VertexAttribute attrib = it->second;
					attrib.buffer->mLock.readLock();
					glBindBuffer( GL_ARRAY_BUFFER, attrib.buffer->mHandle );
					glVertexAttribPointer( it->first, attrib.size, attrib.buffer->mDatatype, attrib.normalized, attrib.stride, nullptr );
					glEnableVertexAttribArray( it->first );
				}

				for ( RenderData::Textures::const_iterator it = data.mTextures.begin(); it != data.mTextures.end(); ++it )
				{
					Texture tex = it->second;
					tex->mLock.readLock();
					glActiveTexture( GL_TEXTURE0 + it->first );
					glBindTexture( GL_TEXTURE_2D, tex->mHandle );
				}

				if ( data.mDrawIndexed )
				{
					data.mIndices->mLock.readLock();
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, data.mIndices->mHandle );
					glDrawElements( data.mPrimitiveType, data.mElementCount, data.mIndices->mDatatype, 0 );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
					data.mIndices->mLock.unlock();
				}
				else
				{
					glDrawArrays( data.mPrimitiveType, 0, data.mElementCount );
				}

				for ( RenderData::Attributes::const_iterator it = data.mAttributes.begin(); it != data.mAttributes.end(); ++it )
				{
					RenderData::VertexAttribute attrib = it->second;
					attrib.buffer->mLock.unlock();
				}

				for ( RenderData::Textures::const_iterator it = data.mTextures.begin(); it != data.mTextures.end(); ++it )
				{
					Texture tex = it->second;
					tex->mLock.unlock();
				}

				glUseProgram( 0 );
				data.mProgram->mLock.unlock();
			}

			void clear( Vec3 const& color, const float depth = 1.0f )
			{
				glClearColor( ( GLfloat )color.x(), ( GLfloat )color.y(), ( GLfloat )color.z(), 1.0f );
				glClearDepth( depth );

				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			}

			void clear( Vec4 const& color, const float depth = 1.0f )
			{
				glClearColor( ( GLfloat )color.x(), ( GLfloat )color.y(), ( GLfloat )color.z(), ( GLfloat )color.w() );
				glClearDepth( depth );

				glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
			}

		protected:

			GlewContext			mGlewContext;
			RessourceManager	const& mManager;

		};

		class RenderWindow
		{

		public:

			enum Key
			{
				A,
				R,
				Q,
				INVALID
			};

			RenderWindow( RenderSettings const& settings, RessourceManager const& mgr );

			void setActive( const bool active ) { mSfWindow.setActive( active ); }
			void display() { mSfWindow.display(); }
			void setTitle( std::string const& title ) { mSfWindow.setTitle( title ); }

			void processEvents();

			Renderer & getRenderer() { return mRenderer; }

			typedef _2Real::AbstractCallback< Key& >	KeyCallback;

			void registerToKeyPressed( KeyCallback &cb )
			{
				mKeyEvent.addListener( cb );
			}

			static Key getKey( sf::Event const& ev )
			{
				switch( ev.key.code )
				{
				case sf::Keyboard::R:
					return R;
				case sf::Keyboard::Q:
					return Q;
				case sf::Keyboard::A:
					return A;
				default:
					return INVALID;
				}
			}

		private:

			sf::Window				mSfWindow;
			sf::ContextSettings		mSfSettings;
			Renderer				mRenderer;
			bool					mIsMouseEnabled;
			bool					mIsKeyboardEnabled;

			_2Real::CallbackEvent< Key& >	mKeyEvent;

		};
	}
}