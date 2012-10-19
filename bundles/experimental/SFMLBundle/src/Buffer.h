#pragma once

#include <GLEW\glew.h>

#include <string>
#include <list>
#include <map>

namespace cthulhuGl
{

	class Shader
	{

	public:

		Shader( const GLenum type );
		Shader( const GLenum type, std::string const& source, const bool isFile );
		Shader( Shader const& src );
		~Shader();

		const GLuint getHandle() const { return m_Handle; }

		void compileFromSourceFile( std::string const& sourcePath );
		void compileFromSourceString( std::string const& shaderSource );

		std::string const& getShaderInfo() const { return m_ShaderInfo; }
		const bool isValid() const { return ( m_CompileStatus ? true : false ); }

		static const std::string loadShaderSource( std::string const& sourcePath );

	private:

		void compile();

		GLuint					m_Handle;
		GLenum					const m_Type;
		std::string				m_ShaderSource;
		GLint					m_CompileStatus;
		std::string				m_ShaderInfo;

	};

	class Program
	{

	public:

		Program();
		Program( const std::list< Shader > shaders );
		Program( Program const& src );
		Program& operator=( Program const& src );
		~Program();

		void attachShader( Shader const& shader );
		void detachShader( Shader const& shader );

		void setUniformValue(std::string const& name, const int value);
		void setUniformValue(std::string const& name, const unsigned int value);
		void setUniformValue(std::string const& name, const float value);
		void setUniformMat4(std::string const& name, float const* mat, const bool transpose);
		void setUniformMat2(std::string const& name, float const* mat, const bool transpose);

		void bind();
		void unbind();
		void link();
		const bool validate();

		void bindAttribLocation( std::string const& name, const GLuint index );
		void bindFragDataLocation( std::string const& name, const GLuint index );

		std::string const& getProgramInfo() const { return m_ProgramInfo; }
		const bool isValid() const { return ( m_LinkStatus ? true : false ); }

	private:

		typedef std::map< std::string, std::pair< GLuint, GLenum > >	UniformTable;

		void getActiveUniforms();

		GLuint					m_Handle;
		std::list< GLuint >		m_Shaders;
		GLint					m_LinkStatus;
		std::string				m_ProgramInfo;
		UniformTable			m_ActiveUniforms;

	};

}