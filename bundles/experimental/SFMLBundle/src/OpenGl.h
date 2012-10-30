#pragma once

#include "gl.h"
#include "Datatypes.h"
#include "RessourceManager.h"

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

		struct RenderSettings
		{
			std::string		title;
			unsigned int	width;
			unsigned int	height;
			unsigned int	glMajor;
			unsigned int	glMinor;
			unsigned int	depthBits;
			unsigned int	stencilBits;
			unsigned int	aaSamples;
			unsigned int	colorBits;
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

			template< typename T >
			void updateBuffer( BufferObj *& buffer, std::vector< T > const& data, const GLenum usageHint )
			{
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
				if ( imageType == ImageType::UNSIGNED_BYTE )		type = GL_UNSIGNED_BYTE;
				else if ( imageType == ImageType::UNSIGNED_SHORT )	type = GL_UNSIGNED_SHORT;
				else if ( imageType == ImageType::FLOAT )			type = GL_FLOAT;
				else if ( imageType == ImageType::DOUBLE )			type = GL_DOUBLE;

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

				glPointSize( 1.5f );
				glEnable( GL_POINT_SMOOTH );
				glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );

				glLineWidth( 0.5f );
				glEnable( GL_LINE_SMOOTH );
				glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );

				if ( data.mProgram.get() == nullptr ) return;

				glUseProgram( data.mProgram->mHandle );

				for ( RenderData::Attributes::const_iterator it = data.mAttributes.begin(); it != data.mAttributes.end(); ++it )
				{
					RenderData::VertexAttribute attrib = it->second;
					glBindBuffer( GL_ARRAY_BUFFER, attrib.buffer->mHandle );
					// TODO: attrib offset ( ? )
					glVertexAttribPointer( it->first, attrib.size, attrib.buffer->mDatatype, attrib.normalized, attrib.stride, nullptr );
					glEnableVertexAttribArray( it->first );
				}

				for ( RenderData::Textures::const_iterator it = data.mTextures.begin(); it != data.mTextures.end(); ++it )
				{
					Texture tex = it->second;
					glActiveTexture( GL_TEXTURE0 + it->first );
					glBindTexture( GL_TEXTURE_2D, tex->mHandle );
				}

				// TODO: indexed drawing ( ! ), offset into buffer ( ? )
				glDrawArrays( data.mPrimitiveType, 0, data.mElementCount );

				glUseProgram( 0 );
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

			RenderWindow( RenderSettings const& settings, RessourceManager const& mgr );

			void setActive( const bool active ) { mSfWindow.setActive( active ); }
			void display() { mSfWindow.display(); }
			void setTitle( std::string const& title ) { mSfWindow.setTitle( title ); }
			void processEvents();

			Renderer & getRenderer() { return mRenderer; }

		private:

			sf::Window				mSfWindow;
			sf::ContextSettings		mSfSettings;
			Renderer				mRenderer;

		};
	}
}