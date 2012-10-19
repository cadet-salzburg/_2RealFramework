#include "TextureGeneratorBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

RandomTexture2DBlock::RandomTexture2DBlock( ContextBlock &context ) :
	Block(), m_Manager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	m_Context( nullptr ), m_Renderer( nullptr )
{
	m_Texture.texture = m_Manager.createTexture( GL_TEXTURE_2D );
	m_TexCoords.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
	m_Positions.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
}

RandomTexture2DBlock::~RandomTexture2DBlock() {}

void RandomTexture2DBlock::setup( BlockHandle &block )
{
	try
	{
		m_BlockHandle = block;

		if ( m_Context == nullptr )
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

			m_Context = new RenderContext( settings );
			m_Renderer = &( m_Context->getRenderer() );

			std::vector< float > coords;
			coords.push_back( 0.0f );
			coords.push_back( 0.0f );
			coords.push_back( 0.5f );
			coords.push_back( 1.0f );
			coords.push_back( 0.8f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );
			coords.push_back( 0.0f );

			m_Renderer->updateBuffer( m_TexCoords.buffer, coords );
			m_TexCoords.normalized = GL_FALSE;
			m_TexCoords.offset = 0;
			m_TexCoords.size = 2;
			m_TexCoords.stride = 0;

			std::vector< float > vertices;
			vertices.push_back( -1.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( 0.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( 0.0f );
			vertices.push_back( -1.0f );
			vertices.push_back( -1.0f );
			vertices.push_back( 0.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( -1.0f );
			vertices.push_back( 0.0f );

			m_Renderer->updateBuffer( m_Positions.buffer, vertices );
			m_Positions.normalized = GL_FALSE;
			m_Positions.offset = 0;
			m_Positions.size = 3;
			m_Positions.stride = 0;

			m_Renderer->finish();
			m_Context->setActive( false );

			srand( static_cast< unsigned int >( time( NULL ) ) );
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

void RandomTexture2DBlock::update()
{
	try
	{
		unsigned int w = m_BlockHandle.getInletHandle( "TextureWidth" ).getReadableRef< unsigned int >();
		unsigned int h = m_BlockHandle.getInletHandle( "TextureHeight" ).getReadableRef< unsigned int >();

		m_Context->setActive( true );

		ImageT< unsigned char > img( w, h, ImageChannelOrder::RGBA );
		ImageT< unsigned char >::iterator it = img.iter();
		while( it.nextLine() )
		{
			while ( it.nextPixel() )
			{
				it.r() = rand()%255 + 1;
				it.g() = rand()%255 + 1;
				it.b() = rand()%255 + 1;
				it.a() = rand()%255 + 1;
			}
		}

		m_Renderer->updateTexture( m_Texture.texture, img, GL_RGBA32F );

		m_BlockHandle.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().setPrimitiveType( GL_TRIANGLE_STRIP );
		m_BlockHandle.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().setNumberOfVertices( 4 );
		m_BlockHandle.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().addTexture( m_Texture, "texture0" );
		m_BlockHandle.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().addAttribute( m_TexCoords, "texcoord0" );
		m_BlockHandle.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().addAttribute( m_Positions, "position" );

		m_Renderer->finish();

		m_Context->setActive( false );
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void RandomTexture2DBlock::shutdown()
{
	try
	{
		delete m_Context;
		std::cout << "deleted context ( random texture 2D block )" << std::endl;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

ImageToTexture2DBlock::ImageToTexture2DBlock( ContextBlock &context ) :
	Block(), m_Manager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	m_Context( nullptr ), m_Renderer( nullptr )
{
	m_Texture.texture = m_Manager.createTexture( GL_TEXTURE_2D );
	m_TexCoords.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
	m_Positions.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
}

ImageToTexture2DBlock::~ImageToTexture2DBlock() {}

void ImageToTexture2DBlock::setup( BlockHandle &block )
{
	try
	{
		m_BlockHandle = block;

		if ( m_Context == nullptr )
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

			m_Context = new RenderContext( settings );
			m_Renderer = &( m_Context->getRenderer() );

			std::vector< float > coords;
			coords.push_back( 0.0f );
			coords.push_back( 0.0f );
			coords.push_back( 1.0f );
			coords.push_back( 0.0f );
			coords.push_back( 0.0f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );

			m_Renderer->updateBuffer( m_TexCoords.buffer, coords );
			m_TexCoords.normalized = GL_FALSE;
			m_TexCoords.offset = 0;
			m_TexCoords.size = 2;
			m_TexCoords.stride = 0;

			std::vector< float > vertices;
			vertices.push_back( -1.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( 0.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( 0.0f );
			vertices.push_back( -1.0f );
			vertices.push_back( -1.0f );
			vertices.push_back( 0.0f );
			vertices.push_back( 1.0f );
			vertices.push_back( -1.0f );
			vertices.push_back( 0.0f );

			m_Renderer->updateBuffer( m_Positions.buffer, vertices );
			m_Positions.normalized = GL_FALSE;
			m_Positions.offset = 0;
			m_Positions.size = 3;
			m_Positions.stride = 0;

			m_Renderer->finish();
			m_Context->setActive( false );
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

void ImageToTexture2DBlock::update()
{
	try
	{
		m_Context->setActive( true );

		Image const& img = m_BlockHandle.getInletHandle( "ImageData" ).getReadableRef< Image >();
		m_Renderer->updateTexture( m_Texture.texture, img, GL_RGBA32F );
		RenderData &out = m_BlockHandle.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >();
		out.setPrimitiveType( GL_TRIANGLE_STRIP );
		out.setNumberOfVertices( 4 );
		out.addTexture( m_Texture, "texture0" );
		out.addAttribute( m_TexCoords, "texcoord0" );
		out.addAttribute( m_Positions, "position" );

		m_Renderer->finish();
		m_Context->setActive( false );
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void ImageToTexture2DBlock::shutdown()
{
	try
	{
		delete m_Context;
		std::cout << "deleted context ( image to texture 2D )" << std::endl;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}