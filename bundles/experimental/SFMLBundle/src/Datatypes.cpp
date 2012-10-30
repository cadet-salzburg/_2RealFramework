#include "Datatypes.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <algorithm>

namespace _2Real
{
	namespace gl
	{
		//ShaderObj::ShaderObj( const GLenum type, GlewContext &glewContext ) :
		//	mGlewContext( glewContext ),
		//	mType( type ),
		//	mShaderSource(),
		//	mCompileStatus( GL_FALSE ),
		//	mShaderInfo()
		//{
		//	mHandle = glCreateShader( type );
		//}

		//ShaderObj::ShaderObj( const GLenum type, GlewContext &glewContext, std::string const& source ) :
		//	mGlewContext( glewContext ),
		//	mType( type ),
		//	mShaderSource( source ),
		//	mCompileStatus( GL_FALSE ),
		//	mShaderInfo()
		//{
		//	mHandle = glCreateShader( type );
		//	compile();
		//}

		//ShaderObj::ShaderObj( ShaderObj const& src ) :
		//	mType( src.mType ),
		//	mShaderSource( src.mShaderSource ),
		//	mCompileStatus( GL_FALSE ),
		//	mShaderInfo()
		//{
		//	mHandle = glCreateShader( src.mType );
		//	if ( !mShaderSource.empty() )
		//	{
		//		compile();
		//	}
		//}

		//ShaderObj::~ShaderObj()
		//{
		//	mManager->destroyShader( mHandle );
		//}

		//void ShaderObj::compile()
		//{
		//	const char* source = mShaderSource.c_str();
		//	glShaderSource( mHandle, 1, &source, 0 );
		//	glCompileShader( mHandle );

		//	glGetShaderiv( mHandle, GL_COMPILE_STATUS, &mCompileStatus );

		//	GLint sz = 0;
		//	glGetShaderiv( mHandle, GL_INFO_LOG_LENGTH, &sz );
		//	if (sz > 0)
		//	{
		//		char* buffer = new char[sz];
		//		memset(buffer, 0, sz);
		//		glGetShaderInfoLog( mHandle, sz, NULL, buffer );
		//		mShaderInfo = buffer;
		//		delete[] buffer;
		//	}
		//}

		//void ShaderObj::compileFromSourceString( std::string const& shaderSource )
		//{
		//	mShaderSource = shaderSource;
		//	compile();
		//}

		//ProgramObj::ProgramObj( GlewContext &glewContext ) :
		//	mGlewContext( glewContext ),
		//	mShaders(),
		//	mLinkStatus( GL_FALSE ),
		//	mProgramInfo()
		//{
		//	mHandle = glCreateProgram();
		//}

		//ProgramObj::ProgramObj( ProgramObj const& src ) :
		//	mGlewContext( src.mGlewContext ),
		//	mShaders( src.mShaders ),
		//	mLinkStatus( GL_FALSE ),
		//	mProgramInfo()
		//{
		//	mHandle = glCreateProgram();
		//	for ( std::list< GLuint >::iterator it = mShaders.begin(); it != mShaders.end(); ++it )
		//	{
		//		glAttachShader( mHandle, *it );
		//	}
		//}

		//ProgramObj& ProgramObj::operator=( ProgramObj const& src )
		//{
		//	if (this == &src)
		//	{
		//		return *this;
		//	}

		//	for ( std::list< GLuint >::iterator it = mShaders.begin(); it != mShaders.end(); ++it )
		//	{
		//		glDetachShader( mHandle, *it );
		//	}

		//	mShaders.clear();
		//	mShaders = src.mShaders;

		//	for ( std::list< GLuint >::iterator it = mShaders.begin(); it != mShaders.end(); ++it )
		//	{
		//		glAttachShader( mHandle, *it );
		//	}

		//	return *this;
		//}

		//ProgramObj::~ProgramObj()
		//{
		//	mManager->destroyProgram( mHandle );
		//}

		//void ProgramObj::attachShader( ShaderObj const& shader )
		//{
		//	for ( std::list< GLuint >::iterator it = mShaders.begin(); it != mShaders.end(); ++it )
		//	{
		//		if ( *it == shader.mHandle )
		//		{
		//			return;
		//		}
		//	}

		//	glAttachShader( mHandle, shader.mHandle );
		//	mShaders.push_back( shader.mHandle );
		//}

		//void ProgramObj::detachShader( ShaderObj const& shader )
		//{
		//	std::list< GLuint >::iterator result = std::find( mShaders.begin(), mShaders.end(), shader.mHandle );
		//	if ( result != mShaders.end() )
		//	{
		//		glDetachShader( mHandle, shader.mHandle );
		//		mShaders.erase( result );
		//	}
		//}

		//void ProgramObj::bind()
		//{
		//	glUseProgram( mHandle );
		//}

		//void ProgramObj::unbind()
		//{
		//	glUseProgram( 0 );
		//}

		//void ProgramObj::link()
		//{
		//	glLinkProgram( mHandle );
		//	glGetProgramiv( mHandle, GL_LINK_STATUS, &mLinkStatus );

		//	GLint sz = 0;
		//	glGetProgramiv( mHandle, GL_INFO_LOG_LENGTH, &sz );
		//	if (sz > 0)
		//	{
		//		char* buffer = new char[sz];
		//		memset(buffer, 0, sz);
		//		glGetProgramInfoLog( mHandle, sz, NULL, buffer );
		//		mProgramInfo = buffer;
		//		delete[] buffer;
		//	}

