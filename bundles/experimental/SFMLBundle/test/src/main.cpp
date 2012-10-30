#include "_2RealBundlesUnitTest.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Eigen/Dense"
#include "Eigen/Geometry"

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

string loadShaderSource( string const& filePath )
{
	ifstream file( filePath.c_str() );
	if ( !file.is_open() )
	{
		ostringstream msg;
		cout << "could not open file: " << filePath << endl;
		return "";
	}

	string source;
	while ( !file.eof() )
	{
		string line;
		getline( file, line );
		source.append( line );
		source.append( "\n" );
	}
	file.close();
	return source;
}

int main( int argc, char *argv[] )
{
	float angle = 0;
	float angleInc = M_PI / 180.f;

	try
	{
		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "..\\..\\bin" );
		BundleHandle sfmlBundle = engine.loadBundle( "SFMLBundle" );
		BundleHandle kinectBundle = engine.loadBundle( "KinectOpenNIBundle" );
		BundleHandle cvBundle = engine.loadBundle( "ComputerVisionBundle" );

		//vector< float > vertices;
		//vertices.push_back( -1.0f ); vertices.push_back(  1.0f );
		//vertices.push_back(  1.0f ); vertices.push_back(  1.0f );
		//vertices.push_back( -1.0f ); vertices.push_back( -1.0f );
		//vertices.push_back(  1.0f ); vertices.push_back( -1.0f );

		//vector< float > texcoords;
		//texcoords.push_back( 0.0f ); texcoords.push_back( 1.0f );
		//texcoords.push_back( 1.0f ); texcoords.push_back( 1.0f );
		//texcoords.push_back( 0.0f ); texcoords.push_back( 0.0f );
		//texcoords.push_back( 1.0f ); texcoords.push_back( 0.0f );

		string vertexSrc0, fragmentSrc0, geometrySrc0;
		vertexSrc0 = loadShaderSource( "test0.vert" );
		geometrySrc0 = loadShaderSource( "test0.geo" );
		fragmentSrc0 = loadShaderSource( "test0.frag" );

		string vertexSrc1, fragmentSrc1, geometrySrc1;
		vertexSrc1 = loadShaderSource( "test1.vert" );
		geometrySrc1 = loadShaderSource( "test1.geo" );
		fragmentSrc1 = loadShaderSource( "test1.frag" );

		string uniformDepth = "texDepth ( 0 )";
		string uniformRgb = "texRgb ( 1 )";
		string uniformUser = "texUser ( 2 )";
		string uniformMat = "matModel ( 1.0 0.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0 0.0 0.0 0.0 0.0 1.0 )";
		string attrib0 = "position ( 2 0 )";
		string attrib1 = "texcoord ( 2 0 )";

		Vec4 bgColor;
		bgColor << 0.0, 0.0, 0.0, 1.0;

		const unsigned int w = 640;
		const unsigned int h = 480;

		std::vector< float > pointPositions;
		std::vector< float > pointTexcoords;
		const float stepX = 2.0f/w;
		const float stepY = 2.0f/h;
		const float stepU = 1.0f/w;
		const float stepV = 1.0f/h;

		for ( unsigned int i= 0; i<w; ++i )
		{
			float x = -1.0f + i*stepX;
			float u = i*stepU;
			for ( unsigned int j=0; j<h; ++j )
			{
				float y = -1.0f + j*stepY;
				float v = 1.0f - j*stepV;
				pointPositions.push_back( x );
				pointPositions.push_back( y );
				pointTexcoords.push_back( u );
				pointTexcoords.push_back( v );
			}
		}

		BlockHandle depthBlock = kinectBundle.createBlockInstance( "KinectOpenNIDepthBlock" );
		depthBlock.setup();
		depthBlock.start();
		depthBlock.setUpdateRate( 30.0 );
		depthBlock.getInletHandle( "Is16BitImage" ).setValue< bool >( false );
		depthBlock.getInletHandle( "IsAlignedToColor" ).setValue< bool >( true );

		BlockHandle colorBlock = kinectBundle.createBlockInstance( "KinectOpenNIRgbBlock" );
		colorBlock.setup();
		colorBlock.start();
		colorBlock.setUpdateRate( 30.0 );

		BlockHandle userBlock = kinectBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );
		userBlock.setup();
		userBlock.start();
		userBlock.setUpdateRate( 30.0 );
		userBlock.getInletHandle( "IsAlignedToColor" ).setValue< bool >( true );

		BlockHandle displayWindow0 = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
		displayWindow0.setup();
		displayWindow0.getInletHandle( "ClearColor" ).setValue( bgColor );
		displayWindow0.start();
		displayWindow0.setUpdateRate( 30.0 );

		BlockHandle displayWindow1 = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
		displayWindow1.setup();
		displayWindow1.getInletHandle( "ClearColor" ).setValue( bgColor );
		displayWindow1.start();
		displayWindow1.setUpdateRate( 30.0 );

		BlockHandle eqdepthToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		eqdepthToTexture.setup();
		eqdepthToTexture.start();
		eqdepthToTexture.setUpdateRate( 30.0 );

		BlockHandle depthToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		depthToTexture.setup();
		depthToTexture.start();
		depthToTexture.setUpdateRate( 30.0 );

		BlockHandle colorToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		colorToTexture.setup();
		colorToTexture.start();
		colorToTexture.setUpdateRate( 30.0 );

		BlockHandle userToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		userToTexture.setup();
		userToTexture.start();
		userToTexture.setUpdateRate( 30.0 );

		BlockHandle verticesToBuffer = sfmlBundle.createBlockInstance( "VectorToBufferBlock" );
		verticesToBuffer.setup();
		verticesToBuffer.getInletHandle( "BufferData" ).setValue( pointPositions );
		verticesToBuffer.start();
		verticesToBuffer.setUpdateRate( 30.0 );

		BlockHandle texcoordsToBuffer = sfmlBundle.createBlockInstance( "VectorToBufferBlock" );
		texcoordsToBuffer.setup();
		texcoordsToBuffer.getInletHandle( "BufferData" ).setValue( pointTexcoords );
		texcoordsToBuffer.start();
		texcoordsToBuffer.setUpdateRate( 30.0 );

		BlockHandle eqBlock = cvBundle.createBlockInstance( "OcvHistogramEqualizationBlock" );
		eqBlock.setup();
		eqBlock.start();
		eqBlock.setUpdateRate( 30.0 );

		BlockHandle renderDataCombiner0 = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
		renderDataCombiner0.setup();
		renderDataCombiner0.getInletHandle( "VertexShaderSource" ).setValue< string >( vertexSrc0 );
		renderDataCombiner0.getInletHandle( "FragmentShaderSource" ).setValue< string >( fragmentSrc0 );
		renderDataCombiner0.getInletHandle( "GeometryShaderSource" ).setValue< string >( geometrySrc0 );
		renderDataCombiner0.start();
		renderDataCombiner0.setUpdateRate( 30.0 );

		BlockHandle renderDataCombiner1 = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
		renderDataCombiner1.setup();
		renderDataCombiner1.getInletHandle( "VertexShaderSource" ).setValue< string >( vertexSrc1 );
		renderDataCombiner1.getInletHandle( "FragmentShaderSource" ).setValue< string >( fragmentSrc1 );
		renderDataCombiner1.getInletHandle( "GeometryShaderSource" ).setValue< string >( geometrySrc1 );
		renderDataCombiner1.start();
		renderDataCombiner1.setUpdateRate( 30.0 );

		eqBlock.getInletHandle( "ImageData" ).link( depthBlock.getOutletHandle( "ImageData" ) );
		eqdepthToTexture.getInletHandle( "TextureData" ).link( eqBlock.getOutletHandle( "ImageData" ) );
		depthToTexture.getInletHandle( "TextureData" ).link( depthBlock.getOutletHandle( "ImageData" ) );
		colorToTexture.getInletHandle( "TextureData" ).link( colorBlock.getOutletHandle( "ImageData" ) );
		userToTexture.getInletHandle( "TextureData" ).link( userBlock.getOutletHandle( "ImageData" ) );

		InletHandle buffersMultiin0 = renderDataCombiner0.getInletHandle( "Buffers" );
		buffersMultiin0.add();
		buffersMultiin0[ 0 ].link( verticesToBuffer.getOutletHandle( "Buffer" ) );
		buffersMultiin0.add();
		buffersMultiin0[ 1 ].link( texcoordsToBuffer.getOutletHandle( "Buffer" ) );

		InletHandle texturesMultiin0 = renderDataCombiner0.getInletHandle( "Textures" );
		texturesMultiin0.add();
		texturesMultiin0[ 0 ].link( depthToTexture.getOutletHandle( "Texture" ) );
		texturesMultiin0.add();
		texturesMultiin0[ 1 ].link( colorToTexture.getOutletHandle( "Texture" ) );
		texturesMultiin0.add();
		texturesMultiin0[ 2 ].link( userToTexture.getOutletHandle( "Texture" ) );

		InletHandle uniformsMultiin0 = renderDataCombiner0.getInletHandle( "UniformValues" );
		uniformsMultiin0.add();
		uniformsMultiin0[ 0 ].setValue( uniformDepth );
		uniformsMultiin0.add();
		uniformsMultiin0[ 1 ].setValue( uniformRgb );
		uniformsMultiin0.add();
		uniformsMultiin0[ 2 ].setValue( uniformUser );
		uniformsMultiin0.add();
		uniformsMultiin0[ 3 ].setValue( uniformMat );

		InletHandle attribsMultiin0 = renderDataCombiner0.getInletHandle( "AttributeDescriptions" );
		attribsMultiin0.add();
		attribsMultiin0[ 0 ].setValue( attrib0 );
		attribsMultiin0.add();
		attribsMultiin0[ 1 ].setValue( attrib1 );

		InletHandle displayMultiin0 = displayWindow0.getInletHandle( "RenderData" );
		displayMultiin0.add();
		displayMultiin0[ 0 ].link( renderDataCombiner0.getOutletHandle( "RenderData" ) );

		InletHandle buffersMultiin1 = renderDataCombiner1.getInletHandle( "Buffers" );
		buffersMultiin1.add();
		buffersMultiin1[ 0 ].link( verticesToBuffer.getOutletHandle( "Buffer" ) );
		buffersMultiin1.add();
		buffersMultiin1[ 1 ].link( texcoordsToBuffer.getOutletHandle( "Buffer" ) );

		InletHandle texturesMultiin1 = renderDataCombiner1.getInletHandle( "Textures" );
		texturesMultiin1.add();
		texturesMultiin1[ 0 ].link( depthToTexture.getOutletHandle( "Texture" ) );
		texturesMultiin1.add();
		texturesMultiin1[ 1 ].link( eqdepthToTexture.getOutletHandle( "Texture" ) );
		texturesMultiin1.add();
		texturesMultiin1[ 2 ].link( userToTexture.getOutletHandle( "Texture" ) );

		InletHandle uniformsMultiin1 = renderDataCombiner1.getInletHandle( "UniformValues" );
		uniformsMultiin1.add();
		uniformsMultiin1[ 0 ].setValue( uniformDepth );
		uniformsMultiin1.add();
		uniformsMultiin1[ 1 ].setValue( uniformRgb );
		uniformsMultiin1.add();
		uniformsMultiin1[ 2 ].setValue( uniformUser );
		uniformsMultiin1.add();
		uniformsMultiin1[ 3 ].setValue( uniformMat );

		InletHandle attribsMultiin1 = renderDataCombiner1.getInletHandle( "AttributeDescriptions" );
		attribsMultiin1.add();
		attribsMultiin1[ 0 ].setValue( attrib0 );
		attribsMultiin1.add();
		attribsMultiin1[ 1 ].setValue( attrib1 );

		InletHandle displayMultiin1 = displayWindow1.getInletHandle( "RenderData" );
		displayMultiin1.add();
		displayMultiin1[ 0 ].link( renderDataCombiner1.getOutletHandle( "RenderData" ) );

		while( 1 )
		{
			string line;
			char lineEnd = '\n';
			getline( cin, line, lineEnd );
			if ( line == "q" )
			{
				break;
			}
		}
	}
	catch( Exception &e )
	{
		cout << e.message() << endl;
	}
	catch ( ... )
	{
		cout << "unknown exception" << endl;
	}

	while( 1 )
	{
		string line;
		char lineEnd = '\n';
		getline( cin, line, lineEnd );
		if ( line == "q" )
		{
			break;
		}
	}

	return 0;
}
