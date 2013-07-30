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
#include "videoInput.h"
#include "Poco/Mutex.h"
#include <vector>

using namespace _2Real::bundle;

class CameraDeviceManager : public ContextBlock
{
public:
	CameraDeviceManager();
	~CameraDeviceManager();
	void							setup( _2Real::bundle::BlockHandle &context );
	void							update();
	void							shutdown();

	void							unbindDevice( const unsigned int device );
	bool							bindDevice( const unsigned int device, const unsigned int width, const unsigned int height, const unsigned int fps = 30 );
	bool							setCameraParams( const unsigned int device, const unsigned int width, const unsigned int height, const unsigned int fps = 30 );
	unsigned int					getNumberOfConnectedDevices() const;
	bool							isDeviceRunning( const unsigned int device ) const;
	bool							isNewFrameAvailable( const unsigned int device ) const;
	int								getFirstFreeDevice() const; // returns -1 for no free devices at all, otherwise int is the index for the free device
	unsigned char *					getPixels( const unsigned int device ) const;
	unsigned int					getVideoWidth( const unsigned int device ) const;
	unsigned int					getVideoHeight( const unsigned int device ) const;

private:

	void							initDeviceList();
	void							rescanDeviceList();
	bool							isDeviceAvailable( const unsigned int device ) const;
	bool							isDeviceFree( const unsigned int device ) const;

	struct Device
	{
		Device::Device( std::string const& d, const bool u ) : mDescription( d ), mIsUsed( u ) {}
		std::string					mDescription;
		bool						mIsUsed;
	};

	std::vector< Device >						mDevices; 
	unsigned int								mNumDevices;
	videoInput*									mVideoInputController;
	mutable Poco::FastMutex						mMutex;
};