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
//		BlockHandle depthImage = kinectBundle.createBlockInstance( "KinectOpenNIDepthBlock" );
//		BlockHandle rgbImage = kinectBundle.createBlockInstance( "KinectOpenNIRgbBlock" );
//		BlockHandle userSkeleton = kinectBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );
//		BlockHandle rgbToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
//		BlockHandle userToTexture = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
//		BlockHandle depthToBuffer = sfmlBundle.createBlockInstance( "ImageToBufferBlock" );
//		BlockHandle fullscreenMesh = sfmlBundle.createBlockInstance( "FullscreenMeshBlock" );
//		BlockHandle pointCloudCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
//		BlockHandle displayPointCloud = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
//
//		depthImage.setup();
//		rgbImage.setup();
//		userSkeleton.setup();
//		rgbToTexture.setup();
//		userToTexture.setup();
//		depthToBuffer.setup();
//		fullscreenMesh.setup();
//		pointCloudCombiner.setup();
//		displayPointCloud.setup();
//
//		InletHandle pointCloudBuffers = pointCloudCombiner.getInletHandle( "Buffers" );
//		InletHandle pointCloudBufferIndices = pointCloudBuffers.add();
//		InletHandle pointCloudBufferPositions = pointCloudBuffers.add();
//		InletHandle pointCloudBufferTexcoords = pointCloudBuffers.add();
//		InletHandle pointCloudTextures = pointCloudCombiner.getInletHandle( "Textures" );
//		InletHandle pointCloudTextureRgb = pointCloudTextures.add();
//		InletHandle pointCloudTextureUser = pointCloudTextures.add();
//		InletHandle pointCloudUniforms = pointCloudCombiner.getInletHandle( "UniformValues" );
//		InletHandle pointCloudSamplerRgb = pointCloudUniforms.add();
//		InletHandle pointCloudSamplerUser = pointCloudUniforms.add();
//		InletHandle pointCloudMatModel = pointCloudUniforms.add();
//		InletHandle pointCloudMatView = pointCloudUniforms.add();
//		InletHandle pointCloudMatProj = pointCloudUniforms.add();
//		InletHandle pointCloudAttribs = pointCloudCombiner.getInletHandle( "AttributeDescriptions" );
//		InletHandle pointCloudAttribIndices = pointCloudAttribs.add();
//		InletHandle pointCloudAttribPositions = pointCloudAttribs.add();
//		InletHandle pointCloudAttribTexcoords = pointCloudAttribs.add();
//		InletHandle pointCloudVertexSrc = pointCloudCombiner.getInletHandle( "VertexShaderSource" );
//		InletHandle pointCloudFragmentSrc = pointCloudCombiner.getInletHandle( "FragmentShaderSource" );
//		InletHandle pointCloudGeometrySrc = pointCloudCombiner.getInletHandle( "GeometryShaderSource" );
//		InletHandle pointCloudDrawType = pointCloudCombiner.getInletHandle( "PrimitiveType" );
//
//		InletHandle displayRenderData = displayPointCloud.getInletHandle( "RenderData" );
//		InletHandle displayRenderData1 = displayRenderData.add();
//
//		InletHandle rgbData = rgbToTexture.getInletHandle( "TextureData" );
//		InletHandle userData = userToTexture.getInletHandle( "TextureData" );
//		InletHandle depthData = depthToBuffer.getInletHandle( "BufferData" );
//
//		InletHandle primType = fullscreenMesh.getInletHandle( "PrimitiveType" );
//		InletHandle meshWidth = fullscreenMesh.getInletHandle( "MeshWidth" );
//		InletHandle meshHeight = fullscreenMesh.getInletHandle( "MeshHeight" );
//
//		OutletHandle rgbImageOut = rgbImage.getOutletHandle( "ImageData" );
//		OutletHandle userImageOut = userSkeleton.getOutletHandle( "ImageData" );
//		OutletHandle depthImageOut = depthImage.getOutletHandle( "ImageData" );
//		OutletHandle depthBufferOut = depthToBuffer.getOutletHandle( "Buffer" );
//		OutletHandle texcoordBufferOut = fullscreenMesh.getOutletHandle( "VertexTexcoords" );
//		OutletHandle indexBufferOut = fullscreenMesh.getOutletHandle( "Indices" );
//
//		OutletHandle rgbTextureOut = rgbToTexture.getOutletHandle( "Texture" );
//		OutletHandle userTextureOut = userToTexture.getOutletHandle( "Texture" );
//		OutletHandle renderDataOut = pointCloudCombiner.getOutletHandle( "RenderData" );
//		OutletHandle matModelOut = displayPointCloud.getOutletHandle( "TransformationMatrix" );
//		OutletHandle matViewOut = displayPointCloud.getOutletHandle( "ViewMatrix" );
//		OutletHandle matProjOut = displayPointCloud.getOutletHandle( "ProjectionMatrix" );
//
//		OutletHandle depthWidth = depthImage.getOutletHandle( "Width" );
//		OutletHandle depthHeight = depthImage.getOutletHandle( "Height" );
//
//		// link converters + generators
//
//		// rgb to tex + rgb gen
//		rgbData.link( rgbImageOut );
//		// user to tex + user gen
//		userData.link( userImageOut );
//		// depth to buffer + depth gen
//		depthData.link( depthImageOut );
//
//		// link combiner multiinlets + converters
//
//		// fullscreen mesh gen -> indices
//		pointCloudBufferIndices.link( indexBufferOut );
//		// depth to buffer -> positions
//		pointCloudBufferPositions.link( depthBufferOut);
//		// fullscreen mesh gen -> texcoords
//		pointCloudBufferTexcoords.link( texcoordBufferOut );
//		// rgb tex first
//		pointCloudTextureRgb.link( rgbTextureOut );
//		// user tex second
//		pointCloudTextureUser.link( userTextureOut );
//
//		// matrices
//		pointCloudMatModel.link( matModelOut );
//		pointCloudMatView.link( matViewOut );
//		pointCloudMatProj.link( matProjOut );
//
//		// window + combiner
//		displayRenderData1.link( renderDataOut );
//
//		// fullscreen mesh: width / height come from depth block
//		// primtype: triangles
//		meshWidth.linkWithConversion( depthWidth );
//		meshHeight.linkWithConversion( depthHeight );
//		primType.setValue< int >( 2 );
//
//		// combiner: shader sources, uniforms, attribs
//		string uniformRgb = "texRgb ( 0 )";
//		string uniformUser = "texUser ( 1 )";
//		string attribIndices = "indices ()";
//		string attribPosition = "position ( 3 0 )";
//		string attribTexcoord = "texcoord ( 2 0 )";
//		pointCloudVertexSrc.setValueToString( "pointcloud.vert" );
//		pointCloudFragmentSrc.setValueToString( "pointcloud.frag" );
//		pointCloudGeometrySrc.setValueToString( "pointcloud.geo" );
//		pointCloudSamplerRgb.setValue( uniformRgb );
//		pointCloudSamplerUser.setValue( uniformUser );
//		pointCloudAttribIndices.setValue( attribIndices );
//		pointCloudAttribPositions.setValue( attribPosition );
//		pointCloudAttribTexcoords.setValue( attribTexcoord );
//
//		// kinect values: depth->real, depth->not 16 bit, depth->aligned to color, user->aligned to color
//		InletHandle depth16Bit = depthImage.getInletHandle( "Is16BitImage" );
//		InletHandle depthRealWorld = depthImage.getInletHandle( "IsPointCloud" );
//		InletHandle depthAligned = depthImage.getInletHandle( "IsAlignedToColor" );
//		InletHandle userAligned = userSkeleton.getInletHandle( "IsAlignedToColor" );
//		depth16Bit.setValue< bool >( false );
//		depthRealWorld.setValue< bool >( true );
//		depthAligned.setValue< bool >( true );
//		userAligned.setValue< bool >( true );
//
//		// set prim type of combiner to triangles
//		pointCloudDrawType.setValue< int >( 2 );
//
//		depthImage.start();
//		rgbImage.start();
//		userSkeleton.start();
//		rgbToTexture.start();
//		userToTexture.start();
//		depthToBuffer.start();
//		fullscreenMesh.start();
//		pointCloudCombiner.start();
//		displayPointCloud.start();
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
//
//		//engine.safeConfig( "opengl.xml" );
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
//		string attribIndices = "indices ()";
//		string attribPosition = "position ( 3 0 )";
//
//		BlockHandle skeletonGenerator = kinectBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );
//		BlockHandle skeletonToBuffer = sfmlBundle.createBlockInstance( "SkeletonToBufferBlock" );
//		BlockHandle skeletonCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
//		BlockHandle boneCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
//		BlockHandle displaySkeletons = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
//
//		skeletonGenerator.setup();
//		skeletonToBuffer.setup();
//		skeletonCombiner.setup();
//		boneCombiner.setup();
//		displaySkeletons .setup();
//
//		InletHandle skeletonDataIn = skeletonToBuffer.getInletHandle( "BufferData" );
//
//		InletHandle skeletonBuffers = skeletonCombiner.getInletHandle( "Buffers" );
//		InletHandle skeletonBufferIn = skeletonBuffers.add();
//		InletHandle skeletonAttribs = skeletonCombiner.getInletHandle( "AttributeDescriptions" );
//		InletHandle skeletonAttribPosition = skeletonAttribs.add();
//		InletHandle skeletonVertexSrc = skeletonCombiner.getInletHandle( "VertexShaderSource" );
//		InletHandle skeletonFragmentSrc = skeletonCombiner.getInletHandle( "FragmentShaderSource" );
//		InletHandle skeletonGeometrySrc = skeletonCombiner.getInletHandle( "GeometryShaderSource" );
//		InletHandle skeletonPrimType = skeletonCombiner.getInletHandle( "PrimitiveType" );
//
//		InletHandle boneBuffers = boneCombiner.getInletHandle( "Buffers" );
//		InletHandle boneBufferIn = boneBuffers.add();
//		InletHandle pointBufferIn = boneBuffers.add();
//		InletHandle boneAttribs = boneCombiner.getInletHandle( "AttributeDescriptions" );
//		InletHandle boneAttribIndices = boneAttribs.add();
//		InletHandle boneAttribPosition = boneAttribs.add();
//		InletHandle boneVertexSrc = boneCombiner.getInletHandle( "VertexShaderSource" );
//		InletHandle boneFragmentSrc = boneCombiner.getInletHandle( "FragmentShaderSource" );
//		InletHandle boneGeometrySrc = boneCombiner.getInletHandle( "GeometryShaderSource" );
//		InletHandle bonePrimType = boneCombiner.getInletHandle( "PrimitiveType" );
//
//		InletHandle renderDataIn = displaySkeletons.getInletHandle( "RenderData" );
//		InletHandle skeletonsIn = renderDataIn.add();
//		InletHandle bonesIn = renderDataIn.add();
//
//		OutletHandle skeletonDataOut = skeletonGenerator.getOutletHandle( "Skeletons" );
//		OutletHandle skeletonBufferOut = skeletonToBuffer.getOutletHandle( "VertexBuffer" );
//		OutletHandle boneBufferOut = skeletonToBuffer.getOutletHandle( "BoneBuffer" );
//		OutletHandle skeletonsOut = skeletonCombiner.getOutletHandle( "RenderData" );
//		OutletHandle bonesOut = boneCombiner.getOutletHandle( "RenderData" );
//
//		skeletonVertexSrc.setValueToString( "skeleton.vert" );
//		skeletonFragmentSrc.setValueToString( "skeleton.frag" );
//		skeletonAttribPosition.setValue( attribPosition );
//		skeletonPrimType.setValue< int >( 0 );
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

