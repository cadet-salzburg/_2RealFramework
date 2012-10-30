#pragma once

#include "_2RealBlock.h"

#include "gl.h"

class RessourceManagerBlock;

class RandomTexture2DBlock : public _2Real::bundle::Block
{

public:

	RandomTexture2DBlock( _2Real::bundle::ContextBlock &context );
	~RandomTexture2DBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mTextureWidthIn;
	_2Real::bundle::InletHandle			mTextureHeightIn;
	_2Real::bundle::OutletHandle		mTextureOut;

	_2Real::gl::TextureObj				*mTextureObj;	// modifieable
	_2Real::gl::Texture					mTexture;		// constant

};

class ImageToTexture2DBlock : public _2Real::bundle::Block
{

public:

	ImageToTexture2DBlock( _2Real::bundle::ContextBlock &context );
	~ImageToTexture2DBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mTextureDataIn;
	_2Real::bundle::OutletHandle		mTextureOut;

	_2Real::gl::TextureObj				*mTextureObj;	// modifieable
	_2Real::gl::Texture					mTexture;		// constant

};