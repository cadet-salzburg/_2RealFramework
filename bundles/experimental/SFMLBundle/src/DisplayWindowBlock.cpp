#include "DisplayWindowBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>

using namespace std;
using namespace _2Real::bundle;
using namespace _2Real::gl;
using namespace _2Real;

DisplayWindowBlock::DisplayWindowBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	mWindow( nullptr ), mRenderer( nullptr ), mClock(), mCount( 0 ),
	mTransformMatrix( Mat4::Identity() ), mViewMatrix( Mat4::Identity() ), mProjectionMatrix( Mat4::Identity() ), mAngle( 0.0f )
	{}

DisplayWindowBlock::~DisplayWindowBlock() {}

void DisplayWindowBlock::setup( BlockHandle &block )
{
	try
	{
		mBlockHandle = block;
		mWindowTitleIn = mBlockHandle.getInletHandle( "WindowTitle" );
		mWindowWidthIn = mBlockHandle.getInletHandle( "WindowWidth" );
		mWindowHeightIn = mBlockHandle.getInletHandle( "WindowHeight" );
		mClearColorIn = mBlockHandle.getInletHandle( "ClearColor" );
		mRenderDataMultiin = mBlockHandle.getInletHandle( "RenderData" );
		mIsMouseEnabled = mBlockHandle.getInletHandle( "EnableMouseInteraction" );
		mIsKeyboardEnabledIn = mBlockHandle.getInletHandle( "EnableKeyboardInteraction" );
		mTransformMatOut = mBlockHandle.getOutletHandle( "TransformationMatrix" );
		mViewMatOut = mBlockHandle.getOutletHandle( "ViewMatrix" );
		mProjectionMatOut = mBlockHandle.getOutletHandle( "ProjectionMatrix" );

		if ( mWindow == nullptr )
		{
			RenderSettings settings;// = mManager.getRenderSettings();
			settings.title = "narf";//mWindowTitleIn.getReadableRef< string >();
			settings.width = 640;//mWindowWidthIn.getReadableRef< unsigned int >();
			settings.height = 480;//mWindowHeightIn.getReadableRef< unsigned int >();
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;

			mWindow = new RenderWindow( settings, mManager.getManager() );
			mRenderer = &( mWindow->getRenderer() );
			mWindow->setActive( false );

			RenderWindow::KeyCallback *keyPressedCb = new _2Real::MemberCallback< DisplayWindowBlock, RenderWindow::Key & >( *this, &DisplayWindowBlock::keyPressed );
			mWindow->registerToKeyPressed( *keyPressedCb );

			mClock.restart();
			mCount = 0;
		}
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

void DisplayWindowBlock::keyPressed( RenderWindow::Key &k )
{
	if ( k == RenderWindow::R )
	{
		mAngle += M_PI/180.0f;
	}
	else if ( k == RenderWindow::Q )
	{
		mAngle -= M_PI/180.0f;
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

		mTransformMatrix << 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;

		Vec3 eye = Vec3( 0.0, 0.0, -2.0 );
		Eigen::AngleAxis< float > rot( mAngle, Vec3( 0.0, 1.0, 0.0 ) );
		Vec3 e = rot.toRotationMatrix()*eye;
		mViewMatrix = Camera::lookAt( e, Vec3( 0.0, 0.0, 0.0 ), Vec3( 0.0, 1.0, 0.0 ) );
		mProjectionMatrix = Camera::perspective( 45.f, 1.0f, 1.0f, 10.0f );

		stringstream modelStr;
		modelStr << "matModel ( ";
		for ( unsigned int i=0; i<16; ++i ) modelStr << mTransformMatrix( i ) << " ";
		modelStr << ")";

		stringstream viewStr;
		viewStr << "matView ( ";
		for ( unsigned int i=0; i<16; ++i ) viewStr << mViewMatrix( i ) << " ";
		viewStr << ")";

		stringstream projStr;
		projStr << "matProj ( ";
		for ( unsigned int i=0; i<16; ++i ) projStr << mProjectionMatrix( i ) << " ";
		projStr << ")";

		mTransformMatOut.getWriteableRef< string >() = modelStr.str();
		mViewMatOut.getWriteableRef< string >() = viewStr.str();
		mProjectionMatOut.getWriteableRef< string >() = projStr.str();
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