#pragma once

#include "gl.h"
#include "Poco/RWLock.h"

#include "_2RealDatatypes.h"
#include "datatypes/_2RealTypeStreamOperators.h"

#include <fstream>

namespace _2Real
{
	namespace gl
	{
		class RessourceManager;

		struct ShaderSource
		{
			ShaderSource() : mFile( "" ), mSource( "" ) {}
			ShaderSource( std::string const& file ) : mFile( file )
			{
				mSource = loadShaderSource( mFile );
			}

			bool operator==( ShaderSource const& other ) const { return mFile == other.mFile; }

			std::string loadShaderSource( std::string const& filePath )
			{
				std::ifstream file( filePath.c_str() );
				if ( !file.is_open() )
				{
					std::ostringstream msg;
					std::cout << "could not open file: " << filePath << std::endl;
					return "";
				}

				std::string source;
				while ( !file.eof() )
				{
					std::string line;
					std::getline( file, line );
					source.append( line );
					source.append( "\n" );
				}
				file.close();
				return source;
			}

			std::string			mFile;
			std::string			mSource;
		};

		struct ShaderObj
		{
			ShaderObj( RessourceManager const& mgr );
			~ShaderObj();

			RessourceManager	const& mManager;
			GLuint				mHandle;
			bool				mIsCompiled;
			std::string			mSource;
			std::string			mInfoLog;
		};

		struct ProgramObj
		{
			ProgramObj( RessourceManager const& mgr );
			~ProgramObj();

			struct ActiveInput
			{
				ActiveInput();
				ActiveInput( const GLint l, const GLint sz, const GLenum t );

				GLint			mLocation;
				GLint			mSize;
				GLenum			mType;
			};

			typedef std::map< std::string, ActiveInput >	ActiveInputs;
			typedef std::list< ShaderObj * >				Shaders;

			RessourceManager		const& mManager;
			GLuint					mHandle;
			bool					mIsLinked;
			std::string				mInfoLog;
			Shaders					mShaders;
			ActiveInputs			mActiveUniforms;
			ActiveInputs			mActiveAttributes;
			mutable Poco::RWLock	mLock;
		};

		struct TextureObj
		{
			TextureObj( RessourceManager const& mgr );
			~TextureObj();

			bool isEmpty() const { return ( mWidth == 0 || mHeight == 0 ); }
			static GLenum getTextureFormat( const ImageChannelOrder order );

			struct Settings
			{
				Settings();
				bool operator==( Settings const& o ) const;
				bool operator!=( Settings const& o ) const;

				GLenum		wrapS;
				GLenum		wrapT;
				GLenum		wrapR;
				GLenum		minFilter;
				GLenum		magFilter;
				GLenum		format;
			};

			RessourceManager		const& mManager;
			GLuint					mHandle;
			GLenum					mTarget;
			unsigned int			mWidth;
			unsigned int			mHeight;
			GLenum					mDatatype;
			GLenum					mFormat;
			Settings				mSettings;
			mutable Poco::RWLock	mLock;
		};

		struct BufferObj
		{
			BufferObj( RessourceManager const& mgr );
			~BufferObj();

			RessourceManager		const& mManager;
			GLuint					mHandle;
			size_t					mSizeInBytes;
			unsigned int			mElementCount;
			GLenum					mDatatype;
			GLenum					mTarget;
			mutable Poco::RWLock	mLock;
		};

		struct RenderData
		{
			RenderData();

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

			struct VertexAttribute
			{
				VertexAttribute();
				VertexAttribute( Buffer const& b, const unsigned int sz, const size_t s, const bool n );

				gl::Buffer			buffer;
				GLuint				size;
				GLsizei				stride;
				GLboolean			normalized;
			};

			// index, description -> index is determined by render data combiner
			typedef std::map< GLuint, VertexAttribute >		Attributes;
			// tex unit, description -> unit is determined by render data combiner
			typedef std::map< GLuint, Texture >				Textures;

			void addAttribute( const unsigned int index, VertexAttribute const& attribute );
			void addTexture( const unsigned int unit, Texture const& texture );
			void addIndices( Buffer const& b );

			Program					mProgram;
			Attributes				mAttributes;
			Buffer					mIndices;
			Textures				mTextures;
			GLenum					mPrimitiveType;
			unsigned int			mElementCount;
			bool					mDrawIndexed;

		};
	}

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
	struct traits< _2Real::gl::ShaderSource >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( gl::ShaderSource ), Type::RENDEROBJECT, "shader source", TypeCategory::UNIQUE );
		}
	};

	template< >
	inline void writeTo( std::ostream &out, gl::ShaderSource const& source )
	{
		out << source.mFile;
	}

	template< >
	inline void readFrom( std::istream &in, gl::ShaderSource & s )
	{
		std::string file;
		in >> file;
		s = gl::ShaderSource( file );
	}

	template< >
	struct traits< _2Real::gl::Texture >
	{
		static TypeDescriptor *createTypeDescriptor()
		{
			return new TypeDescriptor( typeid( gl::Texture ), Type::RENDEROBJECT, "render object", TypeCategory::UNIQUE );
		}
	};
}