#include "_2RealBundle.h"
#include "OpenGl.h"

#include "RessourceManagerBlock.h"
#include "DisplayWindowBlock.h"
#include "OffscreenRenderBlock.h"
#include "TextureGeneratorBlock.h"
#include "BufferGeneratorBlock.h"

#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

template< typename TData >
TData * makeCheckerboard( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz )
{
	TData *data = new TData[ width * height * channels ];
	TData *p = data;
	for ( unsigned int i=0; i<height; ++i )
	{
		unsigned int divI = i/sz;
		for ( unsigned int j=0; j<width; ++j )
		{
			unsigned int divJ = j/sz;
			if ( ( divJ%2 == 0 ) ^ ( divI%2 == 0 ) )
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = static_cast< TData >( 255 );
					++p;
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = static_cast< TData >( 0 );
					++p;
				}
			}
		}
	}
	return data;
}

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "SFMLBundle" );
		info.setDescription( "xxxx" );
		info.setAuthor( "gerlinde emsenhuber" );
		info.setCategory( "xxxx" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		Vec4 vecClear;
		vecClear << 0.0, 0.0, 0.0, 1.0;

		unsigned char *init = makeCheckerboard< unsigned char >( 640, 480, 3, 80 );
		Image checkerImg( init, true, 640, 480, ImageChannelOrder::RGB );

		ContextBlockMetainfo mgr = info.exportContextBlock< RessourceManagerBlock >();
		mgr.setDescription( "xxxx" );

		BlockMetainfo displayWindow = info.exportBlock< DisplayWindowBlock, WithContext >( "DisplayWindowBlock" );
		displayWindow.setDescription( "xxxx" );
		displayWindow.setCategory( "xxxx" );
		displayWindow.addInlet< Vec4 >( "ClearColor", vecClear );
		displayWindow.addInlet< std::string >( "WindowTitle", "display window" );
		displayWindow.addMultiInlet< RenderData >( "RenderData", RenderData() );

		BlockMetainfo offscreenRenderer = info.exportBlock< OffscreenRenderBlock, WithContext >( "OffscreenRenderBlock" );
		offscreenRenderer.setDescription( "xxxx" );
		offscreenRenderer.setCategory( "xxxx" );
		offscreenRenderer.addInlet< Vec4 >( "ClearColor", vecClear );
		offscreenRenderer.addMultiInlet< RenderData >( "RenderData", RenderData() );
		offscreenRenderer.addOutlet< RenderData >( "RenderData" );

		BlockMetainfo imgSubtractionBlock = info.exportBlock< ImageSubtractionBlock, WithContext >( "OffscreenImageSubtraction" );
		imgSubtractionBlock.setDescription( "xxxx" );
		imgSubtractionBlock.setCategory( "xxxx" );
		imgSubtractionBlock.addInlet< std::string >( "VertexShaderSrc", "" );
		imgSubtractionBlock.addInlet< std::string >( "FragmentShaderSrc", "" );
		imgSubtractionBlock.addInlet< RenderData >( "Minuend", RenderData() );
		imgSubtractionBlock.addInlet< RenderData >( "Subtrahend", RenderData() );
		imgSubtractionBlock.addOutlet< RenderData >( "Difference" );

		BlockMetainfo randTex2D = info.exportBlock< RandomTexture2DBlock, WithContext >( "RandomTexture2DBlock" );
		randTex2D.setDescription( "test block, creates random texture" );
		randTex2D.setCategory( "xxxx" );
		randTex2D.addInlet< unsigned int >( "TextureWidth", 4 );
		randTex2D.addInlet< unsigned int >( "TextureHeight", 3 );
		randTex2D.addOutlet< RenderData >( "RenderData" );

		BlockMetainfo imageToTexture2D = info.exportBlock< ImageToTexture2DBlock, WithContext >( "ImageToTexture2DBlock" );
		imageToTexture2D.setDescription( "xxxx" );
		imageToTexture2D.setCategory( "xxxx" );
		imageToTexture2D.addInlet< Image >( "ImageData", checkerImg );
		imageToTexture2D.addOutlet< RenderData >( "RenderData" );
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}