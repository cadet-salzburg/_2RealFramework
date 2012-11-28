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

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName("CameraCaptureBundle");
		info.setDescription( "Camera Capture" );
		info.setAuthor( "Robert Praxmarer" );
		info.setCategory( "Devices" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		ContextBlockMetainfo contextBlockInfo = info.exportContextBlock< CameraDeviceManager >();
		BlockMetainfo cameraCapture = info.exportBlock< CameraCaptureBlock, WithContext >( "CameraCaptureBlock" );

		cameraCapture.addInlet<int>( "DeviceIndex", 0 );
		cameraCapture.addInlet<int>( "Width", 640 );
		cameraCapture.addInlet<int>( "Height", 480 );
		cameraCapture.addInlet<int>( "Fps", 30 );
		cameraCapture.addOutlet<Image>("ImageData");
		cameraCapture.addOutlet<int>( "Width" );
		cameraCapture.addOutlet<int>( "Height" );

		cameraCapture.setDescription( "Camera Capture" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}