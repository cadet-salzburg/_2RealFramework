#include "DisplayWindowBlock.h"
#include "RessourceManagerBlock.h"

#include <iostream>
#include <math.h>


using namespace std;
using namespace _2Real::bundle;
using namespace _2Real::gl;
using namespace _2Real;

DisplayWindowBlock::DisplayWindowBlock( ContextBlock &context ) :
	Block(), mManager( dynamic_cast< RessourceManagerBlock & >( context ) ),
	mWindow( nullptr ), mRenderer( nullptr ), mClock(), mCount( 0 ),
	mTransformMatrix( Mat4::Identity() ), mViewMatrix( Mat4::Identity() ), mProjectionMatrix( Mat4::Identity() ),
	mAngle( 0.0f ), mScale( 1.0f ), mEyePos( 0.0f, 0.0f, -5.0f ), mOrigin( 0.0f, 0.0f, 0.0f )
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
			settings.width = 800;//mWindowWidthIn.getReadableRef< unsigned int >();
			settings.height = 600;//mWindowHeightIn.getReadableRef< unsigned int >();
			settings.glMajor = 3;
			settings.glMinor = 3;
			settings.aaSamples = 16;
			settings.colorBits = 32;
			settings.depthBits = 16;
			settings.stencilBits = 0;

			mWindow = new RenderWindow( settings, mManager.getManager() );
			mRenderer = &( mWindow->getRenderer() );
			mWindow->setActive( false );

			RenderWindow::KeyCallback *keyPressedCb = new _2Real::MemberCallback< DisplayWindowBlock, RenderWindow::KeyEvent & >( *this, &DisplayWindowBlock::keyEvent );
			mWindow->registerToKeyPressed( *keyPressedCb );

			mArcball.setCenter( 400, 300 );
			mArcball.setRadius( 400.f );
			mArcball.setConstraintAxis( 0.f, 1.f, 0.f );
			mArcball.enableConstraintAxis();
			RenderWindow::MouseCallback *mouseEventCb = new _2Real::MemberCallback< DisplayWindowBlock, RenderWindow::MouseEvent & >( *this, &DisplayWindowBlock::mouseEvent );
			mWindow->registerToMouseEvent( *mouseEventCb );

			RenderWindow::ResizeCallback *resizeEventCb = new _2Real::MemberCallback< DisplayWindowBlock, RenderWindow::ResizeEvent & >( *this, &DisplayWindowBlock::resizeEvent );
			mWindow->registerToResizeEvent( *resizeEventCb );

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

void DisplayWindowBlock::resizeEvent( RenderWindow::ResizeEvent &r )
{
	mArcball.setCenter( 0.5f * r.width, 0.5f * r.height );
	mArcball.setRadius( std::max< float >( 0.5f * r.width, 0.5f * r.height ) );
}

void DisplayWindowBlock::mouseEvent( RenderWindow::MouseEvent &e )
{
	switch ( e.action )
	{
	case RenderWindow::LEFT_BUTTON_PRESSED:
		mArcball.mouseDown( e.x, e.y );
		break;
	case RenderWindow::RIGHT_BUTTON_PRESSED:
		break;
	case RenderWindow::MIDDLE_BUTTON_PRESSED:
		break;
	case RenderWindow::LEFT_BUTTON_RELEASED:
		break;
	case RenderWindow::RIGHT_BUTTON_RELEASED:
		break;
	case RenderWindow::MIDDLE_BUTTON_RELEASED:
		break;
	case RenderWindow::MOVED:
		break;
	case RenderWindow::LEFT_DRAG:
		mArcball.mouseDrag( e.x, e.y );
		mArcball.mouseDown( e.x, e.y );
		break;
	case RenderWindow::MIDDLE_DRAG:
		break;
	case RenderWindow::RIGHT_DRAG:
		break;
	case RenderWindow::WHEEL_UP:
		mScale *= 2.0f;
		break;
	case RenderWindow::WHEEL_DOWN:
		mScale /= 2.0f;
		break;
	default:
		break;
	}
}

void DisplayWindowBlock::keyEvent( RenderWindow::KeyEvent &k )
{
	switch( k.key )
	{
	case RenderWindow::X:
		mArcball.setConstraintAxis( 1.f, 0.f, 0.f );
		break;
	case RenderWindow::Y:
		mArcball.setConstraintAxis( 0.f, 1.f, 0.f );
		break;
	case RenderWindow::Z:
		mArcball.setConstraintAxis( 0.f, 0.f, 1.f );
		break;
	case RenderWindow::C:
		if ( k.wasPressed )
			mArcball.enableConstraintAxis( !mArcball.isConstraintAxisEnabled() );
		break;
	default:
		break;
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

		mTransformMatrix << 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, mOrigin[0], mOrigin[1], mOrigin[2], 1.0;

		Eigen::AngleAxis< float > rot( mAngle, Vec3( 0.0, 1.0, 0.0 ) );

		Eigen::Matrix4f rotationMatrix;
		rotationMatrix << 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0;
		Eigen::Matrix3f mat = rot.toRotationMatrix();

		for ( unsigned int i=0; i<3; ++i )
		{
			for ( unsigned int j=0; j<3; ++j )
			{
				unsigned int m3 = i*3+j;
				unsigned int m4 = i*4+j;

				rotationMatrix( m4 ) = mat( m3 );
			}
		}

		mTransformMatrix = mArcball.getRotation();

		Mat4 scale;
		scale << mScale, 0.f, 0.f, 0.f, 0.f, mScale, 0.f, 0.f, 0.f, 0.f, mScale, 0.f, 0.f, 0.f, 0.f, 1.f;
		mTransformMatrix *= scale;
		mViewMatrix = Camera::lookAt( mEyePos, Vec3( 0.0, 0.0, 0.0 ), Vec3( 0.0, 1.0, 0.0 ) );
		mProjectionMatrix = Camera::perspective( 45.f, 1.0f, 1.0f, 20.0f );

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