		//	getActiveUniforms();
		//}

		//bool ProgramObj::validate()
		//{
		//	glValidateProgram( mHandle );
		//	GLint status;
		//	glGetProgramiv( mHandle, GL_VALIDATE_STATUS, &status );

		//	return ( status ? true : false );
		//}

		//void ProgramObj::getActiveUniforms()
		//{
		//	GLint count, maxLength;
		//	glGetProgramiv( mHandle, GL_ACTIVE_UNIFORMS, &count );
		//	glGetProgramiv( mHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength );

		//	char* buffer = new char[maxLength];
		//	memset(buffer, 0, maxLength);

		//	GLsizei written;
		//	GLenum type;
		//	GLint size;

		//	for (int i=0; i<count; i++)
		//	{
		//		std::ostringstream s;
		//		glGetActiveUniform( mHandle, i, maxLength, &written, &size, &type, buffer );
		//		s << buffer;
		//		mActiveUniforms[s.str()] = std::make_pair< GLuint, GLenum >( i, type );

		//		//std::cout << "active uniform: " << s.str() << std::endl;
		//	}
		//}

		//void ProgramObj::setUniformTexture( std::string const& name, const int value )
		//{
		//	if ( mActiveUniforms.find( name ) == mActiveUniforms.end() ) return;

		//	if (	mActiveUniforms[ name ].second != GL_SAMPLER_1D &&
		//			mActiveUniforms[ name ].second != GL_SAMPLER_2D &&
		//			mActiveUniforms[ name ].second != GL_SAMPLER_3D &&
		//			mActiveUniforms[ name ].second != GL_SAMPLER_CUBE	)
		//	{
		//		std::cout << "wrong datatype: uniform " << name << " should be a texture\n";
		//		return;
		//	}
		//
		//	glUniform1i( mActiveUniforms[ name ].first, value );
		//}

		//void ProgramObj::setUniformValue( std::string const& name, const int value )
		//{
		//	if ( mActiveUniforms.find( name ) == mActiveUniforms.end() ) return;

		//	if ( mActiveUniforms[ name ].second != GL_INT )
		//	{
		//		std::cout << "wrong datatype: uniform " << name << " should be a signed integer\n";
		//		return;
		//	}
		//
		//	glUniform1i( mActiveUniforms[ name ].first, value );
		//}

		//void ProgramObj::setUniformValue( std::string const& name, const unsigned int value )
		//{
		//	if ( mActiveUniforms.find( name ) == mActiveUniforms.end() ) return;

		//	if ( mActiveUniforms[ name ].second != GL_UNSIGNED_INT )
		//	{
		//		std::cout << "wrong datatype: uniform " << name << " should be an unsigned integer\n";
		//		return;
		//	}
		//
		//	glUniform1ui( mActiveUniforms[ name ].first, value );
		//}

		//void ProgramObj::setUniformValue( std::string const& name, const float value )
		//{
		//	if ( mActiveUniforms.find( name ) == mActiveUniforms.end() ) return;

		//	if ( mActiveUniforms[ name ].second != GL_FLOAT )
		//	{
		//		std::cout << "wrong datatype: uniform " << name << " should be a float\n";
		//		return;
		//	}
		//
		//	glUniform1f( mActiveUniforms[ name ].first, value );
		//}

		//void ProgramObj::setUniformMat2( std::string const& name, float const *mat, const bool transpose )
		//{
		//	if ( mActiveUniforms.find(name) == mActiveUniforms.end() )
		//	{
		//		return;
		//	}

		//	if ( mActiveUniforms.at(name).second != GL_FLOAT_MAT2 )
		//	{
		//		std::cout << "wrong datatype: uniform " << name << " should be a 2x2 matrix\n";
		//	}
		//
		//	glUniformMatrix2fv( mActiveUniforms.at(name).first, 1, transpose, mat );
		//}

		//void ProgramObj::setUniformMat4(std::string const& name, float const* mat, const bool transpose)
		//{
		//	if (mActiveUniforms.find(name) == mActiveUniforms.end())
		//	{
		//		return;
		//	}

		//	if ( mActiveUniforms[ name ].second != GL_FLOAT_MAT4 )
		//	{
		//		std::cout << "wrong datatype: uniform " << name << " should be a 4x4 matrix\n";
		//		return;
		//	}

		//	glUniformMatrix4fv( mActiveUniforms[ name ].first, 1, transpose, mat );
		//}

		//void Program::bindAttribLocation( std::string const& name, const GLuint index )
		//{
		//	glBindAttribLocation( m_Handle, index, name.c_str() );
		//}

		//GLuint ProgramObj::getAttributeLocation( std::string const& attrib )
		//{
		//	return glGetAttribLocation( mHandle, attrib.c_str() );
		//}

		//void Program::bindFragDataLocation( std::string const& name, const GLuint index)
		//{
		//	glBindFragDataLocation( m_Handle, index, name.c_str() );
		//}

		//GLuint ProgramObj::getFragDataLocation( std::string const& fragData )
		//{
		//	return glGetFragDataLocation( mHandle, fragData.c_str() );
		//}
	}
}