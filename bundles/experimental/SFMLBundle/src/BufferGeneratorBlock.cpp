#include "BufferGeneratorBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real;
using namespace _2Real::bundle;
using namespace _2Real::gl;

//BufferGenerator::BufferGenerator( ContextBlock &context ) :
//	Block(), m_Manager( dynamic_cast< RessourceManager & >( context ) ),
//	m_Context( nullptr ), m_Renderer( nullptr )
//	{}
//
//BufferGenerator::~BufferGenerator() {}
//
//void BufferGenerator::setup( BlockHandle &block )
//{
//	try
//	{
//		m_Block = block;
//
//		if ( m_Context == nullptr )
//		{
//			// TODO: add inlets for window settings
//			RenderSettings settings;
//			settings.title = "";
//			settings.glMajor = 3;
//			settings.glMinor = 3;
//			settings.aaSamples = 16;
//			settings.colorBits = 32;
//			settings.depthBits = 16;
//			settings.stencilBits = 0;
//			settings.width = 640;
//			settings.height = 480;
//
//			m_Context = new RenderContext( settings );
//			m_Renderer = &( m_Context->getRenderer() );
//
//			m_Context->setActive( false );
//
//			m_Buffer = m_Manager.createBuffer( GL_DYNAMIC_DRAW );
//		}
//		// TODO: recreate context if settings change
//		// else {}
//	}
//	catch( Exception & e )
//	{
//		cout << e.message() << " " << e.what() << endl;
//		e.rethrow();
//	}
//	catch( std::exception & e )
//	{
//		cout << e.what() << endl;
//		Exception exc( e.what() );
//		throw exc;
//	}
//}
//
//void BufferGenerator::update()
//{
//	try
//	{
//		m_Context->setActive( true );
//
//		std::vector< float > vertices;
//		vertices.push_back( -0.5f );
//		vertices.push_back( 0.5f );
//		vertices.push_back( 0.0f );
//		vertices.push_back( 0.5f );
//		vertices.push_back( 0.5f );
//		vertices.push_back( 0.0f );
//		vertices.push_back( -0.5f );
//		vertices.push_back( -0.5f );
//		vertices.push_back( 0.0f );
//		vertices.push_back( 0.5f );
//		vertices.push_back( -0.5f );
//		vertices.push_back( 0.0f );
//
//		m_Renderer->updateBuffer( m_Buffer, vertices );
//		//m_Renderer->finish();		// todo: better sync
//
//		RenderData::VertexAttribute attrib;
//		attrib.buffer = m_Buffer;
//		attrib.size = 3;
//		attrib.normalized = GL_FALSE;
//		attrib.offset = 0;
//		attrib.stride = 0;
//
//		m_Block.getOutletHandle( "render_data" ).getWriteableRef< RenderData >().setPrimitiveType( GL_TRIANGLE_STRIP );
//		m_Block.getOutletHandle( "render_data" ).getWriteableRef< RenderData >().setNumberOfVertices( 4 );
//		m_Block.getOutletHandle( "render_data" ).getWriteableRef< RenderData >().addVertexAttribute( attrib, "position" );
//
//		m_Context->setActive( false );
//	}
//	catch( Exception & e )
//	{
//		cout << e.message() << " " << e.what() << endl;
//		e.rethrow();
//	}
//	catch( std::exception & e )
//	{
//		cout << e.what() << endl;
//	}
//}
//
//void BufferGenerator::shutdown()
//{
//	try
//	{
//		delete m_Context;
//		std::cout << "deleted context ( buffer generator )" << std::endl;
//	}
//	catch( Exception & e )
//	{
//		cout << e.message() << " " << e.what() << endl;
//		e.rethrow();
//	}
//	catch( std::exception & e )
//	{
//		cout << e.what() << endl;
//	}
//}