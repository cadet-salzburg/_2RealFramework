/*
	CADET - Center for Advances in Digital Entertainment Technologies
	Copyright 2011 Fachhochschule Salzburg GmbH
		http://www.cadet.at

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
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