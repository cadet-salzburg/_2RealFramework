#include "Datatypes.h"
#include "RessourceManager.h"

namespace _2Real
{
	namespace gl
	{
		ShaderObj::ShaderObj( RessourceManager const& mgr ) :
			mManager( mgr ), mHandle( 0 ), mIsCompiled( false ), mSource(), mInfoLog()
		{
		}

		ShaderObj::~ShaderObj()
		{
			mManager.destroyShader( mHandle );
		}

		ProgramObj::ProgramObj( RessourceManager const& mgr ) :
			mManager( mgr ), mHandle( 0 ), mIsLinked( false ), mInfoLog(), mShaders(), mActiveUniforms(), mActiveAttributes()
		{
		}

		ProgramObj::~ProgramObj()
		{
			for ( std::list< ShaderObj * >::iterator it = mShaders.begin(); it != mShaders.end(); ++it ) delete ( *it );
			mManager.destroyProgram( mHandle );
		}

		ProgramObj::ActiveInput::ActiveInput() :
			mLocation( -1 ), mSize( 1 ), mType( GL_FLOAT ) 
		{
		}

		ProgramObj::ActiveInput::ActiveInput( const GLint loc, const GLint sz, const GLenum type ) :
			mLocation( loc ), mSize( sz ), mType( type )
		{
		}

		TextureObj::TextureObj( RessourceManager const& mgr ) :
			mManager( mgr ), mHandle( 0 ), mTarget( GL_TEXTURE_2D ), mWidth( 0 ), mHeight( 0 ), mDatatype( GL_FLOAT ), mFormat( GL_RGBA )
		{
		}

		TextureObj::~TextureObj()
		{
			mManager.destroyTexture( mHandle );
		}

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

		TextureObj::Settings::Settings() :
			wrapS( GL_REPEAT ), wrapT( GL_REPEAT ), wrapR( GL_REPEAT ),
			minFilter( GL_LINEAR ), magFilter( GL_LINEAR_MIPMAP_LINEAR ), format( GL_RGBA )
		{
		}

		bool TextureObj::Settings::operator==( Settings const& o ) const
		{
			return ( ( wrapS == o.wrapS ) && ( wrapT == o.wrapT ) && ( wrapR == o.wrapR ) &&
				( minFilter == o.minFilter ) && ( magFilter == o.magFilter ) && ( format == o.format ) );
		}
		
		bool TextureObj::Settings::operator!=( Settings const& o ) const
		{
			return ( !( *this == o ) );
		}

		BufferObj::BufferObj( RessourceManager const& mgr ) :
			mManager( mgr ), mHandle( 0 ), mSizeInBytes( 0 ),
			mElementCount( 0 ), mDatatype( GL_FLOAT ), mTarget( GL_ARRAY_BUFFER )
		{
		}

		BufferObj::~BufferObj()
		{
			mManager.destroyBuffer( mHandle );
		}

		RenderData::RenderData() :
			mPrimitiveType( GL_POINTS ), mElementCount( 0 ), mDrawIndexed( false )
		{
		}

		void RenderData::addAttribute( const unsigned int index, VertexAttribute const& attribute )
		{
			mAttributes[ index ] = attribute;
		}

		void RenderData::addTexture( const unsigned int unit, Texture const& texture )
		{
			mTextures[ unit ] = texture;
		}

		void RenderData::addIndices( Buffer const& i )
		{
			mIndices = i; mDrawIndexed = true; mElementCount = i->mElementCount;
		}

		RenderData::VertexAttribute::VertexAttribute() :
			buffer( nullptr ), size( 0 ), stride( 0 ), normalized( false )
		{
		}

		RenderData::VertexAttribute::VertexAttribute( Buffer const& b, const unsigned int sz, const size_t s, const bool n ) :
			buffer( b ), size( sz ), stride( s ), normalized( n )
		{
		}
	}
}