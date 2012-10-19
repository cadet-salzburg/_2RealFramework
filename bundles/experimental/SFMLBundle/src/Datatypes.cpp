#include "Datatypes.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <algorithm>

namespace _2Real
{
	namespace gl
	{
		ShaderObj::ShaderObj( const GLenum type, GlewContext &glewContext ) :
			m_GlewContext( glewContext ),
			m_Type( type ),
			m_ShaderSource(),
			m_CompileStatus( GL_FALSE ),
			m_ShaderInfo()
		{
			m_Handle = glCreateShader( type );
		}

		ShaderObj::ShaderObj( const GLenum type, GlewContext &glewContext, std::string const& source, const bool isFile ) :
			m_GlewContext( glewContext ),
			m_Type( type ),
			m_ShaderSource( isFile ? loadShaderSource( source ) : source ),
			m_CompileStatus( GL_FALSE ),
			m_ShaderInfo()
		{
			m_Handle = glCreateShader( type );
			compile();
		}

		ShaderObj::ShaderObj( ShaderObj const& src ) :
			m_Type( src.m_Type ),
			m_ShaderSource( src.m_ShaderSource ),
			m_CompileStatus( GL_FALSE ),
			m_ShaderInfo()
		{
			m_Handle = glCreateShader( src.m_Type );
			if ( !m_ShaderSource.empty() )
			{
				compile();
			}
		}

		ShaderObj::~ShaderObj()
		{
			glDeleteShader(m_Handle);
		}

		void ShaderObj::compile()
		{
			const char* source = m_ShaderSource.c_str();
			glShaderSource( m_Handle, 1, &source, 0 );
			glCompileShader( m_Handle );

			glGetShaderiv( m_Handle, GL_COMPILE_STATUS, &m_CompileStatus );

			GLint sz = 0;
			glGetShaderiv( m_Handle, GL_INFO_LOG_LENGTH, &sz );
			if (sz > 0)
			{
				char* buffer = new char[sz];
				memset(buffer, 0, sz);
				glGetShaderInfoLog( m_Handle, sz, NULL, buffer );
				m_ShaderInfo = buffer;
				delete[] buffer;
			}
		}

		void ShaderObj::compileFromSourceString( std::string const& shaderSource )
		{
			m_ShaderSource = shaderSource;
			compile();
		}

		void ShaderObj::compileFromSourceFile( std::string const& sourcePath )
		{
			compileFromSourceString( loadShaderSource( sourcePath ) );
		}

		const std::string ShaderObj::loadShaderSource( std::string const& shaderSource )
		{
			std::string source;

			std::ifstream file( shaderSource.c_str() );
			if ( !file.is_open() )
			{
				std::ostringstream msg;
				msg << "could not open file" << shaderSource;
				throw std::exception( msg.str().c_str() );
			}

			while ( !file.eof() )
			{
				std::string line;
				getline(file, line);
				source.append(line);
				source.append("\n");
			}

			file.close();
			return source;
		}

		ProgramObj::ProgramObj( GlewContext &glewContext ) :
			m_GlewContext( glewContext ),
			m_Shaders(),
			m_LinkStatus( GL_FALSE ),
			m_ProgramInfo()
		{
			m_Handle = glCreateProgram();
		}

		ProgramObj::ProgramObj( ProgramObj const& src ) :
			m_GlewContext( src.m_GlewContext ),
			m_Shaders( src.m_Shaders ),
			m_LinkStatus( GL_FALSE ),
			m_ProgramInfo()
		{
			m_Handle = glCreateProgram();
			for ( std::list< GLuint >::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it )
			{
				glAttachShader( m_Handle, *it );
			}
		}

		ProgramObj& ProgramObj::operator=( ProgramObj const& src )
		{
			if (this == &src)
			{
				return *this;
			}

			for ( std::list< GLuint >::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it )
			{
				glDetachShader( m_Handle, *it );
			}

			m_Shaders.clear();
			m_Shaders = src.m_Shaders;

			for ( std::list< GLuint >::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it )
			{
				glAttachShader( m_Handle, *it );
			}

			return *this;
		}

		ProgramObj::~ProgramObj()
		{
			for ( std::list< GLuint >::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it )
			{
				glDetachShader( m_Handle, *it );
			}

			glDeleteProgram( m_Handle );
		}

		void ProgramObj::attachShader( ShaderObj const& shader )
		{
			for ( std::list< GLuint >::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it )
			{
				if ( *it == shader.m_Handle )
				{
					return;
				}
			}

			glAttachShader( m_Handle, shader.m_Handle );
			m_Shaders.push_back( shader.m_Handle );
		}

		void ProgramObj::detachShader( ShaderObj const& shader )
		{
			std::list< GLuint >::iterator result = std::find( m_Shaders.begin(), m_Shaders.end(), shader.m_Handle );
			if ( result != m_Shaders.end() )
			{
				glDetachShader( m_Handle, shader.m_Handle );
				m_Shaders.erase( result );
			}
		}

		void ProgramObj::bind()
		{
			glUseProgram( m_Handle );
		}

		void ProgramObj::unbind()
		{
			glUseProgram( 0 );
		}

