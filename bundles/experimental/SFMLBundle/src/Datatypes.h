#pragma once

#include "gl.h"
#include "RessourceManager.h"

namespace _2Real
{
	namespace gl
	{
		struct ShaderObj
		{
			ShaderObj( RessourceManager const& mgr ) : mManager( mgr ), mHandle( 0 ) {}
			~ShaderObj() { mManager.destroyShader( mHandle ); }

			RessourceManager			const& mManager;
			GLuint						mHandle;
			std::string					mSource;
			std::string					mInfoLog;
		};

		struct ProgramObj
		{
			struct ActiveVar
			{
				ActiveVar() : location( -1 ), size( 1 ), type( GL_FLOAT ) {}
				ActiveVar( const GLint l, const GLint sz, const GLenum t ) : location( l ), size( sz ), type( t ) {}

				GLint		location;
				GLint		size;
				GLenum		type;
			};

			ProgramObj( RessourceManager const& mgr ) : mManager( mgr ), mHandle( 0 ), mIsLinked( false ) {}
			~ProgramObj()
			{
				for ( std::list< ShaderObj * >::iterator it = mShaders.begin(); it != mShaders.end(); ++it ) delete ( *it );
				mManager.destroyProgram( mHandle );
			}

			typedef std::map< std::string, ActiveVar >		ActiveVars;

			RessourceManager			const& mManager;
			GLuint						mHandle;
			std::string					mInfoLog;
			std::list< ShaderObj * >	mShaders;
			ActiveVars					mActiveUniforms;
			ActiveVars					mActiveAttributes;
			bool						mIsLinked;

		};

		struct TextureObj
		{
			struct Settings
			{
				Settings() : wrapS( GL_REPEAT ), wrapT( GL_REPEAT ), wrapR( GL_REPEAT ), minFilter( GL_LINEAR ), magFilter( GL_LINEAR_MIPMAP_LINEAR ), format( GL_RGBA ) {}
				bool operator==( Settings const& o ) const { return ( ( wrapS == o.wrapS ) && ( wrapT == o.wrapT ) && ( wrapR == o.wrapR ) && ( minFilter == o.minFilter ) && ( magFilter == o.magFilter ) && ( format == o.format ) ); }
				bool operator!=( Settings const& o ) const { return ( !( *this == o ) ); }

				GLenum		wrapS;
				GLenum		wrapT;
				GLenum		wrapR;
				GLenum		minFilter;
				GLenum		magFilter;
				GLenum		format;
			};

			TextureObj( RessourceManager const& mgr ) : mManager( mgr ), mHandle( 0 ), mTarget( GL_TEXTURE_2D ), mWidth( 0 ), mHeight( 0 ), mDatatype( GL_FLOAT ), mFormat( GL_RGBA ) {}
			~TextureObj() { mManager.destroyTexture( mHandle ); }

			bool isEmpty() const { return ( mWidth == 0 || mHeight == 0 ); }

			RessourceManager		const& mManager;
			GLuint					mHandle;
			GLenum					mTarget;
			unsigned int			mWidth;
			unsigned int			mHeight;
			GLenum					mDatatype;
			GLenum					mFormat;
			Settings				mSettings;

			static GLenum getTextureFormat( const ImageChannelOrder order )
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
		};

		struct BufferObj
		{
			BufferObj( RessourceManager const& mgr ) : mManager( mgr ), mHandle( 0 ), mSizeInBytes( 0 ), mElementCount( 0 ), mDatatype( GL_FLOAT ), mTarget( GL_ARRAY_BUFFER ) {}
			~BufferObj() { mManager.destroyBuffer( mHandle ); }

			RessourceManager		const& mManager;		// ressource manager ( for deletion )
			GLuint					mHandle;				// handle
			size_t					mSizeInBytes;			// size of data store in bytes
			unsigned int			mElementCount;			// nr of elements in data store
			GLenum					mDatatype;				// type of elements
			GLenum					mTarget;				// buffer target
		};

		// TODO
		struct UniformValue
		{
			bool operator==( UniformValue const& other ) const
			{
				return ( mName == other.mName );
			}

			std::string			mName;
			int					mSampler;
			int					mInt;
			unsigned int		mUint;
			float				mFloat;
			//Vec2				mVec2;
			//Vec3				mVec3;
			//Vec4				mVec4;
			//Mat3				mMat3;
			//Mat4				mMat4;
		};

		struct RenderData
		{

			struct VertexAttribute
			{
				VertexAttribute() : buffer( nullptr ), size( 0 ), stride( 0 ), normalized( false ) {}

				VertexAttribute( gl::Buffer const& b, const unsigned int sz, const size_t s, const bool n ) :
					buffer( b ), size( sz ), stride( s ), normalized( n )
				{
				}

				gl::Buffer			buffer;
				GLuint				size;
				//GLuint			offset;
				GLsizei				stride;
				GLboolean			normalized;
			};

			//RenderData() : m_PrimitiveType( GL_POINTS ), m_ElementCount( 0 ), m_DrawIndexed( false ) {}
			RenderData() {}

			//RenderData( RenderData const& src ) :
			//	m_Textures( src.m_Textures ),
			//	m_Attributes( src.m_Attributes ),
			//	m_PrimitiveType( src.m_PrimitiveType ),
			//	m_ElementCount( src.m_ElementCount ),
			//	m_DrawIndexed( src.m_DrawIndexed )
			//{
			//}
			//RenderData( RenderData const& src ) :
			//	mTextures( src.mTextures ),
			//	mVertexArray( src.mVertexArray ),
			//	mProgram( src.mProgram )
			//{
			//}

			//RenderData& operator=( RenderData const& src )
			//{
			//	if ( this == &src ) return *this;

			//	m_Textures = src.m_Textures;
			//	m_Attributes = src.m_Attributes;
			//	m_PrimitiveType = src.m_PrimitiveType;
			//	m_ElementCount = src.m_ElementCount;
			//	m_DrawIndexed = src.m_DrawIndexed;

			//	return *this;
			//}
			//RenderData& operator=( RenderData const& src )
			//{
			//	if ( this == &src ) return *this;
			//	mTextures = src.mTextures;
			//	mVertexArray = src.mVertexArray;
			//	mProgram = src.mProgram;
			//	return *this;
			//}

			bool operator==( RenderData const& other ) const { return false; }

			void addAttribute( const unsigned int index, VertexAttribute const& attribute )
			{
				mAttributes[ index ] = attribute;
			}

			void addTexture( const unsigned int unit, Texture const& texture )
			{
				mTextures[ unit ] = texture;
			}

			typedef std::map< GLuint, VertexAttribute >		Attributes;
			typedef std::map< GLuint, Texture >				Textures;

			Program				mProgram;
			Attributes			mAttributes;
			Textures			mTextures;

			GLenum				mPrimitiveType;
			unsigned int		mElementCount;
			bool				mDrawIndexed;

		};
	}

	template< >
	struct traits< _2Real::gl::UniformValue >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( gl::UniformValue ), Type::RENDEROBJECT, "render object", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< _2Real::gl::RenderData >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( gl::RenderData ), Type::RENDEROBJECT, "render object", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< _2Real::gl::Buffer >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( gl::Buffer ), Type::RENDEROBJECT, "render object", TypeCategory::UNIQUE );
		}
	};

	template< >
	struct traits< _2Real::gl::Texture >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( gl::Texture ), Type::RENDEROBJECT, "render object", TypeCategory::UNIQUE );
		}
	};
}