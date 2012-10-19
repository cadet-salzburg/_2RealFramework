#pragma once

#include "gl.h"
#include "Datatypes.h"

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

		//class PixelBuffers
		//{
		//public:

		//	struct Info
		//	{
		//		bool			isTexture;		// texture or buffer?
		//		unsigned int	width;
		//		unsigned int	height;
		//		GLenum			format;
		//		GLenum			datatype;
		//		//....
		//	};

		//	struct PBO
		//	{
		//		gl::Buffer		buffer;
		//		Info			info;
		//	};

		//	PixelBuffers( const unsigned int sz ) : size( sz ), buffers( sz, PBO() )
		//	{
		//		current = buffers.begin();
		//	}

		//	std::vector< PBO >				buffers;
		//	std::vector< PBO >::iterator	current;
		//	unsigned int					const size;
		//};

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

		class GlRessourceManager
		{

		public:

			GlRessourceManager( RenderSettings const& settings );
			~GlRessourceManager();

			GlewContext const& getGlewContext() const { return m_GlewContext; }

			void setActive( const bool active ) { m_SfContext.setActive( active ); }

			void cleanUp();

			Texture createTexture( const GLenum target );
			Buffer createBuffer( const GLenum usageHint );

			void destroyTexture( TextureObj &texture );
			void destroyBuffer( BufferObj &buffer );

			typedef std::pair< std::string, GLenum > ShaderSource;
			typedef std::list< std::string, GLenum > ShaderSources;

			Program createProgram( ShaderSources const& src );
			void destroyProgram( ProgramObj &program );

		private:

			Poco::FastMutex				m_Mutex;
			RenderSettings				m_CreationSettings;
			GlewContext					m_GlewContext;
			sf::Context					m_SfContext;
			std::list< TextureObj * >	m_Textures;
			std::list< BufferObj * >	m_Buffers;
			std::list< GLuint	>		m_DeletedTextures;
			std::list< GLuint	>		m_DeletedBuffers;

		};

		class BufferObj
		{

		public:

			BufferObj( GlRessourceManager &mgr, const GLuint handle, const GLenum usageHint ) :
				m_Manager( mgr ), m_Handle( handle ), m_IsValid( true ),
				m_Size( 0 ), m_ElementCount( 0 ), m_UsageHint( usageHint )
			{
			}
			
			~BufferObj()
			{
				if ( m_IsValid )
				{
					m_Manager.destroyBuffer( *this );
				}
			}

			void invalidate() { m_IsValid = false; }

			bool isValid() const { return m_IsValid; }
			bool isEmpty() const { return m_Size == 0; }

			const GLenum getDatatype() const { return m_Datatype; }

		private:

			friend class GlRessourceManager;
			friend class Renderer;
			friend class BufferTarget;
			friend class TextureUnit;

			GlRessourceManager		&m_Manager;
			GLuint					m_Handle;
			bool					m_IsValid;

			size_t					m_Size;
			unsigned int			m_ElementCount;
			GLenum					m_Datatype;
			GLenum					m_UsageHint;

		};

		class BufferTarget
		{

		public:

			BufferTarget( const GLenum target, GlewContext &glewContext ) :
				mGlewContext( glewContext ),
				mBufferTarget( target )
			{
			}

			GlewContext const& getGlewContext() const { return mGlewContext; }

			template< typename T >
			void updateBuffer( Buffer &buffer, T const*const data, const size_t size, const GLenum usageHint ) const
			{
				bool createNewDataStore = false;

				unsigned int elementCount = size / sizeof( T );
				GLenum datatype = getGLEnumeration< T >();

				if ( buffer->m_Size != size || buffer->m_UsageHint != usageHint )
				{
					createNewDataStore = true;
				}

				buffer->m_Datatype = datatype;
				buffer->m_Size = size;
				buffer->m_ElementCount = elementCount;
				buffer->m_UsageHint = usageHint;

				glBindBuffer( mBufferTarget, buffer->m_Handle );

				if ( createNewDataStore )	glBufferData( mBufferTarget, size, data, usageHint );
				else						glBufferSubData( mBufferTarget, 0, size, data );

				glBindBuffer( mBufferTarget, 0 );
			}

			template< typename T >
			void updateBuffer( Buffer &buffer, typename std::vector< T >::const_iterator const& start, typename std::vector< T >::const_iterator const& end, const GLenum usageHint ) const
			{
				bool createNewDataStore = false;

				unsigned int elementCount = end - start;
				size_t size = elementCount * sizeof( T );
				GLenum datatype = getGLEnumeration< T >();

				if ( buffer->m_Size != size || buffer->m_UsageHint != usageHint )
				{
					createNewDataStore = true;
				}

				buffer->m_Datatype = datatype;
				buffer->m_Size = size;
				buffer->m_ElementCount = elementCount;
				buffer->m_UsageHint = usageHint;

				glBindBuffer( mBufferTarget, buffer->m_Handle );

				if ( createNewDataStore )	glBufferData( mBufferTarget, size, &( *start ), usageHint );
				else						glBufferSubData( mBufferTarget, 0, size, &( *start ) );

				glBindBuffer( mBufferTarget, 0 );
			}

			void bind( Buffer const& buffer )
			{
				glBindBuffer( mBufferTarget, buffer->m_Handle );
			}

			void reset()
			{
				glBindBuffer( mBufferTarget, 0 );
			}

		private:

			GlewContext		mGlewContext;
			GLenum			mBufferTarget;

		};

		class TextureObj
		{

		public:

			struct Settings
			{
				Settings()
				{
					wrapS = GL_REPEAT;
					wrapT = GL_REPEAT;
					wrapR = GL_REPEAT;
					minFilter = GL_LINEAR;
					magFilter = GL_LINEAR_MIPMAP_LINEAR;
				}

				bool operator==( Settings const& other ) const
				{
					return ( ( wrapS == other.wrapS ) && ( wrapT == other.wrapT ) && ( wrapR == other.wrapR ) &&
								( minFilter == other.minFilter ) && ( magFilter == other.magFilter ) );
				}

				bool operator!=( Settings const& other ) const
				{
					return ( !( *this == other ) );
				}

				GLenum		wrapS;
				GLenum		wrapT;
				GLenum		wrapR;
				GLenum		minFilter;
				GLenum		magFilter;
			};

			TextureObj( GlRessourceManager &mgr, const GLuint handle, const GLenum target ) :
				m_Manager( mgr ), m_Handle( handle ), m_IsValid( true ),
				m_Width( 0 ), m_Height( 0 ), m_TextureTarget( target ), m_InternalFormat( GL_RGBA )
			{
			}

			~TextureObj()
			{
				if ( m_IsValid )
				{
					m_Manager.destroyTexture( *this );
				}
			}

			void invalidate() { m_IsValid = false; }
			bool isValid() const { return m_IsValid; }
			bool isEmpty() const { return ( m_Width == 0 || m_Height == 0 ); }

			// helper function, returns opengl data format ( incomplete! )
			static GLenum getTextureFormat( const ImageChannelOrder order );

		private:

			friend class GlRessourceManager;
			friend class Renderer;
			friend class OffScreenRenderer;
			friend class TextureUnit;

			GlRessourceManager		&m_Manager;
			GLuint					m_Handle;
			bool					m_IsValid;

			GLenum					m_TextureTarget;
			unsigned int			m_Width;
			unsigned int			m_Height;
			GLenum					m_Format;
			GLenum					m_Datatype;
			GLenum					m_InternalFormat;
			Settings				m_Settings;

		};

		class TextureUnit
		{

		public:

			TextureUnit( const unsigned int unit, GlewContext &glewContext ) :
				m_GlewContext( glewContext ),
				m_TextureUnit( unit ),
				m_ActiveTarget( GL_TEXTURE_2D )
			{
			}

			GlewContext const& getGlewContext() const { return m_GlewContext; }

			void set( Texture const& texture )
			{
				m_ActiveTarget = texture->m_TextureTarget;
	
				glActiveTexture( GL_TEXTURE0 + m_TextureUnit );
				glBindTexture( m_ActiveTarget, texture->m_Handle );
			}
	
			void reset()
			{
				glBindTexture( m_ActiveTarget, 0 );
				glDisable( m_ActiveTarget );
			}

			void resizeTexture( Texture &tex, const GLenum i, const unsigned int w, const unsigned int h, TextureObj::Settings const& s ) const
			{
				if ( tex->m_TextureTarget != GL_TEXTURE_2D )
				{
					throw std::exception( "texture object is not 2d!" );
				}

				tex->m_Width = w;
				tex->m_Height = h;
				tex->m_InternalFormat = i;

				glActiveTexture( GL_TEXTURE0 + m_TextureUnit );
				glBindTexture( GL_TEXTURE_2D, tex->m_Handle );
				
				glTexImage2D( GL_TEXTURE_2D, 0, i, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr );

				if ( tex->m_Settings != s )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s.wrapS );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s.wrapT );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, s.wrapR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s.minFilter );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s.magFilter );
					tex->m_Settings = s;
				}

				glGenerateMipmap( GL_TEXTURE_2D );
				glBindTexture( GL_TEXTURE_2D, 0 );		// not sure if this is necessary
			}

			//void updateTextureAsynchronously( Texture &tex, const GLenum i, const GLenum f, const GLenum t, const unsigned int w, const unsigned int h, TextureObj::Settings const& s ) const
			//{
			//	if ( tex->m_TextureTarget != GL_TEXTURE_2D )
			//	{
			//		throw std::exception( "texture object is not 2d!" );
			//	}

			//	bool createNewDataStore = false;
			//	if ( w != tex->m_Width || h != tex->m_Height || f != tex->m_Format || i != tex->m_InternalFormat )
			//	{
			//		createNewDataStore = true;
			//	}

			//	// TODO: helper struct for tex data
			//	tex->m_Width = w;
			//	tex->m_Height = h;
			//	tex->m_Format = f;
			//	tex->m_InternalFormat = i;
			//	tex->m_Datatype = t;

			//	glActiveTexture( GL_TEXTURE0 + m_TextureUnit );
			//	glBindTexture( GL_TEXTURE_2D, tex->m_Handle );
			//	
			//	if ( createNewDataStore )		glTexImage2D( GL_TEXTURE_2D, 0, i, w, h, 0, f, t, 0 );
			//	else							glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, f, t, 0 );

			//	if ( tex->m_Settings != s )
			//	{
			//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s.wrapS );
			//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, s.wrapT );
			//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, s.wrapR );
			//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, s.minFilter );
			//		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, s.magFilter );
			//		tex->m_Settings = s;
			//	}

			//	// TODO: gen mipmaps based on settings!
			//	glGenerateMipmap( GL_TEXTURE_2D );
			//	glBindTexture( GL_TEXTURE_2D, 0 );		// not sure if this is necessary
			//}

			void updateTexture( Texture &texture, Image const& img, const GLenum internalFormat, TextureObj::Settings const& settings ) const
			{
				if ( texture->m_TextureTarget != GL_TEXTURE_2D )
				{
					throw std::exception( "texture object is not 2d!" );
				}

				GLenum format = TextureObj::getTextureFormat( img.getChannelOrder() );
				unsigned int w = img.getWidth();
				unsigned int h = img.getHeight();

				bool createNewDataStore = false;
				if ( w != texture->m_Width || h != texture->m_Height || format != texture->m_Format || internalFormat != texture->m_InternalFormat )
				{
					createNewDataStore = true;
				}

				texture->m_Width = w;
				texture->m_Height = h;
				texture->m_Format = format;
				texture->m_InternalFormat = internalFormat;

				if ( img.getImageType() == ImageType::UNSIGNED_BYTE )			texture->m_Datatype = GL_UNSIGNED_BYTE;
				else if ( img.getImageType() == ImageType::UNSIGNED_SHORT )		texture->m_Datatype = GL_UNSIGNED_SHORT;
				else if ( img.getImageType() == ImageType::DOUBLE )				texture->m_Datatype = GL_DOUBLE;
				else															texture->m_Datatype = GL_FLOAT;

				glActiveTexture( GL_TEXTURE0 + m_TextureUnit );
				glBindTexture( GL_TEXTURE_2D, texture->m_Handle );

				if ( createNewDataStore )
				{
					glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, img.getWidth(), img.getHeight(), 0, format, texture->m_Datatype, static_cast< GLvoid const* >( img.getData() ) );
				}
				else
				{
					glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, format, texture->m_Datatype, img.getData() );
				}

				if ( texture->m_Settings != settings )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrapS );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrapT );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, settings.wrapR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.minFilter );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.magFilter );

					texture->m_Settings = settings;
				}

				glGenerateMipmap( GL_TEXTURE_2D );
				glBindTexture( GL_TEXTURE_2D, 0 );
			}

			template< typename T >
			void updateTexture( Texture &texture, ImageT< T > const& img, const GLenum internalFormat, TextureObj::Settings const& settings ) const
			{
				if ( texture->m_TextureTarget != GL_TEXTURE_2D )
				{
					throw std::exception( "texture object is not 2d!" );
				}

				GLenum format = TextureObj::getTextureFormat( img.getChannelOrder() );
				unsigned int w = img.getWidth();
				unsigned int h = img.getHeight();

				bool createNewDataStore = false;
				if ( w != texture->m_Width || h != texture->m_Height || format != texture->m_Format || internalFormat != texture->m_InternalFormat )
				{
					createNewDataStore = true;
				}

				texture->m_Width = w;
				texture->m_Height = h;
				texture->m_Format = format;
				texture->m_InternalFormat = internalFormat;
				texture->m_Datatype = getGLEnumeration< T >();

				glActiveTexture( GL_TEXTURE0 + m_TextureUnit );
				glBindTexture( GL_TEXTURE_2D, texture->m_Handle );

				if ( createNewDataStore )
				{
					glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, img.getWidth(), img.getHeight(), 0, format, texture->m_Datatype, img.getData() );
				}
				else
				{
					glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, w, h, format, texture->m_Datatype, img.getData() );
				}

				if ( texture->m_Settings != settings )
				{
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, settings.wrapS );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, settings.wrapT );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, settings.wrapR );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, settings.minFilter );
					glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, settings.magFilter );

					texture->m_Settings = settings;
				}

				glGenerateMipmap( GL_TEXTURE_2D );

				glBindTexture( GL_TEXTURE_2D, 0 );
			}

		private :

			GlewContext				m_GlewContext;
			unsigned int			m_TextureUnit;
			GLenum					m_ActiveTarget;

		};

		class Renderer
		{

		public:

			Renderer();
			GlewContext const& getGlewContext() const { return m_GlewContext; }

			void setViewport( const unsigned int w, const unsigned int h )
			{
				glViewport( 0, 0, w, h );
			}

			void setProgram( std::string const& name, std::string const& vertexSrc, std::string const& fragmentSrc, std::string const& geometrySrc = "" )
			{
				ProgramObj *prog = new ProgramObj( m_GlewContext );

				ShaderObj vert( GL_VERTEX_SHADER, m_GlewContext, vertexSrc, false );
				ShaderObj frag( GL_FRAGMENT_SHADER, m_GlewContext, fragmentSrc, false );

				std::cout << vert.getShaderInfo() << "\n" << frag.getShaderInfo() << std::endl;

				prog->attachShader( vert );
				prog->attachShader( frag );

				if ( !geometrySrc.empty() )
				{
					ShaderObj geo( GL_GEOMETRY_SHADER, m_GlewContext, geometrySrc, false );
					std::cout << geo.getShaderInfo() << std::endl;
					prog->attachShader( geo );
				}

				prog->link();
				prog->validate();

				std::cout << prog->getProgramInfo() << std::endl;

				m_Programs[ name ] = prog;
			}

			//void updateTextureAsync( Texture &texture, Image const& img, PixelBuffers &pbos, const GLenum internalFormat, TextureObj::Settings const& settings = TextureObj::Settings() )
			//{
			//	GLenum datatype;
			//	if ( img.getImageType() == ImageType::UNSIGNED_BYTE )			datatype = GL_UNSIGNED_BYTE;
			//	else if ( img.getImageType() == ImageType::UNSIGNED_SHORT )		datatype = GL_UNSIGNED_SHORT;
			//	else if ( img.getImageType() == ImageType::DOUBLE )				datatype = GL_DOUBLE;
			//	else															datatype = GL_FLOAT;

			//	m_BufferTargets[ UPLOAD_BUFFER ].bind( pbos.current->buffer );

			//	m_TextureUnits[0].updateTextureAsynchronously( texture, internalFormat, pbos.current->info.format, pbos.current->info.datatype, pbos.current->info.width, pbos.current->info.height, settings );

			//	++pbos.current;
			//	if ( pbos.current == pbos.buffers.end() )
			//	{
			//		pbos.current = pbos.buffers.begin();
			//	}

			//	m_BufferTargets[ UPLOAD_BUFFER ].updateBuffer( pbos.current->buffer, img.getData(), img.getByteSize(), GL_STREAM_DRAW );

			//	pbos.current->info.isTexture = true;
			//	pbos.current->info.datatype = datatype;
			//	pbos.current->info.format = TextureObj::getTextureFormat( img.getChannelOrder() );
			//	pbos.current->info.width = img.getWidth();
			//	pbos.current->info.height = img.getHeight();
			//}

			//template< typename T >
			//void updateTextureAsync( Texture &texture, ImageT< T > const& img, PixelBuffers &pbos, const GLenum internalFormat, TextureObj::Settings const& settings = TextureObj::Settings() )
			//{
			//	GLenum datatype = getGLEnumeration< T >();

			//	m_BufferTargets[ UPLOAD_BUFFER ].bind( pbos.current->buffer );

			//	m_TextureUnits[0].updateTextureAsynchronously( texture, internalFormat, pbos.current->info.format, pbos.current->info.datatype, pbos.current->info.width, pbos.current->info.height, settings );

			//	++pbos.current;
			//	if ( pbos.current == pbos.buffers.end() )
			//	{
			//		pbos.current = pbos.buffers.begin();
			//	}

			//	m_BufferTargets[ UPLOAD_BUFFER ].updateBuffer( pbos.current->buffer, img.getData(), img.getWidth()*img.getHeight()*img.getNumberOfChannels()*sizeof( T ), GL_STREAM_DRAW );

			//	pbos.current->info.isTexture = true;
			//	pbos.current->info.datatype = datatype;
			//	pbos.current->info.format = TextureObj::getTextureFormat( img.getChannelOrder() );
			//	pbos.current->info.width = img.getWidth();
			//	pbos.current->info.height = img.getHeight();
			//}

			void updateTexture( Texture &texture, Image const& img, const GLenum internalFormat, TextureObj::Settings const& settings = TextureObj::Settings() )
			{
				m_TextureUnits[0].updateTexture( texture, img, internalFormat, settings );
			}

			template< typename T >
			void updateTexture( Texture &texture, ImageT< T > const& img, const GLenum internalFormat, TextureObj::Settings const& settings = TextureObj::Settings() )
			{
				m_TextureUnits[0].updateTexture< T >( texture, img, internalFormat, settings );
			}

			void resizeTexture( Texture &texture, const GLenum internalFormat, const unsigned int w, const unsigned int h, TextureObj::Settings const& settings = TextureObj::Settings() )
			{
				m_TextureUnits[0].resizeTexture( texture, internalFormat, w, h, settings );
			}

			template< typename T >
			void updateBuffer( Buffer &buffer, std::vector< T > const& data )
			{
				m_BufferTargets[ ARRAY_BUFFER ].updateBuffer< T >( buffer, data.begin(), data.end(), buffer->m_UsageHint );
			}

			void finish()
			{
				glFinish();
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

			void render( gl::RenderData const& data, std::string const& program = "default" )
			{
				glEnable( GL_BLEND );
				glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

				glPointSize( 2.0 );
				glEnable( GL_POINT_SMOOTH );
				glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );

				ProgramObj *p = m_Programs[ program ];
				p->bind();

				std::map< std::string, gl::RenderData::TextureBinding > const& textures = data.m_Textures;
				std::map< std::string, gl::RenderData::VertexAttribute > const& attribs = data.m_Attributes;

				unsigned int currentUnit = 0;
				for ( std::map< std::string, RenderData::TextureBinding >::const_iterator it = textures.begin(); it != textures.end(); ++it )
				{
					RenderData::TextureBinding const& tex = it->second;

					tex.unit = currentUnit;
					m_TextureUnits[ currentUnit ].set( tex.texture );

					p->setUniformTexture( it->first, currentUnit );

					currentUnit++;
					if ( currentUnit >= m_TextureUnits.size() )
					{
						std::cout << "trying to bind more textures than available texunits!" << std::endl;
						break;
					}
				}

				for ( std::map< std::string, RenderData::VertexAttribute >::const_iterator it = attribs.begin(); it != attribs.end(); ++it )
				{
					RenderData::VertexAttribute const& attrib = it->second;
					m_BufferTargets[ ARRAY_BUFFER ].bind( attrib.buffer );

					attrib.index = p->getAttributeLocation( it->first );

					glVertexAttribPointer( attrib.index, attrib.size, attrib.buffer->m_Datatype, attrib.normalized, attrib.stride, static_cast< GLvoid const* >( nullptr ) );
					glEnableVertexAttribArray( attrib.index );
				}

				if ( data.m_DrawIndexed )
				{
				}
				else
				{
					glDrawArrays( data.m_PrimitiveType, 0, data.m_ElementCount );
				}

				for ( std::map< std::string, RenderData::VertexAttribute >::const_iterator it = attribs.begin(); it != attribs.end(); ++it )
				{
					glDisableVertexAttribArray( it->second.index );
				}

				for ( std::map< std::string, RenderData::TextureBinding >::const_iterator it = textures.begin(); it != textures.end(); ++it )
				{
					m_TextureUnits[ it->second.unit ].reset();
				}

				glUseProgram( 0 );

				glDisable( GL_BLEND );
			}

		protected:

			GlewContext							m_GlewContext;
			std::map< std::string, ProgramObj* >	m_Programs;
			std::vector< TextureUnit >			m_TextureUnits;
			std::vector< BufferTarget >			m_BufferTargets;

		};

		/**
		*	TODO: resize!!!
		*/
		class OffScreenRenderer : public Renderer
		{

		public:

			OffScreenRenderer( const unsigned int w, const unsigned int h ) : Renderer(), m_Width( w ), m_Height( h )
			{
				glGenFramebuffers( 1, &m_Framebuffer );
				glGenRenderbuffers( 1, &m_Renderbuffer );
				glBindRenderbuffer( GL_RENDERBUFFER, m_Renderbuffer );
				glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h );
				glBindRenderbuffer( GL_RENDERBUFFER, 0 );
				glBindFramebuffer( GL_FRAMEBUFFER, m_Framebuffer );
				glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_Renderbuffer );
			}

			void attachTexture( Texture &tex )
			{
				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->m_Handle, 0 );
			}

			void detachTexture( Texture &tex )
			{
				glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0 );
			}

			void checkStatus()
			{
				GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
				if ( status != GL_FRAMEBUFFER_COMPLETE )
				{
					std::cout << "framebuffer is incomplete!" << std::endl;
				}
			}

			~OffScreenRenderer()
			{
				glBindFramebuffer( GL_FRAMEBUFFER, 0 );
				glDeleteRenderbuffers( 1, &m_Renderbuffer );
				glDeleteFramebuffers( 1, &m_Framebuffer );
			}

		private:

			GLuint			m_Framebuffer;
			GLuint			m_Renderbuffer;
			GLuint			m_Texture;
			unsigned int	m_Width;
			unsigned int	m_Height;

		};

		class RenderWindow
		{

		public:

			RenderWindow( RenderSettings const& settings );

			void setActive( const bool active ) { m_SfWindow.setActive( active ); }
			void display() { m_SfWindow.display(); }

			void setTitle( std::string const& title ) { m_SfWindow.setTitle( title ); }
			void processEvents();

			Renderer & getRenderer() { return m_Renderer; }

		private:

			RenderSettings			m_CreationSettings;
			sf::Window				m_SfWindow;
			sf::ContextSettings		m_SfSettings;
			Renderer				m_Renderer;

		};

		class RenderContext
		{

		public:

			RenderContext( RenderSettings const& settings );

			void setActive( const bool active ) { m_SfContext.setActive( active ); }

			OffScreenRenderer & getRenderer() { return m_Renderer; }

		private:

			RenderSettings			m_CreationSettings;
			sf::Context				m_SfContext;
			OffScreenRenderer		m_Renderer;

		};
	}
}