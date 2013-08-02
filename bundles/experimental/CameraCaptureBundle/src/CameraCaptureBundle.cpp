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
#include "CameraCaptureBlock.h"

#include "_2RealBundle.h"
#include <sstream>
#include <iostream>

using namespace _2Real;

using namespace _2Real::bundle;
using			_2Real::Exception;
using std::string;
using std::cout;
using std::endl;

template< typename TData >
TData * makeCheckerPattern( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz, TData *black, TData *white )
{
	TData *data = new TData[ width * height * channels ];
	TData *p = data;
	for ( unsigned int i=0; i<height; ++i )
	{
		unsigned int divI = i/sz;
		for ( unsigned int j=0; j<width; ++j )
		{
			unsigned int divJ = j/sz;
			if ( ( divJ%2 == 0 ) ^ ( divI%2 == 0 ) )
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = black[ k ];
					++p;
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = white[ k ];
					++p;
				}
			}
		}
	}
	return data;
}

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName("CameraCaptureBundle");
		info.setDescription( "Camera Capture" );
		info.setAuthor( "The Grandmaster" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();
		FunctionBlockMetainfo cameraCapture = info.exportFunctionBlock< CameraCaptureBlock, WithContext >( "CameraCaptureBlock" );

		cameraCapture.addParameter< unsigned int >( "device", 0 );
		cameraCapture.addParameter< unsigned int >( "width", 640 );
		cameraCapture.addParameter< unsigned int >( "height", 480 );
		cameraCapture.addParameter< unsigned int >( "fps", 30 );

		cameraCapture.addCustomTypeOutlet( "image", "image" );
		cameraCapture.addOutlet< unsigned int >( "width" );
		cameraCapture.addOutlet< unsigned int >( "height" );

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}