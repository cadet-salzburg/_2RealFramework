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
#include "CameraDeviceManager.h"

using namespace _2Real::bundle;

class CameraCaptureBlock : public Block
{
public:
	CameraCaptureBlock( ContextBlock & context );
	~CameraCaptureBlock();
	void					shutdown();
	void					update();
	void					setup( BlockHandle &context );

private:
	void					discardOutlets();

	ParameterHandle						mDeviceIn;
	ParameterHandle						mWidthIn;
	ParameterHandle						mHeightIn;
	ParameterHandle						mFpsIn;

	OutletHandle						mImageOut;
	OutletHandle						mWidthOut;
	OutletHandle						mHeightOut;

	CameraDeviceManager					&mDeviceManager;
	int									mDevice;
	int									mWidth;
	int									mHeight;
	int									mFps;
};