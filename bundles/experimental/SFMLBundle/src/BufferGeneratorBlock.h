#pragma once

#include "_2RealBlock.h"

#include "gl.h"

class RessourceManagerBlock;

class VectorToBufferBlock : public _2Real::bundle::Block
{

public:

	VectorToBufferBlock( _2Real::bundle::ContextBlock &context );
	~VectorToBufferBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mBufferDataIn;
	_2Real::bundle::OutletHandle		mBufferOut;

	_2Real::gl::BufferObj				*mBufferObj;	// modifieable
	_2Real::gl::Buffer					mBuffer;		// constant

};