int main( int argc, char *argv[] )
{
	try
	{
		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "..\\..\\bin" );

		BundleHandle sfmlBundle = engine.loadBundle( "SFMLBundle" );

		//engine.loadConfig( "modelimport.xml" );

		string filePath = "D:\\opengl\\data\\dragon.ply";

		string vertexSrcBunny = "modelshaded.vert";
		string fragmentSrcBunny = "modelshaded.frag";

		string attribIndices = "indices ()";
		string attribPosition = "position ( 3 0 )";
		string attribNormal = "normal ( 3 0 )";

		BlockHandle bunnyImporter = sfmlBundle.createBlockInstance( "ModelImporterBlock" );
		bunnyImporter.setup();
		bunnyImporter.setUpdateRate( 30.0 );
		bunnyImporter.start();
		bunnyImporter.getInletHandle( "FilePath" ).setValue( filePath );

		BlockHandle bunnyCombiner = sfmlBundle.createBlockInstance( "RenderDataCombinerBlock" );
		bunnyCombiner.setup();
		bunnyCombiner.setUpdateRate( 30.0 );
		bunnyCombiner.start();
		bunnyCombiner.getInletHandle( "VertexShaderSource" ).setValue( vertexSrcBunny );
		bunnyCombiner.getInletHandle( "FragmentShaderSource" ).setValue( fragmentSrcBunny );

		BlockHandle bunnyDisplay = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
		bunnyDisplay.setup();
		bunnyDisplay.setUpdateRate( 30.0 );
		bunnyDisplay.start();

		InletHandle bunnyBuffers = bunnyCombiner.getInletHandle( "Buffers" );
		bunnyBuffers.add();
		bunnyBuffers[ 0 ].link( bunnyImporter.getOutletHandle( "Indices" ) );
		bunnyBuffers.add();
		bunnyBuffers[ 1 ].link( bunnyImporter.getOutletHandle( "VertexPositions" ) );
		bunnyBuffers.add();
		bunnyBuffers[ 2 ].link( bunnyImporter.getOutletHandle( "VertexNormals" ) );

		InletHandle bunnyAttribs = bunnyCombiner.getInletHandle( "AttributeDescriptions" );
		bunnyAttribs.add();
		bunnyAttribs[ 0 ].setValue( attribIndices );
		bunnyAttribs.add();
		bunnyAttribs[ 1 ].setValue( attribPosition );
		bunnyAttribs.add();
		bunnyAttribs[ 2 ].setValue( attribNormal );

		InletHandle bunnyUniforms = bunnyCombiner.getInletHandle( "UniformValues" );
		InletHandle bunnyMatModel = bunnyUniforms.add();
		bunnyMatModel.link( bunnyDisplay.getOutletHandle( "TransformationMatrix" ) );
		InletHandle bunnyMatView = bunnyUniforms.add();
		bunnyMatView.link( bunnyDisplay.getOutletHandle( "ViewMatrix" ) );
		InletHandle bunnyMatProj = bunnyUniforms.add();
		bunnyMatProj.link( bunnyDisplay.getOutletHandle( "ProjectionMatrix" ) );

		InletHandle bunnyData = bunnyDisplay.getInletHandle( "RenderData" );
		InletHandle bunnyData0 = bunnyData.add();
		bunnyData0.link( bunnyCombiner.getOutletHandle( "RenderData" ) );

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

		//engine.safeConfig( "modelimport.xml" );
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
