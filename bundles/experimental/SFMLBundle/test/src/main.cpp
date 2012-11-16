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

		InletHandle pType = pointCloudCombiner.getInletHandle( "PrimitiveType" );

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
		pType.setValue< int >( 2 );

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

//#include "_2RealBundlesUnitTest.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//
//#include "Eigen/Dense"
//#include "Eigen/Geometry"
//
//#include "Poco/Mutex.h"
//
//#include <Windows.h>
//
//using namespace std;
//using namespace _2Real;
//using namespace _2Real::app;
//
//int main( int argc, char *argv[] )
//{
//	try
//	{
//		Engine &engine = Engine::instance();
//		engine.setBaseDirectory( "..\\..\\bin" );
//
//		BundleHandle sfmlBundle = engine.loadBundle( "SFMLBundle" );
//		BundleHandle kinectBundle = engine.loadBundle( "KinectOpenNIBundle" );
//
//		BlockHandle skeletonGenerator, boneCombiner;
//		BlockHandle skeletonToBuffer;
//		BlockHandle skeletonCombiner, displaySkeletons;
//
//		InletHandle skeletonBuffers;
//		InletHandle skeletonTextures;
//		InletHandle skeletonAttribs, skeletonAttribPosition;
//		InletHandle skeletonUniforms;
//		InletHandle skeletonVertexSrc, skeletonFragmentSrc, skeletonGeometrySrc;
//		InletHandle skeletonsIn, renderDataIn;
//		InletHandle skeletonDataIn, skeletonBufferIn;
//		InletHandle bonesIn;
//		InletHandle boneAttribs, boneAttribPosition, boneAttribIndices;
//		InletHandle boneBuffers, pointBufferIn, boneBufferIn;
//		InletHandle boneVertexSrc, boneFragmentSrc, boneGeometrySrc;
//		InletHandle bonePrimType;
//
//		OutletHandle skeletonDataOut, skeletonBufferOut, skeletonsOut;
//		OutletHandle boneBufferOut, bonesOut;
//
//		string attribIndices = "indices ()";
//		string attribPosition = "position ( 3 0 )";
//
//		skeletonGenerator = kinectBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );
//		skeletonToBuffer = sfmlBundle.createBlockInstance( "SkeletonToBufferBlock" );
//		skeletonCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
//		boneCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
//		displaySkeletons = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
//
//		skeletonGenerator.setup();
//		skeletonToBuffer.setup();
//		skeletonCombiner.setup();
//		boneCombiner.setup();
//		displaySkeletons .setup();
//
//		skeletonDataIn = skeletonToBuffer.getInletHandle( "BufferData" );
//
//		skeletonBuffers = skeletonCombiner.getInletHandle( "Buffers" );
//		skeletonBufferIn = skeletonBuffers.add();
//		skeletonAttribs = skeletonCombiner.getInletHandle( "AttributeDescriptions" );
//		skeletonAttribPosition = skeletonAttribs.add();
//		skeletonVertexSrc = skeletonCombiner.getInletHandle( "VertexShaderSource" );
//		skeletonFragmentSrc = skeletonCombiner.getInletHandle( "FragmentShaderSource" );
//		skeletonGeometrySrc = skeletonCombiner.getInletHandle( "GeometryShaderSource" );
//
//		boneBuffers = boneCombiner.getInletHandle( "Buffers" );
//		boneBufferIn = boneBuffers.add();
//		pointBufferIn = boneBuffers.add();
//		boneAttribs = boneCombiner.getInletHandle( "AttributeDescriptions" );
//		boneAttribIndices = boneAttribs.add();
//		boneAttribPosition = boneAttribs.add();
//		boneVertexSrc = boneCombiner.getInletHandle( "VertexShaderSource" );
//		boneFragmentSrc = boneCombiner.getInletHandle( "FragmentShaderSource" );
//		boneGeometrySrc = boneCombiner.getInletHandle( "GeometryShaderSource" );
//		bonePrimType = boneCombiner.getInletHandle( "PrimitiveType" );
//
//		renderDataIn = displaySkeletons.getInletHandle( "RenderData" );
//		skeletonsIn = renderDataIn.add();
//		bonesIn = renderDataIn.add();
//
//		skeletonDataOut = skeletonGenerator.getOutletHandle( "Skeletons" );
//		skeletonBufferOut = skeletonToBuffer.getOutletHandle( "VertexBuffer" );
//		boneBufferOut = skeletonToBuffer.getOutletHandle( "BoneBuffer" );
//		skeletonsOut = skeletonCombiner.getOutletHandle( "RenderData" );
//		bonesOut = boneCombiner.getOutletHandle( "RenderData" );
//
//		skeletonVertexSrc.setValueToString( "skeleton.vert" );
//		skeletonFragmentSrc.setValueToString( "skeleton.frag" );
//		skeletonAttribPosition.setValue( attribPosition );
//
//		boneVertexSrc.setValueToString( "skeleton.vert" );
//		boneFragmentSrc.setValueToString( "skeleton.frag" );
//		boneAttribIndices.setValue( attribIndices );
//		boneAttribPosition.setValue( attribPosition );
//		bonePrimType.setValue< int >( 1 );
//
//		skeletonDataIn.link( skeletonDataOut );
//		skeletonBufferIn.link( skeletonBufferOut );
//		skeletonsIn.link( skeletonsOut );
//
//		boneBufferIn.link( boneBufferOut );
//		pointBufferIn.link( skeletonBufferOut );
//		bonesIn.link( bonesOut );
//
//		skeletonGenerator.start();
//		skeletonToBuffer.start();
//		skeletonCombiner.start();
//		boneCombiner.start();
//		displaySkeletons.start();
//
//		while( 1 )
//		{
//			string line;
//			char lineEnd = '\n';
//			getline( cin, line, lineEnd );
//			if ( line == "q" )
//			{
//				break;
//			}
//		}
//	}
//	catch( Exception &e )
//	{
//		cout << e.message() << endl;
//	}
//	catch ( ... )
//	{
//		cout << "unknown exception" << endl;
//	}
//
//	while( 1 )
//	{
//		string line;
//		char lineEnd = '\n';
//		getline( cin, line, lineEnd );
//		if ( line == "q" )
//		{
//			break;
//		}
//	}
//
//	return 0;
//}
