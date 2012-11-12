#include "_2RealBundlesUnitTest.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "Eigen/Dense"
#include "Eigen/Geometry"

#include "Poco/Mutex.h"

#include <Windows.h>

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

int main( int argc, char *argv[] )
{
	try
	{
		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "..\\..\\bin" );

		BundleHandle sfmlBundle = engine.loadBundle( "SFMLBundle" );
		BundleHandle kinectBundle = engine.loadBundle( "KinectOpenNIBundle" );

		//engine.loadConfig( "opengl.xml" );

		BlockHandle depthImage, rgbImage, userSkeleton, fullscreenMesh;
		BlockHandle depthToBuffer, rgbToTexture, userToTexture;
		BlockHandle pointCloudCombiner, displayPointCloud;

		InletHandle pointCloudBuffers, pointCloudBuffer1, pointCloudBuffer2, pointCloudBufferIndices;
		InletHandle pointCloudTextures, pointCloudTexture1, pointCloudTexture2;
		InletHandle pointCloudAttribs, pointCloudAttrib1, pointCloudAttrib2, pointCloudAttribIndices;
		InletHandle pointCloudUniforms, pointCloudSamplerRgb, pointCloudSamplerUser, pointCloudMatModel, pointCloudMatView, pointCloudMatProj;
		InletHandle pointCloudVertexSrc, pointCloudFragmentSrc, pointCloudGeometrySrc;
		InletHandle displayRenderData, displayRenderData1;
		InletHandle displayClearColor;
		InletHandle rgbData, userData, depthData, texcoordData, indexData;
		InletHandle depthAligned, depthRealWorld, depth16Bit;
		InletHandle userAligned;
		InletHandle meshWidth, meshHeight, primType;

		OutletHandle rgbImageOut, userImageOut, depthImageOut, depthBufferOut, texcoordBufferOut, indexBufferOut;
		OutletHandle rgbTextureOut, userTextureOut, renderDataOut, matModelOut, matViewOut, matProjOut;
		OutletHandle depthWidth, depthHeight;

		string uniformRgb = "texRgb ( 0 )";
		string uniformUser = "texUser ( 1 )";
		string attribIndices = "indices ()";
		string attribPosition = "position ( 3 0 )";
		string attribTexcoord = "texcoord ( 2 0 )";

		depthImage = kinectBundle.createBlockInstance( "KinectOpenNIDepthBlock" );
		rgbImage = kinectBundle.createBlockInstance( "KinectOpenNIRgbBlock" );
		userSkeleton = kinectBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );
		rgbToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		userToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		depthToBuffer = sfmlBundle.createBlockInstance( "ImageToBufferBlock" );
		fullscreenMesh = sfmlBundle.createBlockInstance( "FullscreenMeshBlock" );
		pointCloudCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
		displayPointCloud = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );

		depthImage.setup();
		rgbImage.setup();
		userSkeleton.setup();
		rgbToTexture.setup();
		userToTexture.setup();
		depthToBuffer.setup();
		fullscreenMesh.setup();
		pointCloudCombiner.setup();
		displayPointCloud.setup();

		//depthImage.setUpdateRate( 1.0 );
		//rgbImage.setUpdateRate( 1.0 );
		//userSkeleton.setUpdateRate( 1.0 );
		//rgbToTexture.setUpdateRate( 0.1 );
		//userToTexture.setUpdateRate( 0.1 );
		//depthToBuffer.setUpdateRate( 0.1 );
		//fullscreenMesh.setUpdateRate( 0.1 );
		//pointCloudCombiner.setUpdateRate( 0.1 );
		//displayPointCloud.setUpdateRate( 0.1 );

		pointCloudBuffers = pointCloudCombiner.getInletHandle( "Buffers" );
		pointCloudBufferIndices = pointCloudBuffers.add();
		pointCloudBuffer1 = pointCloudBuffers.add();
		pointCloudBuffer2 = pointCloudBuffers.add();
		pointCloudTextures = pointCloudCombiner.getInletHandle( "Textures" );
		pointCloudTexture1 = pointCloudTextures.add();
		pointCloudTexture2 = pointCloudTextures.add();
		pointCloudUniforms = pointCloudCombiner.getInletHandle( "UniformValues" );
		pointCloudSamplerRgb = pointCloudUniforms.add();
		pointCloudSamplerUser = pointCloudUniforms.add();
		pointCloudMatModel = pointCloudUniforms.add();
		pointCloudMatView = pointCloudUniforms.add();
		pointCloudMatProj = pointCloudUniforms.add();
		pointCloudAttribs = pointCloudCombiner.getInletHandle( "AttributeDescriptions" );
		pointCloudAttribIndices = pointCloudAttribs.add();
		pointCloudAttrib1 = pointCloudAttribs.add();
		pointCloudAttrib2 = pointCloudAttribs.add();
		pointCloudVertexSrc = pointCloudCombiner.getInletHandle( "VertexShaderSource" );
		pointCloudFragmentSrc = pointCloudCombiner.getInletHandle( "FragmentShaderSource" );
		pointCloudGeometrySrc = pointCloudCombiner.getInletHandle( "GeometryShaderSource" );

		displayRenderData = displayPointCloud.getInletHandle( "RenderData" );
		displayRenderData1 = displayRenderData.add();
		displayClearColor = displayPointCloud.getInletHandle( "ClearColor" );

		rgbData = rgbToTexture.getInletHandle( "TextureData" );
		userData = userToTexture.getInletHandle( "TextureData" );
		depthData = depthToBuffer.getInletHandle( "BufferData" );

		depth16Bit = depthImage.getInletHandle( "Is16BitImage" );
		depthRealWorld = depthImage.getInletHandle( "IsPointCloud" );
		depthAligned = depthImage.getInletHandle( "IsAlignedToColor" );
		userAligned = userSkeleton.getInletHandle( "IsAlignedToColor" );

		rgbImageOut = rgbImage.getOutletHandle( "ImageData" );
		userImageOut = userSkeleton.getOutletHandle( "ImageData" );
		depthImageOut = depthImage.getOutletHandle( "ImageData" );
		depthBufferOut = depthToBuffer.getOutletHandle( "Buffer" );
		texcoordBufferOut = fullscreenMesh.getOutletHandle( "VertexTexcoords" );
		indexBufferOut = fullscreenMesh.getOutletHandle( "Indices" );
		rgbTextureOut = rgbToTexture.getOutletHandle( "Texture" );
		userTextureOut = userToTexture.getOutletHandle( "Texture" );
		renderDataOut = pointCloudCombiner.getOutletHandle( "RenderData" );
		matModelOut = displayPointCloud.getOutletHandle( "TransformationMatrix" );
		matViewOut = displayPointCloud.getOutletHandle( "ViewMatrix" );
		matProjOut = displayPointCloud.getOutletHandle( "ProjectionMatrix" );
		depthWidth = depthImage.getOutletHandle( "Width" );
		depthHeight = depthImage.getOutletHandle( "Height" );

		primType = fullscreenMesh.getInletHandle( "PrimitiveType" );
		meshWidth = fullscreenMesh.getInletHandle( "MeshWidth" );
		meshHeight = fullscreenMesh.getInletHandle( "MeshHeight" );

		rgbData.link( rgbImageOut );
		userData.link( userImageOut );
		depthData.link( depthImageOut );
		pointCloudBufferIndices.link( indexBufferOut );
		pointCloudBuffer1.link( depthBufferOut);
		pointCloudBuffer2.link( texcoordBufferOut );
		pointCloudTexture1.link( rgbTextureOut );
		pointCloudTexture2.link( userTextureOut );
		displayRenderData1.link( renderDataOut );
		pointCloudMatModel.link( matModelOut );
		pointCloudMatView.link( matViewOut );
		pointCloudMatProj.link( matProjOut );

		meshWidth.linkWithConversion( depthWidth );
		meshHeight.linkWithConversion( depthHeight );

		pointCloudVertexSrc.setValueToString( "pointcloud.vert" );
		pointCloudFragmentSrc.setValueToString( "pointcloud.frag" );
		pointCloudGeometrySrc.setValueToString( "pointcloud.geo" );
		pointCloudSamplerRgb.setValue( uniformRgb );
		pointCloudSamplerUser.setValue( uniformUser );
		pointCloudAttrib1.setValue( attribPosition );
		pointCloudAttrib2.setValue( attribTexcoord );
		pointCloudAttribIndices.setValue( attribIndices );

		depth16Bit.setValue< bool >( false );
		depthRealWorld.setValue< bool >( true );
		depthAligned.setValue< bool >( true );
		userAligned.setValue< bool >( true );

		depthImage.start();
		rgbImage.start();
		userSkeleton.start();
		rgbToTexture.start();
		userToTexture.start();
		depthToBuffer.start();
		fullscreenMesh.start();
		pointCloudCombiner.start();
		displayPointCloud.start();

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

		//engine.safeConfig( "opengl.xml" );
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
