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
#include "_2RealBundle.h"
#include "OcvGaussianBlurBlock.h"
#include "ImageHelpers.h"
#include <sstream>
#include <iostream>

using namespace _2Real;
using namespace _2Real::bundle;

using std::string;
using std::cout;
using std::endl;

void getBundleMetainfo( BundleMetainfo& info )
{
	try
	{
		info.setName( "ComputerVisionBundle" );
		info.setDescription( "computer vision stuff" );
		info.setAuthor( "gerlinde emsenhuber" );
		info.setCategory( "image processing" );
		info.setContact( "help@cadet.at" );
		info.setVersion( 0, 1, 0 );

		float blue[ 4 ] = { 0.2f, 0.1f, 0.7f, 1.0f };
		float green[ 4 ] = { 0.1f, 0.7f, 0.2f, 1.0f };
		float black[ 4 ] = { 0.f, 0.f, 0.f, 1.0f };
		float white[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };

		float *initA = makeCirclePattern< float >( 480, 480, 4, 20, white, green );
		float *initB = makeCirclePattern< float >( 480, 480, 4, 40, blue, black );

		_2Real::Image imgA;
		_2Real::Image imgB;
		imgA.setImagedata( initA, 480, 480, Image::ChannelOrder::RGBA, Image::Datatype::FLOAT32 );
		imgB.setImagedata( initB, 480, 480, Image::ChannelOrder::RGBA, Image::Datatype::FLOAT32 );

		delete initA;
		delete initB;

		BlockMetainfo gauss = info.exportBlock< OcvGaussianBlurBlock, WithoutContext >( "OcvGaussianBlurBlock" );
		gauss.setDescription( "applies gaussian blur to input image" );
		gauss.setCategory( "image filter" );

		gauss.addCustomTypeInlet( "InImageA", "image", imgA.toCustomType() );
		gauss.addCustomTypeInlet( "InImageB", "image", imgB.toCustomType() );
		//gauss.addInlet< Image >( "InImageB", imgB.toCustomType() );
		//gauss.addInlet< int >( "InKernelX", 10 );
		//gauss.addCustomTypeInlet( "InKernelY", "int", ToCustomType< int >( 10 ) );
		gauss.addCustomTypeOutlet( "OutImage", "image" );

		//gauss.addInlet< int >( "InInt", "int", 10 );
		//gauss.addInlet( "OutImage", "Image" );
		//gauss.addInlet< unsigned char >( "kernel_x", 1 );
		//gauss.addInlet< unsigned char >( "kernel_y", 1 );
		//gauss.addInlet< double >( "sigma_x", 1.1 );
		//gauss.addInlet< double >( "sigma_y", 1.1 );
		//gauss.addInlet< int >( "boder_interpolation", 0, borderOptions );
		//gauss.addOutlet< Image >( "OutImage" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}