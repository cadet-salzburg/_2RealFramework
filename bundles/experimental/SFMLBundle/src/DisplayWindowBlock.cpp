#include "DisplayWindowBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>
#include <Windows.h>

using namespace std;
using namespace _2Real::bundle;
using namespace _2Real::gl;
using namespace _2Real;

DisplayWindowBlock::DisplayWindowBlock( ContextBlock &context ) :
	Block(), m_Manager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	m_Window( nullptr ), m_Renderer( nullptr ), m_Clock(), m_Count( 0 )
	{}

DisplayWindowBlock::~DisplayWindowBlock() {}

void DisplayWindowBlock::setup( BlockHandle &block )
{
	try
	{
		m_BlockHandle = block;

		if ( m_Window == nullptr )
		{
			// TODO: add inlets for window settings
			RenderSettings settings;
			settings.title = m_BlockHandle.getInletHandle( "WindowTitle" ).getReadableRef< std::string >();
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;
			settings.width = 640;
			settings.height = 480;

			m_Window = new RenderWindow( settings );
			m_Renderer = &( m_Window->getRenderer() );
			m_Window->setActive( false );

			m_Clock.restart();
			m_Count = 0;
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

void DisplayWindowBlock::update()
{
	try
	{
		m_Window->setActive( true );
		m_Window->processEvents();

		++m_Count;
		if ( m_Clock.getElapsedTime().asMicroseconds() > 1000000 )
		{
			double microsPerUpdate = m_Clock.getElapsedTime().asMicroseconds() / m_Count;
			double updatesPerSecond = 1000000 / microsPerUpdate;

			std::ostringstream title;
			title << m_BlockHandle.getInletHandle( "WindowTitle" ).getReadableRef< std::string >();
			title << " " << updatesPerSecond << " fps";
			m_Window->setTitle( title.str() );

			m_Clock.restart();
			m_Count = 0;
		}

		Vec4 const& bgColor = m_BlockHandle.getInletHandle( "ClearColor" ).getReadableRef< Vec4 >();
		m_Renderer->clear( bgColor );

		unsigned int dataCount = m_BlockHandle.getInletHandle( "RenderData" ).getSize();
		for ( unsigned int i=0; i<dataCount; ++i )
		{
			RenderData const& data = m_BlockHandle.getInletHandle( "RenderData" )[ i ].getReadableRef< RenderData >();
			m_Renderer->render( data );
		}

		m_Renderer->finish();

		m_Window->display();
		m_Window->setActive( false );
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

void DisplayWindowBlock::shutdown()
{
	try
	{
		delete m_Window;
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