#pragma once

#include "_2RealBlock.h"

#include "gl.h"

class RessourceManagerBlock;

class FullscreenMeshBlock : public _2Real::bundle::Block
{

public:

	FullscreenMeshBlock( _2Real::bundle::ContextBlock &context );
	~FullscreenMeshBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	void calcAttribs( const unsigned int w, const unsigned int h, const int p );

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mMeshWidthIn;
	_2Real::bundle::InletHandle			mMeshHeightIn;
	_2Real::bundle::InletHandle			mPrimitiveTypeIn;
	_2Real::bundle::OutletHandle		mVerticesOut;
	_2Real::bundle::OutletHandle		mTexcoordsOut;
	_2Real::bundle::OutletHandle		mIndicesOut;

	_2Real::gl::BufferObj				*mVertexBufferObj;
	_2Real::gl::Buffer					mVertexBuffer;
	_2Real::gl::BufferObj				*mTexcoordBufferObj;
	_2Real::gl::Buffer					mTexcoordBuffer;
	_2Real::gl::BufferObj				*mIndexBufferObj;
	_2Real::gl::Buffer					mIndexBuffer;

};