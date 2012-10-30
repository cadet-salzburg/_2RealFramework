#include "_2RealBundle.h"

#include "RessourceManagerBlock.h"
#include "DisplayWindowBlock.h"
//#include "OffscreenRenderBlock.h"
#include "RenderDataCombinerBlock.h"
#include "TextureGeneratorBlock.h"
#include "BufferGeneratorBlock.h"

#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;
using namespace std;

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
		displayWindow.addInlet< bool >( "EnableMouseInteraction", true );
		displayWindow.addInlet< bool >( "EnableKeyboardInteraction", true );

		BlockMetainfo randTex2D = info.exportBlock< RandomTexture2DBlock, WithContext >( "RandomTexture2DBlock" );
		randTex2D.setDescription( "xxxx" );
		randTex2D.setCategory( "xxxx" );
		randTex2D.addInlet< unsigned int >( "TextureWidth", 4 );
		randTex2D.addInlet< unsigned int >( "TextureHeight", 3 );
		randTex2D.addOutlet< Texture >( "Texture" );

		BlockMetainfo imageToTexture2D = info.exportBlock< ImageToTexture2DBlock, WithContext >( "ImageToTexture2DBlock" );
		imageToTexture2D.setDescription( "xxxx" );
		imageToTexture2D.setCategory( "xxxx" );
		imageToTexture2D.addInlet< Image >( "TextureData", checkerImg );
		imageToTexture2D.addOutlet< Texture >( "Texture" );

		BlockMetainfo vectorToBuffer = info.exportBlock< VectorToBufferBlock, WithContext >( "VectorToBufferBlock" );
		vectorToBuffer.setDescription( "xxxx" );
		vectorToBuffer.setCategory( "xxxx" );
		vectorToBuffer.addInlet< vector< float > >( "BufferData", vector< float >() );
		vectorToBuffer.addOutlet< Buffer >( "Buffer" );

		BlockMetainfo dataGenerator = info.exportBlock< RenderDataCombinerBlock, WithContext >( "RenderDataCombinerBlock" );
		dataGenerator.setDescription( "xxxx" );
		dataGenerator.setCategory( "xxxx" );
		dataGenerator.addOutlet< RenderData >( "RenderData" );
		dataGenerator.addMultiInlet< Buffer >( "Buffers", Buffer() );
		dataGenerator.addMultiInlet< Texture >( "Textures", Texture() );
		dataGenerator.addMultiInlet< string >( "UniformValues", string() );
		dataGenerator.addMultiInlet< string >( "AttributeDescriptions", string() );
		dataGenerator.addInlet< string >( "VertexShaderSource", string() );
		dataGenerator.addInlet< string >( "GeometryShaderSource", string() );
		dataGenerator.addInlet< string >( "FragmentShaderSource", string() );
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}