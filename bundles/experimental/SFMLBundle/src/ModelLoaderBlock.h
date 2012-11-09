#pragma once

#include "OpenGl.h"
#include "_2RealBlock.h"

#include <ASSIMP\Importer.hpp>
#include <ASSIMP\postprocess.h>
#include <ASSIMP\scene.h>

class RessourceManagerBlock;

class ModelLoaderBlock : public _2Real::bundle::Block
{

public:

	ModelLoaderBlock( _2Real::bundle::ContextBlock &context );
	~ModelLoaderBlock();

	void shutdown();
	void update();
	void setup( _2Real::bundle::BlockHandle &context );

private:

	RessourceManagerBlock				&mManager;
	_2Real::gl::Context					*mContext;

	Assimp::Importer					mModelLoader;

	_2Real::bundle::BlockHandle			mBlockHandle;
	_2Real::bundle::InletHandle			mFilePathIn;
	_2Real::bundle::OutletHandle		mIndicesOut;
	_2Real::bundle::OutletHandle		mVertexPositionsOut;
	_2Real::bundle::OutletHandle		mVertexNormalsOut;
	_2Real::bundle::OutletHandle		mNumberOfMeshesOut;

};