#include "DisplayWindowBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real::bundle;
using namespace _2Real::gl;
using namespace _2Real;

DisplayWindowBlock::DisplayWindowBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	mWindow( nullptr ), mRenderer( nullptr ), mClock(), mCount( 0 )
	{}

DisplayWindowBlock::~DisplayWindowBlock() {}

void DisplayWindowBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mWindowTitleIn = mBlockHandle.getInletHandle( "WindowTitle" );
		mClearColorIn = mBlockHandle.getInletHandle( "ClearColor" );
		mRenderDataMultiin = mBlockHandle.getInletHandle( "RenderData" );

		if ( mWindow == nullptr )
		{
			// TODO: add inlets for window settings
			RenderSettings settings;
			settings.title = mBlockHandle.getInletHandle( "WindowTitle" ).getReadableRef< std::string >();
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;
			settings.width = 640;
			settings.height = 480;

			mWindow = new RenderWindow( settings, mManager.getManager() );
			mRenderer = &( mWindow->getRenderer() );
			mWindow->setActive( false );

			mClock.restart();
			mCount = 0;
		}
		// TODO: recreate window if settings change ( ? )
		//else {}
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( exception & e )
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
		mWindow->setActive( true );
		mWindow->processEvents();

		++mCount;
		if ( mClock.getElapsedTime().asMicroseconds() > 1000000 )
		{
			double microsPerUpdate = mClock.getElapsedTime().asMicroseconds() / mCount;
			double updatesPerSecond = 1000000 / microsPerUpdate;

			std::ostringstream title;
			title << mWindowTitleIn.getReadableRef< string >();
			title << " " << updatesPerSecond << " fps";
			mWindow->setTitle( title.str() );

			mClock.restart();
			mCount = 0;
		}

		Vec4 const& bgColor = mClearColorIn.getReadableRef< Vec4 >();
		mRenderer->clear( bgColor );

		unsigned int renderDataCount = mRenderDataMultiin.getSize();
		for ( unsigned int i=0; i<renderDataCount; ++i )
		{
			RenderData const& data = mRenderDataMultiin[ i ].getReadableRef< RenderData >();
			mRenderer->render( data );
		}

		mWindow->display();
		mWindow->setActive( false );
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void DisplayWindowBlock::shutdown()
{
	try
	{
		delete mWindow;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}