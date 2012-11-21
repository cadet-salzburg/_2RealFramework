#pragma once

#include "OpenGl.h"
#include "Arcball.h"
#include "_2RealBlock.h"

class RessourceManagerBlock;

class DisplayWindowBlock : public _2Real::bundle::Block
{

public:

	DisplayWindowBlock( _2Real::bundle::ContextBlock &context );
	~DisplayWindowBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

	void keyEvent( _2Real::gl::RenderWindow::KeyEvent &k );
	void mouseEvent( _2Real::gl::RenderWindow::MouseEvent &ev );
	void resizeEvent( _2Real::gl::RenderWindow::ResizeEvent &ev );

private:

	sf::Clock							mClock;
	double								mCount;
	RessourceManagerBlock				&mManager;
	_2Real::gl::RenderWindow			*mWindow;
	_2Real::gl::Renderer				*mRenderer;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mRenderDataMultiin;
	_2Real::bundle::InletHandle			mClearColorIn;
	_2Real::bundle::InletHandle			mWindowTitleIn;
	_2Real::bundle::InletHandle			mWindowWidthIn;
	_2Real::bundle::InletHandle			mWindowHeightIn;
	_2Real::bundle::InletHandle			mIsKeyboardEnabledIn;
	_2Real::bundle::InletHandle			mIsMouseEnabled;
	_2Real::bundle::OutletHandle		mTransformMatOut;
	_2Real::bundle::OutletHandle		mViewMatOut;
	_2Real::bundle::OutletHandle		mProjectionMatOut;

	_2Real::Mat4						mTransformMatrix;
	_2Real::Mat4						mViewMatrix;
	_2Real::Mat4						mProjectionMatrix;
	float								mAngle;
	float								mScale;
	_2Real::Vec3						mEyePos;
	_2Real::Vec3						mOrigin;

	_2Real::gl::Arcball					mArcball;

};