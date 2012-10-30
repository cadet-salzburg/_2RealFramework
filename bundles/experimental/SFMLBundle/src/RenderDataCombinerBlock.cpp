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
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ), mContext( nullptr ) {}

RenderDataCombinerBlock::~RenderDataCombinerBlock() {}

void RenderDataCombinerBlock::setup( BlockHandle &block )
{
	try
	{
		if ( mContext == nullptr )
		{
			RenderSettings settings;
			settings.title = "";
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;
			settings.width = 640;
			settings.height = 480;

			mContext = new Context( settings, mManager.getManager() );

			mBlockHandle = block;
			mRenderDataOut = block.getOutletHandle( "RenderData" );
			mVertexShaderIn = block.getInletHandle( "VertexShaderSource" );
			mFragmentShaderIn = block.getInletHandle( "FragmentShaderSource" );
			mGeometryShaderIn = block.getInletHandle( "GeometryShaderSource" );
			mBuffersMultiin = block.getInletHandle( "Buffers" );
			mTexturesMultiin = block.getInletHandle( "Textures" );
			mUniformsMultiin = block.getInletHandle( "UniformValues" );
			mAttributesMultiin = block.getInletHandle( "AttributeDescriptions" );
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
			string const& vertexSrc = mVertexShaderIn.getReadableRef< string >();
			string const& geometrySrc = mGeometryShaderIn.getReadableRef< string >();
			string const& fragmentSrc = mFragmentShaderIn.getReadableRef< string >();

			mProgramObj = mContext->createProgramObj();
			if ( !vertexSrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_VERTEX_SHADER, vertexSrc ) );
			if ( !fragmentSrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_FRAGMENT_SHADER, fragmentSrc ) );
			if ( !geometrySrc.empty() ) mContext->attachShader( mProgramObj, mContext->createShaderObj( GL_GEOMETRY_SHADER, geometrySrc ) );
			mContext->linkProgram( mProgramObj );
			mProgram.reset( mProgramObj );
		}

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

			ProgramObj::ActiveVars::const_iterator it = mProgramObj->mActiveUniforms.find( name );
			if ( it != mProgramObj->mActiveUniforms.end() )
			{
				ProgramObj::ActiveVar const& u = it->second;
				if ( u.type == GL_SAMPLER_2D )
				{
					int unit;
					sstr >> unit;
					//cout << "setting uniform " << name << " of type sampler2D to\ntex unit " << unit << endl;

					// currently, textures are assigned to units in ascending order -> multiinlet & tex unit are identical
					mContext->setUniformSampler( u.location, unit );
				}
				else if ( u.type == GL_FLOAT_MAT4 )
				{
					Mat4 mat;
					for ( unsigned int i=0; i<16; ++i ) sstr >> mat( i );

					//cout << "setting uniform " << name << " of type mat4x4 to\n" << mat << endl;

					mContext->setUniformMat4( u.location, mat );
				}
				else cout << "found unsupported uniform type" << std::endl;
			}
		}

		mContext->resetProgram();
		mContext->finish();
		mContext->setActive( false );

		// parse attrib descriptions
		struct attr
		{
			std::string		name;
			size_t			stride;
			unsigned int	location;
			unsigned int	size;
		};

		typedef std::map< unsigned int, attr > AttribMap;
		AttribMap attributes;

		const unsigned int numAttribs = mAttributesMultiin.getSize();
		for ( unsigned int i=0; i<numAttribs; ++i )
		{
			string const& attrString = mAttributesMultiin[ i ].getReadableRef< string >();
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

				//cout << "found vertex attribute " << name << " , size: " << size << endl;

				ProgramObj::ActiveVars::const_iterator it = mProgramObj->mActiveAttributes.find( name );
				if ( it != mProgramObj->mActiveAttributes.end() )
				{
					attr attrib;
					attrib.name = name;
					attrib.stride = stride;
					attrib.size = size;
					attrib.location = it->second.location;
					attributes[ i ] = attrib;
				}
				//else cout << "unknown attribute " << name << std::endl;
			}
		}

		list< unsigned int > attribsPerBuffer;

		const unsigned int numBuffers = mBuffersMultiin.getSize();
		unsigned int validAttribs =0;
		for ( unsigned int i=0; i<numBuffers; ++i )
		{
			Buffer const& buffer = mBuffersMultiin[ i ].getReadableRef< Buffer >();
			if ( buffer.get() == nullptr ) continue;

			AttribMap::iterator it = attributes.find( i );
			if ( it != attributes.end() )
			{
				attr a = attributes[ i ];

				out.addAttribute( a.location, RenderData::VertexAttribute( buffer, a.size, a.stride, false ) );
				++validAttribs;

				attribsPerBuffer.push_back( buffer->mElementCount / a.size );
			}
		}

		unsigned int elementsToDraw;
		if ( attribsPerBuffer.empty() )	elementsToDraw = 0;
		else							elementsToDraw = max< unsigned int >( 0, *min_element( attribsPerBuffer.begin(), attribsPerBuffer.end() ) );


		const unsigned int numTextures = mTexturesMultiin.getSize();
		unsigned int validTextures = 0;
		for ( unsigned int i=0; i<numTextures; ++i )
		{
			Texture const& texture = mTexturesMultiin[ i ].getReadableRef< Texture >();
			if ( texture.get() == nullptr ) continue;

			// tex units: ascending order
			out.addTexture( i, texture );
			++validTextures;
		}

		out.mPrimitiveType = GL_POINTS;
		out.mElementCount = elementsToDraw;
		out.mProgram = mProgram;

		// less than 1 attribute -> discard output data
		isValidData &= ( validAttribs > 0 );
		// invalid program obj -> discard output data
		isValidData &= mProgramObj->mIsLinked;

		if ( !isValidData )
		{
			mRenderDataOut.discard();
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