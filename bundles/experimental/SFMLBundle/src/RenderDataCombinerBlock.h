#pragma once

#include "OpenGl.h"
#include "_2RealBlock.h"

class RessourceManagerBlock;

class RenderDataCombinerBlock : public _2Real::bundle::Block
{

public:

	RenderDataCombinerBlock( _2Real::bundle::ContextBlock &context );
	~RenderDataCombinerBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::OutletHandle		mRenderDataOut;
	_2Real::bundle::InletHandle			mBuffersMultiin;
	_2Real::bundle::InletHandle			mTexturesMultiin;
	_2Real::bundle::InletHandle			mVertexShaderIn;
	_2Real::bundle::InletHandle			mFragmentShaderIn;
	_2Real::bundle::InletHandle			mGeometryShaderIn;
	_2Real::bundle::InletHandle			mUniformsMultiin;
	_2Real::bundle::InletHandle			mAttributesMultiin;
	_2Real::bundle::InletHandle			mPrimitiveTypeIn;

	_2Real::gl::ProgramObj				*mProgramObj;	// modifieable
	_2Real::gl::Program					mProgram;		// constant

};