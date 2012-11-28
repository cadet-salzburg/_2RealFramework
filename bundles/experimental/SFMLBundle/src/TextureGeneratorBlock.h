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