#include "_2RealBundle.h"

#include "RessourceManagerBlock.h"
#include "DisplayWindowBlock.h"
//#include "OffscreenRenderBlock.h"
#include "ModelLoaderBlock.h"
#include "GeometryBlock.h"
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
		mgr.setDescription( "opengl ressource mgr, responsible for deletion of opengl objects" );

		/**
		*	TODO: argh
		*	-not only clear color, but also clear depth ( ? )
		*	-allow user to specify size ( ! )
		*	-allow user to specify context settings ( ? )
		*	-interaction with window:	mouse, keyboard etc
		*	-should this thing actually be capable of rendering, or should everything be done in offscreen renderer?
		**/
		BlockMetainfo displayWindow = info.exportBlock< DisplayWindowBlock, WithContext >( "DisplayWindowBlock" );
		displayWindow.setDescription( "a simple display window, capable of rendering" );
		displayWindow.setCategory( "rendering" );
		displayWindow.addInlet< Vec4 >( "ClearColor", vecClear );
		displayWindow.addInlet< string >( "WindowTitle", "display window" );
		displayWindow.addInlet< unsigned int >( "WindowWidth", 1200 );
		displayWindow.addInlet< unsigned int >( "WindowHeight", 900 );
		displayWindow.addMultiInlet< RenderData >( "RenderData", RenderData() );
		displayWindow.addInlet< bool >( "EnableMouseInteraction", true );
		displayWindow.addInlet< bool >( "EnableKeyboardInteraction", true );
		displayWindow.addOutlet< string >( "TransformationMatrix" );
		displayWindow.addOutlet< string >( "ViewMatrix" );
		displayWindow.addOutlet< string >( "ProjectionMatrix" );

		/**
		*	TODO: offscreen renderer
		**/
		
		/**
		*	this is a debug block, basically
		**/
		BlockMetainfo randTex2D = info.exportBlock< RandomTexture2DBlock, WithContext >( "RandomTexture2DBlock" );
		randTex2D.setDescription( "creates a random 2D texture of a specified size" );
		randTex2D.setCategory( "rendering" );
		randTex2D.addInlet< unsigned int >( "TextureWidth", 4 );
		randTex2D.addInlet< unsigned int >( "TextureHeight", 3 );
		randTex2D.addOutlet< Texture >( "Texture" );

		/**
		*	TODO: imageTs
		**/
		BlockMetainfo imageToTexture2D = info.exportBlock< ImageToTexture2DBlock, WithContext >( "ImageToTexture2DBlock" );
		imageToTexture2D.setDescription( "transforms an image into a 2D texture" );
		imageToTexture2D.setCategory( "rendering" );
		imageToTexture2D.addInlet< Image >( "TextureData", checkerImg );
		imageToTexture2D.addOutlet< Texture >( "Texture" );

		/**
		*	TODO: not only vectors, but also other stuff? ( assimp returns pointers... )
		**/
		BlockMetainfo floatVectorToBuffer = info.exportBlock< VectorToBufferBlock< float >, WithContext >( "FloatVectorToBufferBlock" );
		floatVectorToBuffer.setDescription( "transforms a std::vector of floats into a vertex buffer" );
		floatVectorToBuffer.setCategory( "rendering" );
		floatVectorToBuffer.addInlet< vector< float > >( "BufferData", vector< float >() );
		floatVectorToBuffer.addOutlet< Buffer >( "Buffer" );

		BlockMetainfo vectorToBuffer = info.exportBlock< VectorToBufferBlock< unsigned int >, WithContext >( "UintVectorToBufferBlock" );
		vectorToBuffer.setDescription( "transforms a std::vector of uints into a vertex buffer" );
		vectorToBuffer.setCategory( "rendering" );
		vectorToBuffer.addInlet< vector< unsigned int > >( "BufferData", vector< unsigned int >() );
		vectorToBuffer.addOutlet< Buffer >( "Buffer" );

		BlockMetainfo imgToBuffer = info.exportBlock< ImageToBufferBlock, WithContext >( "ImageToBufferBlock" );
		imgToBuffer.setDescription( "transforms an img of floats into a vertex buffer" );
		imgToBuffer.setCategory( "rendering" );
		imgToBuffer.addInlet< Image >( "BufferData", Image() );
		imgToBuffer.addOutlet< Buffer >( "Buffer" );

		// vector of skeletons :/
		BlockMetainfo skeletonsToBuffer = info.exportBlock< SkeletonsToBufferBlock, WithContext >( "SkeletonToBufferBlock" );
		skeletonsToBuffer.setDescription( "transforms a vector of skeletons into a vertex buffer" );
		skeletonsToBuffer.setCategory( "rendering" );
		skeletonsToBuffer.addInlet< vector< Skeleton > >( "BufferData", vector< Skeleton >() );
		skeletonsToBuffer.addOutlet< Buffer >( "VertexBuffer" );
		skeletonsToBuffer.addOutlet< Buffer >( "BoneBuffer" );

		/**
		*	TODO: attrib & unforms are currently defined via strings...
		**/
		BlockMetainfo dataGenerator = info.exportBlock< RenderDataCombinerBlock, WithContext >( "RenderDataCombinerBlock" );
		dataGenerator.setDescription( "combines buffers, textures & glsl code into a datatype that can be rendered by either a display window or an offscreen renderer" );
		dataGenerator.setCategory( "rendering" );
		dataGenerator.addInlet< int >( "PrimitiveType", 2, PrimitiveType::getPrimitiveOptions() );
		dataGenerator.addOutlet< RenderData >( "RenderData" );
		dataGenerator.addMultiInlet< Buffer >( "Buffers", Buffer() );
		dataGenerator.addMultiInlet< Texture >( "Textures", Texture() );
		dataGenerator.addMultiInlet< string >( "UniformValues", string() );
		dataGenerator.addMultiInlet< string >( "AttributeDescriptions", string() );
		dataGenerator.addInlet< string >( "VertexShaderSource", string() );
		dataGenerator.addInlet< string >( "GeometryShaderSource", string() );
		dataGenerator.addInlet< string >( "FragmentShaderSource", string() );

		/**
		*	TODO: not even sure this should be in here?
		*	filepath is currently a string -> switch to path class in future
		**/
		BlockMetainfo modelImporter = info.exportBlock< ModelLoaderBlock, WithContext >( "ModelImporterBlock" );
		modelImporter.setDescription( "loads a model file via assimp" );
		modelImporter.setCategory( "rendering" );
		modelImporter.addInlet< string >( "FilePath", string() );
		modelImporter.addOutlet< Buffer >( "Indices" );
		modelImporter.addOutlet< Buffer >( "VertexPositions" );
		modelImporter.addOutlet< Buffer >( "VertexNormals" );
		modelImporter.addOutlet< unsigned int >( "NumberOfMeshes" );

		BlockMetainfo fullscreenMeshGenerator = info.exportBlock< FullscreenMeshBlock, WithContext >( "FullscreenMeshBlock" );
		fullscreenMeshGenerator.setDescription( "calculates a fullscrren mesh" );
		fullscreenMeshGenerator.setCategory( "rendering" );
		fullscreenMeshGenerator.addInlet< unsigned int >( "MeshWidth", 2 );
		fullscreenMeshGenerator.addInlet< unsigned int >( "MeshHeight", 2 );
		fullscreenMeshGenerator.addInlet< int >( "PrimitiveType", 0 );
		fullscreenMeshGenerator.addOutlet< Buffer >( "VertexPositions" );
		fullscreenMeshGenerator.addOutlet< Buffer >( "VertexTexcoords" );
		fullscreenMeshGenerator.addOutlet< Buffer >( "Indices" );
	}
	catch ( Exception &e )
	{
		std::cout << e.message() << std::endl;
		e.rethrow();
	}
}