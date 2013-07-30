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
#include <iostream>
#include <string>
#include <sstream>
#include "CameraCaptureBlock.h"
#include "_2RealDatatypes.h"

using namespace _2Real;
using _2Real::bundle::BlockHandle;
using _2Real::Exception;
using std::cout;
using std::endl;
using std::string;

CameraCaptureBlock::CameraCaptureBlock( ContextBlock &context ) :
	Block(),
	mDeviceManager( static_cast< CameraDeviceManager & >( context ) ),
	mDevice( -1 ),
	mWidth( 0 ),
	mHeight( 0 ),
	mFps( 0 )
{
}

CameraCaptureBlock::~CameraCaptureBlock()
{
}

void CameraCaptureBlock::setup( BlockHandle &block )
{
	try
	{
		mDeviceIn	= block.getParameterHandle( "device" );
		mWidthIn	= block.getParameterHandle( "width" );
		mHeightIn	= block.getParameterHandle( "height" );
		mFpsIn		= block.getParameterHandle( "fps" );
		mImageOut	= block.getOutletHandle( "image" );
		mWidthOut	= block.getOutletHandle( "width" );
		mHeightOut	= block.getOutletHandle( "height" );
		mDevice = -1;
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void CameraCaptureBlock::update()
{
	try
	{
		if ( mDeviceManager.getNumberOfConnectedDevices() > 0 )
		{
			std::shared_ptr< const CustomType > dData = mDeviceIn.getReadableRef();
			unsigned int device = *( dData->get< unsigned int >( "default" ).get() );

			std::shared_ptr< const CustomType > wData = mWidthIn.getReadableRef();
			unsigned int width = *( wData->get< unsigned int >( "default" ).get() );

			std::shared_ptr< const CustomType > hData = mHeightIn.getReadableRef();
			unsigned int height = *( hData->get< unsigned int >( "default" ).get() );

			std::shared_ptr< const CustomType > fpsData = mFpsIn.getReadableRef();
			unsigned int fps  = *( fpsData->get< unsigned int >( "default" ).get() );

			std::cout << device << std::endl;

			// handle change of camera
			if ( device != mDevice )
			{
				if ( mDevice >= 0 )
				{
					mDeviceManager.unbindDevice( mDevice );
					mDevice = -1;
				}

				if ( mDeviceManager.bindDevice( device, width, height, fps ) )
				{
					mDevice = device;
					mWidth  = width;
					mHeight = height;
					mFps = fps;
				}
			}

			std::cout << mDevice << std::endl;

			if ( mDevice >=0 )
			{
				// handle change of resolution
				if ( width != mWidth || height != mHeight || fps != mFps )
				{
					if( !mDeviceManager.setCameraParams( mDevice, width, height, fps ) )	// failed ?
					{
						mDeviceManager.unbindDevice( mDevice );
						mDevice = -1;
						discardOutlets();
						return;
					}
					else
					{
						mWidth  = width;
						mHeight = height;
						mFps = fps;
					}
				}

				if ( mDeviceManager.isDeviceRunning( mDevice ) )
				{
					if ( mDeviceManager.isNewFrameAvailable( mDevice ) )
					{
						std::shared_ptr< CustomType > wOutData = mWidthOut.getWriteableRef();
						unsigned int &widthOut = *( wOutData->get< unsigned int >( "default" ).get() );
						widthOut = mDeviceManager.getVideoWidth( mDevice );

						std::shared_ptr< CustomType > hOutData = mHeightOut.getWriteableRef();
						unsigned int &heightOut = *( hOutData->get< unsigned int >( "default" ).get() );
						heightOut = mDeviceManager.getVideoHeight( mDevice );

						std::shared_ptr< CustomType > outData = mImageOut.getWriteableRef();
						std::shared_ptr< Image > outImage = Image::asImage( outData );
						Image &out = *outImage.get();
						out.setImagedata( mDeviceManager.getPixels( mDevice ), widthOut, heightOut, Image::ChannelOrder::RGB, Image::Datatype::UINT8 );
					}
					else
					{
						discardOutlets();
					}
				}
				else
				{
					discardOutlets();
					mDevice = -1;		// it was all running and ok but through a rescan a change in the device list got noticed and so all devices have to quit
				}
			}
			else
			{
				discardOutlets();
			}
		}
		else
		{
			discardOutlets();
		}
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}

void CameraCaptureBlock::shutdown()
{
	if ( mDevice >= 0 )
	{
		mDeviceManager.unbindDevice( mDevice );
		mDevice = -1;
	}
}

void CameraCaptureBlock::discardOutlets()
{
	mWidthOut.discard();
	mHeightOut.discard();
	mImageOut.discard();
}
