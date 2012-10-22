#pragma once

#include "gl.h"

namespace _2Real
{
	namespace gl
	{
		class ShaderObj
		{

		public:

			GlewContext const& getGlewContext() const { return m_GlewContext; }

			ShaderObj( const GLenum type, GlewContext &glewContext );
			ShaderObj( const GLenum type, GlewContext &glewContext, std::string const& source, const bool isFile = false );
			ShaderObj( ShaderObj const& src );
			~ShaderObj();

			void compileFromSourceFile( std::string const& sourcePath );
			void compileFromSourceString( std::string const& shaderSource );

			std::string const& getShaderInfo() const { return m_ShaderInfo; }
			const bool isValid() const { return ( m_CompileStatus ? true : false ); }

			static const std::string loadShaderSource( std::string const& sourcePath );

		private:

			friend class ProgramObj;

			void compile();

			GlewContext				m_GlewContext;
			GLuint					m_Handle;
			GLenum					const m_Type;
			std::string				m_ShaderSource;
			GLint					m_CompileStatus;
			std::string				m_ShaderInfo;

		};

		class ProgramObj
		{

		public:

			GlewContext const& getGlewContext() const { return m_GlewContext; }

			ProgramObj( GlewContext &glewContext );
			//Program( const std::list< Shader > shaders, GlewContext &glewContext );
			ProgramObj( ProgramObj const& src );
			ProgramObj& operator=( ProgramObj const& src );
			~ProgramObj();

			void attachShader( ShaderObj const& shader );
			void detachShader( ShaderObj const& shader );

			void setUniformTexture( std::string const& name, const int value );
			void setUniformValue( std::string const& name, const int value );
			void setUniformValue( std::string const& name, const unsigned int value );
			void setUniformValue( std::string const& name, const float value );
			void setUniformMat4( std::string const& name, float const* mat, const bool transpose=false );
			void setUniformMat2( std::string const& name, float const* mat, const bool transpose=false );

			void bind();
			void unbind();
			void link();
			bool validate();

			//void bindAttribLocation( std::string const& name, const GLuint index );
			//void bindFragDataLocation( std::string const& name, const GLuint index );

			std::string const& getProgramInfo() const { return m_ProgramInfo; }
			const bool isValid() const { return ( m_LinkStatus ? true : false ); }

			GLuint getAttributeLocation( std::string const& attribute );
			GLuint getFragDataLocation( std::string const& fragData );

		private:

			typedef std::map< std::string, std::pair< GLuint, GLenum > >	UniformTable;

			void getActiveUniforms();

			GlewContext				m_GlewContext;
			GLuint					m_Handle;
			std::list< GLuint >		m_Shaders;
			GLint					m_LinkStatus;
			std::string				m_ProgramInfo;
			UniformTable			m_ActiveUniforms;

		};

		class ShaderSetting
		{
			struct SamplerUniform
			{
				GLenum type;
				std::string const& name;
			};

			struct Mat4x4Uniform
			{
			};

			struct IntUniform
			{
			};

		};

		class RenderData
		{

		public:

			struct TextureBinding
			{
				gl::Texture			texture;
				mutable GLuint		unit;			// holds the resolved tex unit
			};

			struct VertexAttribute
			{
				gl::Buffer			buffer;
				mutable GLint		index;			// holds the resolved attribute name
				GLuint				size;			// variables used to describe the vertex attribute - datatype comes from the buffer itself
				GLuint				offset;
				GLsizei				stride;
				GLboolean			normalized;
			};

			RenderData() : m_PrimitiveType( GL_POINTS ), m_ElementCount( 0 ), m_DrawIndexed( false ) {}

			RenderData( RenderData const& src ) :
				m_Textures( src.m_Textures ),
				m_Attributes( src.m_Attributes ),
				m_PrimitiveType( src.m_PrimitiveType ),
				m_ElementCount( src.m_ElementCount ),
				m_DrawIndexed( src.m_DrawIndexed )
			{
			}

			RenderData& operator=( RenderData const& src )
			{
				if ( this == &src ) return *this;

				m_Textures = src.m_Textures;
				m_Attributes = src.m_Attributes;
				m_PrimitiveType = src.m_PrimitiveType;
				m_ElementCount = src.m_ElementCount;
				m_DrawIndexed = src.m_DrawIndexed;

				return *this;
			}

			void addTextureAs( RenderData const& src, std::string const& srcTexName, std::string const& name )
			{
				std::map< std::string, TextureBinding >::const_iterator tIt = src.m_Textures.find( srcTexName );
				if ( tIt == src.m_Textures.end() ) return;
				m_Textures[ name ] = tIt->second;
			}

			void addAttributeAs( RenderData const& src, std::string const& srcAttribName, std::string const& name )
			{
				std::map< std::string, VertexAttribute >::const_iterator aIt = src.m_Attributes.find( srcAttribName );
				if ( aIt == src.m_Attributes.end() ) return;
				m_Attributes[ name ] = aIt->second;
			}

			bool operator==( RenderData const& other ) const { return false; }

			void setNumberOfVertices( const unsigned int count )	{ m_ElementCount = count; }
			void setPrimitiveType( const GLenum primType )			{ m_PrimitiveType = primType; }
			unsigned int getNumberOfVertices() const				{ return m_ElementCount; }
			GLenum getPrimitiveType() const							{ return m_PrimitiveType; }

			void addAttribute( VertexAttribute const& attribute, std::string const& name )
			{
				m_Attributes[ name ] = attribute;
			}

			void addTexture( TextureBinding const& texture, std::string const& name )
			{
				m_Textures[ name ] = texture;
			}

		private:

			friend class gl::Renderer;

			std::map< std::string, TextureBinding >		m_Textures;
			std::map< std::string, VertexAttribute >	m_Attributes;
			Program										m_Program;


			GLenum										m_PrimitiveType;
			unsigned int								m_ElementCount;
			bool										m_DrawIndexed;

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
}