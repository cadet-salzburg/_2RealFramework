#include "OffscreenRenderBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real::bundle;
using namespace _2Real::gl;
using namespace _2Real;

OffscreenRenderBlock::OffscreenRenderBlock( ContextBlock &context ) :
	Block(), m_Manager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	m_Context( nullptr ), m_Renderer( nullptr )
{
	m_Texture.texture = m_Manager.createTexture( GL_TEXTURE_2D );
	m_TexCoords.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
	m_Positions.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
}

OffscreenRenderBlock::~OffscreenRenderBlock() {}

void OffscreenRenderBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		if ( m_Context == nullptr )
		{
			// TODO: add inlets for window settings?
			RenderSettings settings;
			settings.title = "offscreen renderer";
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;
			settings.width = 400;
			settings.height = 400;

			m_Context = new RenderContext( settings );
			m_Renderer = &( m_Context->getRenderer() );

			m_Renderer->resizeTexture( m_Texture.texture, GL_RGBA32F, 400, 400 );
			m_Renderer->setViewport( 400, 400 );

			std::vector< float > coords;
			coords.push_back( 0.0f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );
			coords.push_back( 0.0f );
			coords.push_back( 0.0f );
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

			m_Context->setActive( false );
		}
		// TODO: recreate window if settings change
		//else {}
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

