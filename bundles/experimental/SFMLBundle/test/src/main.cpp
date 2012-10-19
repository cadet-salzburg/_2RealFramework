#include "_2RealBundlesUnitTest.h"
#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::app;

void receiveNrOfSkeletons( void *, AppData const& data )
{
	std::cout << data.getData< int >() << std::endl;
}

int main( int argc, char *argv[] )
{
	try
	{
		Engine &engine = Engine::instance();
		engine.setBaseDirectory( "..\\..\\bin" );
		BundleHandle sfmlBundle = engine.loadBundle( "SFMLBundle" );
		BundleHandle kinectBundle = engine.loadBundle( "KinectOpenNIBundle" );
		BundleHandle camBundle = engine.loadBundle( "CameraCaptureBundle" );

		BlockHandle depthBlock = kinectBundle.createBlockInstance( "KinectOpenNIDepthBlock" );
		depthBlock.setup();
		depthBlock.start();
		depthBlock.setUpdateRate( 30.0 );
		depthBlock.getInletHandle( "Is16BitImage" ).setValue< bool >( false );
		depthBlock.getInletHandle( "IsAlignedToColor" ).setValue< bool >( true );

		BlockHandle rgbBlock = kinectBundle.createBlockInstance( "KinectOpenNIRgbBlock" );
		rgbBlock.setup();
		rgbBlock.start();
		rgbBlock.setUpdateRate( 30.0 );

		BlockHandle skeletonBlock = kinectBundle.createBlockInstance( "KinectOpenNIUserSkeletonBlock" );
		skeletonBlock.setup();
		skeletonBlock.start();
		skeletonBlock.setUpdateRate( 30.0 );

		BlockHandle displayWindow = sfmlBundle.createBlockInstance( "DisplayWindowBlock" );
		displayWindow.setup();
		displayWindow.start();
		displayWindow.setUpdateRate( 30.0 );

		BlockHandle offscreenRenderer = sfmlBundle.createBlockInstance( "OffscreenImageSubtraction" );
		offscreenRenderer.setup();
		offscreenRenderer.start();
		offscreenRenderer.setUpdateRate( 30.0 );

		BlockHandle imgToTexture0 = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		imgToTexture0.setup();
		imgToTexture0.start();
		imgToTexture0.setUpdateRate( 30.0 );

		BlockHandle imgToTexture1 = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		imgToTexture1.setup();
		imgToTexture1.start();
		imgToTexture1.setUpdateRate( 30.0 );

		BlockHandle imgToTexture2 = sfmlBundle.createBlockInstance( "ImageToTexture2DBlock" );
		imgToTexture2.setup();
		imgToTexture2.start();
		imgToTexture2.setUpdateRate( 30.0 );

		BlockHandle randomTexGen = sfmlBundle.createBlockInstance( "RandomTexture2DBlock" );
		randomTexGen.setup();
		randomTexGen.getInletHandle( "TextureWidth" ).setValue< unsigned int >( 8 );
		randomTexGen.getInletHandle( "TextureHeight" ).setValue< unsigned int >( 6 );
		randomTexGen.start();
		randomTexGen.setUpdateRate( 30.0 );

		OutletHandle outImgSkel = skeletonBlock.getOutletHandle( "ImageData" );
		InletHandle inImgTex0 = imgToTexture0.getInletHandle( "ImageData" );
		inImgTex0.setUpdatePolicy( InletPolicy::AND_NEWER_DATA );
		inImgTex0.link( outImgSkel );

		OutletHandle outImgDepth = depthBlock.getOutletHandle( "ImageData" );
		InletHandle inImgTex1 = imgToTexture1.getInletHandle( "ImageData" );
		inImgTex1.setUpdatePolicy( InletPolicy::AND_NEWER_DATA );
		inImgTex1.link( outImgDepth );

		OutletHandle outImgRgb = rgbBlock.getOutletHandle( "ImageData" );
		InletHandle inImgTex2 = imgToTexture2.getInletHandle( "ImageData" );
		inImgTex2.setUpdatePolicy( InletPolicy::AND_NEWER_DATA );
		inImgTex2.link( outImgRgb );

		InletHandle inRenderData = displayWindow.getInletHandle( "RenderData" );
		inRenderData.add();
		inRenderData.add();
		inRenderData.add();
		InletHandle in0 = inRenderData[ 0 ];
		InletHandle in1 = inRenderData[ 1 ];
		InletHandle in2 = inRenderData[ 2 ];

		InletHandle min = offscreenRenderer.getInletHandle( "Minuend" );
		InletHandle sub = offscreenRenderer.getInletHandle( "Subtrahend" );

		OutletHandle outRenderData0 = imgToTexture0.getOutletHandle( "RenderData" );
		OutletHandle outRenderData1 = imgToTexture1.getOutletHandle( "RenderData" );

		min.link( outRenderData0 );
		sub.link( outRenderData1 );

		OutletHandle outRenderData2 = imgToTexture2.getOutletHandle( "RenderData" );
		OutletHandle outRenderData3 = offscreenRenderer.getOutletHandle( "Difference" );
		OutletHandle outRenderData4 = randomTexGen.getOutletHandle( "RenderData" );

		in0.link( outRenderData2 );
		in1.link( outRenderData3 );
		//in2.link( outRenderData4 );

		OutletHandle outNrOfSkeletons = skeletonBlock.getOutletHandle( "NrOfSkeletons" );
		outNrOfSkeletons.registerToNewData( receiveNrOfSkeletons, nullptr );

		while( 1 )
		{
			string line;
			char lineEnd = '\n';
			getline( cin, line, lineEnd );
			if ( line == "quit" )
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
		if ( line == "quit" )
		{
			break;
		}
	}

	return 0;
}
