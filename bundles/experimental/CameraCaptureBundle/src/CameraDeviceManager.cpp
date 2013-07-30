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
#include "CameraDeviceManager.h"
#include "Objbase.h"	// needed for couninitialize
#include <iostream>
#include <string>

using _2Real::bundle::Block;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;


CameraDeviceManager::CameraDeviceManager() :
	ContextBlock(),
	mVideoInputController( nullptr )
{
	// this is super weird, videoinputlib in multithreaded environment set coinitializeex for multithread mode and complain that another library already did
	// neither _2Real nor Poco use com interfaces so not sure why this next line is needed but otherwise the whole thing won't work, sometimes I had coding ;-(
	//CoUninitialize();
}

CameraDeviceManager::~CameraDeviceManager()
{
}

void CameraDeviceManager::setup( BlockHandle &context )
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mMutex );

		if ( !mVideoInputController )
			mVideoInputController = new videoInput();

		mNumDevices = mVideoInputController->listDevices( false );
		mDevices.clear();
		for( unsigned int i=0; i<mNumDevices; ++i )
			mDevices.push_back( Device( mVideoInputController->getDeviceName( i ), false ) );
	}
	catch ( ... )
	{
		throw _2Real::Exception( "setup error" );
	}
}

void CameraDeviceManager::update()
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mMutex );

		int numDevices = mVideoInputController->listDevices( false );
		if ( numDevices != mNumDevices )
		{
			for( unsigned int i = 0; i<mDevices.size(); ++i )
			{
				if( mDevices[ i ].mIsUsed )
					mVideoInputController->stopDevice( i );
			}
			mDevices.clear();
			for( unsigned int i=0; i<mNumDevices; ++i )
				mDevices.push_back( Device( mVideoInputController->getDeviceName( i ), false ) );
			mNumDevices = numDevices;
		}
	}
	catch ( ... )
	{
		throw _2Real::Exception( "update error" );
	}
}

void CameraDeviceManager::shutdown()
{
	try
	{
		Poco::ScopedLock< Poco::FastMutex > lock( mMutex );

		for( unsigned int i = 0; i<mDevices.size(); ++i )
		{
			if( mDevices[ i ].mIsUsed )
				mVideoInputController->stopDevice( i );
		}

		delete mVideoInputController;
	}
	catch ( ... )
	{
		throw _2Real::Exception( "shutdown error" );
	}
}

int CameraDeviceManager::getFirstFreeDevice() const
{
	Poco::FastMutex::ScopedLock lock( mMutex );

	for( unsigned int i=0; i<mDevices.size(); ++i )
	{
		if( !mDevices[ i ].mIsUsed )
			return i;
	}
	return -1;
}

bool CameraDeviceManager::isDeviceRunning( const unsigned int device ) const
{
	return mVideoInputController->isDeviceSetup( device );
}

bool CameraDeviceManager::isNewFrameAvailable( const unsigned int device ) const
{
	return mVideoInputController->isFrameNew( device );
}

bool CameraDeviceManager::isDeviceAvailable( const unsigned int device ) const
{
	Poco::FastMutex::ScopedLock lock( mMutex );

	return ( device < mDevices.size() );
}

bool CameraDeviceManager::isDeviceFree( const unsigned int device ) const
{
	Poco::FastMutex::ScopedLock lock( mMutex );

	if ( isDeviceAvailable( device ) )
		return !mDevices[ device ].mIsUsed;
	else
		return false;
}

bool CameraDeviceManager::bindDevice( const unsigned int device, const unsigned int width, const unsigned int height, const unsigned int fps )
{
	Poco::FastMutex::ScopedLock lock( mMutex );

	if ( isDeviceFree( device ) )
	{
		mVideoInputController->setIdealFramerate( device, fps );
		return mDevices[ device ].mIsUsed = mVideoInputController->setupDevice( device, width, height );
	}
	else
		return false;
}

void CameraDeviceManager::unbindDevice( const unsigned int device )
{
	Poco::FastMutex::ScopedLock lock( mMutex );

	if( !isDeviceFree( device ) )
	{
		mVideoInputController->stopDevice( device );
		mDevices[ device ].mIsUsed = false;
	}
}

bool CameraDeviceManager::setCameraParams( const unsigned int device, const unsigned int width, const unsigned int height, const unsigned int fps )
{
	Poco::FastMutex::ScopedLock lock( mMutex );

	if ( !isDeviceFree( device ) )
	{
		mVideoInputController->stopDevice( device );
		mVideoInputController->setIdealFramerate( device, fps );
		return mDevices[ device ].mIsUsed = mVideoInputController->setupDevice( device, width, height );
	}
	return false;
}

unsigned int CameraDeviceManager::getNumberOfConnectedDevices() const
{
	Poco::FastMutex::ScopedLock lock( mMutex );
	return mNumDevices;
}

unsigned char * CameraDeviceManager::getPixels( const unsigned int device ) const
{
	// this seems to be locked anyway by videoinput lib, and every device in our case is just grabbed by a single block
	try
	{
		return mVideoInputController->getPixels( device, true, true );
	}
	catch ( ... )
	{
		std::cout << "couldn't get the pixels" << std::endl;
	}
}

unsigned int CameraDeviceManager::getVideoWidth( const unsigned int device ) const
{
	return mVideoInputController->getWidth( device );
}

unsigned int CameraDeviceManager::getVideoHeight( const unsigned int device ) const
{
	return mVideoInputController->getHeight( device );
}
