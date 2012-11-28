/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#include "RenderDataCombinerBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>
#include <algorithm>

#undef min
#undef max

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

RenderDataCombinerBlock::RenderDataCombinerBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr ), mProgramObj( nullptr ) {}

RenderDataCombinerBlock::~RenderDataCombinerBlock() {}

void RenderDataCombinerBlock::setup( BlockHandle &block )
{
	try
	{
		if ( mContext == nullptr )
		{
			mContext = new Context( mManager.getRenderSettings(), mManager.getManager() );

			mBlockHandle = block;
			mRenderDataOut = block.getOutletHandle( "RenderData" );
			mVertexShaderIn = block.getInletHandle( "VertexShaderSource" );
			mFragmentShaderIn = block.getInletHandle( "FragmentShaderSource" );
			mGeometryShaderIn = block.getInletHandle( "GeometryShaderSource" );
			mBuffersMultiin = block.getInletHandle( "Buffers" );
			mTexturesMultiin = block.getInletHandle( "Textures" );
			mUniformsMultiin = block.getInletHandle( "UniformValues" );
			mAttributesMultiin = block.getInletHandle( "AttributeDescriptions" );
			mPrimitiveTypeIn = block.getInletHandle( "PrimitiveType" );

			std::string vertexSrc = "version 330 core";
			std::string fragmentSrc = "version 330 core";

			mProgramObj = mContext->createProgramObj();
			if ( !vertexSrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_VERTEX_SHADER, vertexSrc ) );
			if ( !fragmentSrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_FRAGMENT_SHADER, fragmentSrc ) );
			mContext->linkProgram( mProgramObj );
			mProgram.reset( mProgramObj );
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void RenderDataCombinerBlock::update()
{
	try
	{
		RenderData &out = mRenderDataOut.getWriteableRef< RenderData >();

		bool isValidData = true;

		mContext->setActive( true );

		if ( mVertexShaderIn.hasChanged() || mFragmentShaderIn.hasChanged() || mGeometryShaderIn.hasChanged() )
		{
			string const& vertexSrc = mVertexShaderIn.getReadableRef< string >();//.mSource;
			string const& geometrySrc = mGeometryShaderIn.getReadableRef< string >();//.mSource;
			string const& fragmentSrc = mFragmentShaderIn.getReadableRef< string >();//.mSource;

			ShaderSource vertexShader = ShaderSource( vertexSrc );
			ShaderSource fragmentShader = ShaderSource( fragmentSrc );
			ShaderSource geometryShader = ShaderSource( geometrySrc );

			std::cout << "glsl program changed" << std::endl;
			//std::cout << vertexSrc << std::endl;
			//std::cout << geometrySrc << std::endl;
			//std::cout << fragmentSrc << std::endl;

			mProgramObj = mContext->createProgramObj();
			if ( !vertexSrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_VERTEX_SHADER, vertexShader.mSource ) );
			if ( !fragmentSrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_FRAGMENT_SHADER, fragmentShader.mSource ) );
			if ( !geometrySrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_GEOMETRY_SHADER, geometryShader.mSource ) );
			mContext->linkProgram( mProgramObj );
			mProgram.reset( mProgramObj );
		}

		mProgramObj->mLock.writeLock();
		mContext->useProgram( mProgramObj );

		const unsigned int numUniforms = mUniformsMultiin.getSize();
		for ( unsigned int i=0; i<numUniforms; ++i )
		{
			string const& uniString = mUniformsMultiin[ i ].getReadableRef< string >();
			if ( uniString.empty() ) continue;

			string name = uniString.substr( 0, uniString.find_first_of( " " ) );

			string::size_type p1 = uniString.find_first_of( "(" );
			string::size_type p2 = uniString.find_first_of( ")" );

			string value = trim( uniString.substr( ++p1, p2-p1 ) );
			stringstream sstr;
			sstr << value;

			ProgramObj::ActiveInputs::const_iterator it = mProgramObj->mActiveUniforms.find( name );
			if ( it != mProgramObj->mActiveUniforms.end() )
			{
				ProgramObj::ActiveInput const& u = it->second;
				if ( u.mType == GL_SAMPLER_2D )
				{
					int unit;
					sstr >> unit;
					mContext->setUniformSampler( u.mLocation, unit );
				}
				else if ( u.mType == GL_FLOAT_MAT4 )
				{
					Mat4 mat;
					for ( unsigned int i=0; i<16; ++i ) sstr >> mat( i );
					mContext->setUniformMat4( u.mLocation, mat );

					std::cout << "set uniform of type mat4 to " << std::endl << mat << std::endl;
				}
				else if ( u.mType == GL_FLOAT_MAT3 )
				{
					Mat3 mat;
					for ( unsigned int i=0; i<9; ++i ) sstr >> mat( i );
					mContext->setUniformMat3( u.mLocation, mat );
				}
				else if ( u.mType == GL_FLOAT_VEC2 )
				{
					Vec2 vec;
					for ( unsigned int i=0; i<2; ++i ) sstr >> vec[ i ];
					mContext->setUniformVec2( u.mLocation, vec );
				}
				else cout << "found unsupported uniform type" << std::endl;
			}
			else cout << "found unknown uniform " << name << endl;
		}

		mContext->resetProgram();
		mContext->finish();
		mProgramObj->mLock.unlock();
		mContext->setActive( false );

		isValidData &= mProgramObj->mIsLinked;

		struct attr
		{
			std::string		name;
			size_t			stride;
			unsigned int	location;
			unsigned int	size;
		};

		typedef std::map< unsigned int, attr > AttribMap;
		AttribMap attributes;

		bool hasIndices = false;
		unsigned int index = 0;
		const unsigned int numAttribs = mAttributesMultiin.getSize();
		for ( unsigned int i=0; i<numAttribs; ++i )
		{
			string const& attrString = mAttributesMultiin[ i ].getReadableRef< string >();

			std::cout << attrString << std::endl;

			if ( attrString.empty() ) continue;

			string name = attrString.substr( 0, attrString.find_first_of( " " ) );

			if ( name != "indices" )
			{
				string::size_type p1 = attrString.find_first_of( "(" );
				string::size_type p2 = attrString.find_first_of( ")" );

				string description = trim( attrString.substr( ++p1, p2-p1 ) );
				stringstream sstr;
				sstr << description;
				unsigned int size;
				size_t stride;
				sstr >> size >> stride;

				std::cout << "found attrib " << name << " " << size << " " << stride << std::endl;

				ProgramObj::ActiveInputs::const_iterator it = mProgramObj->mActiveAttributes.find( name );
				if ( it != mProgramObj->mActiveAttributes.end() )
				{
					attr attrib;
					attrib.name = name;
					attrib.stride = stride;
					attrib.size = size;
					attrib.location = it->second.mLocation;
					attributes[ i ] = attrib;
				}
			}
			else
			{
				std::cout << "found index buffer!" << std::endl;
				hasIndices = true;
				index = i;
			}
		}

		list< unsigned int > attribsPerBuffer;

		const unsigned int numBuffers = mBuffersMultiin.getSize();
		unsigned int validAttribs =0;
		bool foundIndex = false;
		unsigned int indexElements = 0;
		for ( unsigned int i=0; i<numBuffers; ++i )
		{
			Buffer const& buffer = mBuffersMultiin[ i ].getReadableRef< Buffer >();

			if ( buffer.get() == nullptr ) continue;

			if ( hasIndices && i == index )
			{
				foundIndex = true;
				out.addIndices( buffer );
				std::cout << "index buffer has " << buffer->mElementCount << " elements" << std::endl;
			}
			else
			{
				++validAttribs;
				std::cout << "attribute buffer has " << buffer->mElementCount << " elements" << std::endl;
				AttribMap::iterator it = attributes.find( i );
				if ( it != attributes.end() )
				{
					attr a = attributes[ i ];
					out.addAttribute( a.location, RenderData::VertexAttribute( buffer, a.size, a.stride, false ) );
					attribsPerBuffer.push_back( buffer->mElementCount / a.size );
				}
				else std::cout << "found unknown attribute" << std::endl;
			}
		}

		if ( validAttribs == 0 ) isValidData = false;
		if ( hasIndices && !foundIndex ) isValidData = false;

		unsigned int elementsToDraw;
		//if ( hasIndices )						elementsToDraw = 
		if ( attribsPerBuffer.empty() )	elementsToDraw = 0;
		else									elementsToDraw = max< unsigned int >( 0, *min_element( attribsPerBuffer.begin(), attribsPerBuffer.end() ) );

		const unsigned int numTextures = mTexturesMultiin.getSize();
		unsigned int validTextures = 0;
		for ( unsigned int i=0; i<numTextures; ++i )
		{
			Texture const& texture = mTexturesMultiin[ i ].getReadableRef< Texture >();
			if ( texture.get() == nullptr ) continue;
			out.addTexture( i, texture );
			++validTextures;
		}

		if ( !isValidData )
		{
			std::cout << "invalid render data!" << std::endl;
			mRenderDataOut.discard();
		}
		else
		{
			if ( !hasIndices )
			{
				out.mPrimitiveType = PrimitiveType::getGLPrimitiveType( mPrimitiveTypeIn.getReadableRef< int >() );
				out.mElementCount = elementsToDraw;
				out.mDrawIndexed = false;
			}
			else
			{
				int primType = mPrimitiveTypeIn.getReadableRef< int >();
				out.mPrimitiveType = PrimitiveType::getGLPrimitiveType( primType );
				out.mDrawIndexed = true;
			}

			out.mProgram = mProgram;
		}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void RenderDataCombinerBlock::shutdown()
{
	try
	{
		mProgramObj = nullptr;
		delete mContext;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}