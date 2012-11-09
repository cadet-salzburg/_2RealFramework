#pragma once

#include "OpenGl.h"
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

	void keyPressed( _2Real::gl::RenderWindow::Key &k );

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

};