		void ProgramObj::link()
		{
			glLinkProgram( m_Handle );
			glGetProgramiv( m_Handle, GL_LINK_STATUS, &m_LinkStatus );

			GLint sz = 0;
			glGetProgramiv( m_Handle, GL_INFO_LOG_LENGTH, &sz );
			if (sz > 0)
			{
				char* buffer = new char[sz];
				memset(buffer, 0, sz);
				glGetProgramInfoLog( m_Handle, sz, NULL, buffer );
				m_ProgramInfo = buffer;
				delete[] buffer;
			}

			getActiveUniforms();
		}

		bool ProgramObj::validate()
		{
			glValidateProgram( m_Handle );
			GLint status;
			glGetProgramiv( m_Handle, GL_VALIDATE_STATUS, &status );

			return ( status ? true : false );
		}

		void ProgramObj::getActiveUniforms()
		{
			GLint count, maxLength;
			glGetProgramiv( m_Handle, GL_ACTIVE_UNIFORMS, &count );
			glGetProgramiv( m_Handle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength );

			char* buffer = new char[maxLength];
			memset(buffer, 0, maxLength);

			GLsizei written;
			GLenum type;
			GLint size;

			for (int i=0; i<count; i++)
			{
				std::ostringstream s;
				glGetActiveUniform( m_Handle, i, maxLength, &written, &size, &type, buffer );
				s << buffer;
				m_ActiveUniforms[s.str()] = std::make_pair< GLuint, GLenum >( i, type );

				//std::cout << "active uniform: " << s.str() << std::endl;
			}
		}

		void ProgramObj::setUniformTexture( std::string const& name, const int value )
		{
			if ( m_ActiveUniforms.find( name ) == m_ActiveUniforms.end() ) return;

			if (	m_ActiveUniforms[ name ].second != GL_SAMPLER_1D &&
					m_ActiveUniforms[ name ].second != GL_SAMPLER_2D &&
					m_ActiveUniforms[ name ].second != GL_SAMPLER_3D &&
					m_ActiveUniforms[ name ].second != GL_SAMPLER_CUBE	)
			{
				std::cout << "wrong datatype: uniform " << name << " should be a texture\n";
				return;
			}
		
			glUniform1i( m_ActiveUniforms[ name ].first, value );
		}

		void ProgramObj::setUniformValue( std::string const& name, const int value )
		{
			if ( m_ActiveUniforms.find( name ) == m_ActiveUniforms.end() ) return;

			if ( m_ActiveUniforms[ name ].second != GL_INT )
			{
				std::cout << "wrong datatype: uniform " << name << " should be a signed integer\n";
				return;
			}
		
			glUniform1i( m_ActiveUniforms[ name ].first, value );
		}

		void ProgramObj::setUniformValue( std::string const& name, const unsigned int value )
		{
			if ( m_ActiveUniforms.find( name ) == m_ActiveUniforms.end() ) return;

			if ( m_ActiveUniforms[ name ].second != GL_UNSIGNED_INT )
			{
				std::cout << "wrong datatype: uniform " << name << " should be an unsigned integer\n";
				return;
			}
		
			glUniform1ui( m_ActiveUniforms[ name ].first, value );
		}

		void ProgramObj::setUniformValue( std::string const& name, const float value )
		{
			if ( m_ActiveUniforms.find( name ) == m_ActiveUniforms.end() ) return;

			if ( m_ActiveUniforms[ name ].second != GL_FLOAT )
			{
				std::cout << "wrong datatype: uniform " << name << " should be a float\n";
				return;
			}
		
			glUniform1f( m_ActiveUniforms[ name ].first, value );
		}

		void ProgramObj::setUniformMat2( std::string const& name, float const *mat, const bool transpose )
		{
			if ( m_ActiveUniforms.find(name) == m_ActiveUniforms.end() )
			{
				return;
			}

			if ( m_ActiveUniforms.at(name).second != GL_FLOAT_MAT2 )
			{
				std::cout << "wrong datatype: uniform " << name << " should be a 2x2 matrix\n";
			}
		
			glUniformMatrix2fv( m_ActiveUniforms.at(name).first, 1, transpose, mat );
		}

		void ProgramObj::setUniformMat4(std::string const& name, float const* mat, const bool transpose)
		{
			if (m_ActiveUniforms.find(name) == m_ActiveUniforms.end())
			{
				return;
			}

			if ( m_ActiveUniforms[ name ].second != GL_FLOAT_MAT4 )
			{
				std::cout << "wrong datatype: uniform " << name << " should be a 4x4 matrix\n";
				return;
			}

			glUniformMatrix4fv( m_ActiveUniforms[ name ].first, 1, transpose, mat );
		}

		//void Program::bindAttribLocation( std::string const& name, const GLuint index )
		//{
		//	glBindAttribLocation( m_Handle, index, name.c_str() );
		//}

		GLuint ProgramObj::getAttributeLocation( std::string const& attrib )
		{
			return glGetAttribLocation( m_Handle, attrib.c_str() );
		}

		//void Program::bindFragDataLocation( std::string const& name, const GLuint index)
		//{
		//	glBindFragDataLocation( m_Handle, index, name.c_str() );
		//}

		GLuint ProgramObj::getFragDataLocation( std::string const& fragData )
		{
			return glGetFragDataLocation( m_Handle, fragData.c_str() );
		}
	}
}