void OffscreenRenderBlock::update()
{
	try
	{
		m_Context->setActive( true );

		m_Renderer->attachTexture( m_Texture.texture );
		m_Renderer->checkStatus();

		Vec3 clear;
		clear << 0.5f, 0.0f, 0.5f;
		m_Renderer->clear( clear );

		unsigned int dataCount = m_Block.getInletHandle( "RenderData" ).getSize();
		for ( unsigned int i=0; i<dataCount; ++i )
		{
			RenderData const& data = m_Block.getInletHandle( "RenderData" )[ i ].getReadableRef< RenderData >();
			m_Renderer->render( data );
		}

		m_Renderer->finish();
		m_Renderer->detachTexture( m_Texture.texture );

		m_Block.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().setPrimitiveType( GL_TRIANGLE_STRIP );
		m_Block.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().setNumberOfVertices( 4 );
		m_Block.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().addTexture( m_Texture, "texture0" );
		m_Block.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().addAttribute( m_TexCoords, "texcoord0" );
		m_Block.getOutletHandle( "RenderData" ).getWriteableRef< RenderData >().addAttribute( m_Positions, "position" );

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

void OffscreenRenderBlock::shutdown()
{
	try
	{
		delete m_Context;
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

ImageSubtractionBlock::ImageSubtractionBlock( ContextBlock &context ) : OffscreenRenderBlock( context )
{
	m_PointPositions.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
	m_PointTexcoords.buffer = m_Manager.createBuffer( GL_STATIC_DRAW );
}

ImageSubtractionBlock::~ImageSubtractionBlock() {}

void ImageSubtractionBlock::setup( BlockHandle &block )
{
	try
	{
		m_Block = block;

		if ( m_Context == nullptr )
		{
			// TODO: add inlets for window settings?
			RenderSettings settings;
			settings.title = "offscreen renderer";
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;
			settings.width = 400;
			settings.height = 400;

			m_Context = new RenderContext( settings );
			m_Renderer = &( m_Context->getRenderer() );

			m_Renderer->resizeTexture( m_Texture.texture, GL_RGBA32F, 400, 400 );
			m_Renderer->setViewport( 400, 400 );

			std::vector< float > pointPos;
			std::vector< float > pointTex;
			const float stepX = 2.0f/settings.width;
			const float halfX = settings.width*stepX*0.5f;
			const float stepY = 2.0f/settings.height;
			const float halfY = settings.height*stepY*0.5f;
			const float stepU = 1.0f/settings.width;
			const float stepV = 1.0f/settings.height;
			for ( unsigned int i= 0;i<400; ++i )
			{
				float x = -halfX + i*stepX;
				float u = i*stepU;
				for ( unsigned int j=0; j<400; ++j )
				{
					float y = -halfY + j*stepY;
					float v = 1.0f - j*stepV;
					pointPos.push_back( x );
					pointPos.push_back( y );
					pointTex.push_back( u );
					pointTex.push_back( v );
				}
			}

			m_Renderer->updateBuffer( m_PointTexcoords.buffer, pointTex );
			m_PointTexcoords.normalized = GL_FALSE;
			m_PointTexcoords.offset = 0;
			m_PointTexcoords.size = 2;
			m_PointTexcoords.stride = 0;

			m_Renderer->updateBuffer( m_PointPositions.buffer, pointPos );
			m_PointPositions.normalized = GL_FALSE;
			m_PointPositions.offset = 0;
			m_PointPositions.size = 2;
			m_PointPositions.stride = 0;

			std::vector< float > coords;
			coords.push_back( 0.0f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );
			coords.push_back( 1.0f );
			coords.push_back( 0.0f );
			coords.push_back( 0.0f );
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

			//std::string vert = "#version 330 core\nin vec3 position;\nin vec2 texcoord0;\nin vec2 texcoord1;\nout vec2 fTexcoord0;\nout vec2 fTexcoord1;\nvoid main()\n{\nfTexcoord0 = texcoord0;\nfTexcoord1=texcoord1;\ngl_Position = vec4( position.xyz, 1.0 );\n}\n";
			//std::string frag = "#version 330 core\nuniform sampler2D texture0;\nuniform sampler2D texture1;\nin vec2 fTexcoord0;\nin vec2 fTexcoord1;\nout vec4 fragColor;\nvoid main()\n{\nvec4 col0 = texture( texture0, fTexcoord0 );\nfloat lum = col0.x+col0.y+col0.z;\nif ( lum < 0.5 ) discard;\nfragColor = vec4( vec3( 1.0 ) - texture( texture1, fTexcoord1 ).xyz, 1.0 );\n}\n";

			//std::string vert = "#version 330 core\nin vec3 position;\nin vec2 texcoord0;\nin vec2 texcoord1;\nout vec2 gTexcoord0;\nout vec2 gTexcoord1;\nvoid main(){\ngTexcoord0=texcoord0;\ngTexcoord1=texcoord1;\ngl_Position = vec4( position.xy, 0.0, 1.0 );\n}\n";
			//std::string geo = "#version 330 core\nlayout(points) in;\nlayout(points) out;\nuniform sampler2D texture0;\nuniform sampler2D texture1;\nin vec2 gTexcoord0[];\nin vec2 gTexcoord1[];\nout vec4 fColor;\nvoid main(){\nvec4 userColor=texture( texture0, gTexcoord0[ 0 ] );\nfloat lum=userColor.x+userColor.y+userColor.z;\nif( lum > 0.5 ){\nfloat col=1.0-texture( texture1, gTexcoord1[ 0 ] ).r;\nfColor = vec4( col, col, col, 1.0 );\ngl_Position=gl_in[0].gl_Position;\nEmitVertex();\n}\n}\n";
			//std::string frag = "#version 330 core\nin vec4 fColor;\nout vec4 fragColor;\nvoid main(){\nfragColor=fColor;\n}\n";

			std::string vert = "#version 330 core\nin vec3 position;\nin vec2 texcoord0;\nin vec2 texcoord1;\nout vec2 gTexcoord0;\nout vec2 gTexcoord1;\nvoid main(){\ngTexcoord0=texcoord0;\ngTexcoord1=texcoord1;\ngl_Position = vec4( position.xy, 0.0, 1.0 );\n}\n";
			std::string geo = "#version 330 core\nlayout(points) in;\nlayout(points) out;\nuniform sampler2D texture0;\nuniform sampler2D texture1;\nin vec2 gTexcoord0[];\nin vec2 gTexcoord1[];\nout vec4 fColor;\nvoid main(){\nvec4 userColor=texture( texture0, gTexcoord0[ 0 ] );\nfloat lum=userColor.x+userColor.y+userColor.z;\nif( lum > 0.5 ){\nfloat col=1.0-texture( texture1, gTexcoord1[ 0 ] ).r;\nfColor = vec4( col, col, col, 1.0 );\ngl_Position=gl_in[0].gl_Position;\nEmitVertex();\n}\n}\n";
			std::string frag = "#version 330 core\nin vec4 fColor;\nout vec4 fragColor;\nvoid main(){\nfragColor=fColor;\n}\n";


			m_Renderer->setProgram( "subtract", vert, frag, geo );

			m_Context->setActive( false );
		}
		// TODO: recreate window if settings change
		//else {}
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

void ImageSubtractionBlock::update()
{
	try
	{
		m_Context->setActive( true );

		m_Renderer->attachTexture( m_Texture.texture );
		m_Renderer->checkStatus();

		Vec4 clear;
		clear << 0.0f, 0.0f, 0.0f, 0.0f;
		m_Renderer->clear( clear );

		RenderData const& min = m_Block.getInletHandle( "Minuend" ).getReadableRef< RenderData >();
		RenderData const& sub = m_Block.getInletHandle( "Subtrahend" ).getReadableRef< RenderData >();

		RenderData data;
		data.addTextureAs( min, "texture0", "texture0" );
		data.addTextureAs( sub, "texture0", "texture1" );
		data.addAttribute( m_PointPositions, "position" );
		data.addAttribute( m_PointTexcoords, "texcoord0" );
		data.addAttribute( m_PointTexcoords, "texcoord1" );
		data.setPrimitiveType( GL_POINTS );
		data.setNumberOfVertices( 400*400 );

		m_Renderer->render( data, "subtract" );

		m_Renderer->finish();
		m_Renderer->detachTexture( m_Texture.texture );

		RenderData &out = m_Block.getOutletHandle( "Difference" ).getWriteableRef< RenderData >();

		out.setPrimitiveType( GL_TRIANGLE_STRIP );
		out.setNumberOfVertices( 4 );
		out.addTexture( m_Texture, "texture0" );
		out.addAttribute( m_TexCoords, "texcoord0" );
		out.addAttribute( m_Positions, "position" );

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

void ImageSubtractionBlock::shutdown()
{
	try
	{
		delete m_Context;
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