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
#include "OcvSobelBlock.h"
#include "OcvHistogramNormalizationBlock.h"
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

		float fblue[ 4 ] =	{ 0.2f, 0.1f, 0.7f, 1.0f };
		float fgreen[ 4 ] =	{ 0.1f, 0.7f, 0.2f, 1.0f };
		float fblack[ 4 ] =	{  0.f,  0.f,  0.f, 1.0f };
		float fwhite[ 4 ] =	{ 1.0f, 1.0f, 1.0f, 1.0f };
		unsigned char ublue[ 4 ] =	{ 100,  50, 200, 255 };
		unsigned char ugreen[ 4 ] =	{  50, 200, 100, 255 };
		unsigned char ublack[ 4 ] =	{   0,   0,   0, 255 };
		unsigned char uwhite[ 4 ] =	{ 255, 255, 255, 255 };

		float * initF1 = makeCirclePattern< float >( 480, 480, 1, 40, fwhite, fgreen );
		float * initF2 = makeCirclePattern< float >( 480, 480, 2, 40, fwhite, fgreen );
		float * initF3 = makeCirclePattern< float >( 480, 480, 3, 40, fwhite, fgreen );
		float * initF4 = makeCirclePattern< float >( 480, 480, 4, 40, fwhite, fgreen );
		unsigned char * initU1 = makeCirclePattern< unsigned char >( 480, 480, 1, 40, ublue, ublack );
		unsigned char * initU2 = makeCirclePattern< unsigned char >( 480, 480, 2, 40, ublue, ublack );
		unsigned char * initU3 = makeCirclePattern< unsigned char >( 480, 480, 3, 40, ublue, ublack );
		unsigned char * initU4 = makeCirclePattern< unsigned char >( 480, 480, 4, 40, ublue, ublack );

		_2Real::Image imgF1, imgF2, imgF3, imgF4, imgU1, imgU2, imgU3, imgU4;
		imgF1.setImagedata( initF1, 480, 480, Image::ChannelOrder::SINGLECHANNEL, Image::Datatype::FLOAT32 );
		imgF2.setImagedata( initF2, 480, 480, Image::ChannelOrder::DUALCHANNEL, Image::Datatype::FLOAT32 );
		imgF3.setImagedata( initF3, 480, 480, Image::ChannelOrder::RGB, Image::Datatype::FLOAT32 );
		imgF4.setImagedata( initF4, 480, 480, Image::ChannelOrder::RGBA, Image::Datatype::FLOAT32 );
		imgU1.setImagedata( initU1, 480, 480, Image::ChannelOrder::SINGLECHANNEL, Image::Datatype::UINT8 );
		imgU2.setImagedata( initU2, 480, 480, Image::ChannelOrder::DUALCHANNEL, Image::Datatype::UINT8 );
		imgU3.setImagedata( initU3, 480, 480, Image::ChannelOrder::RGB, Image::Datatype::UINT8 );
		imgU4.setImagedata( initU4, 480, 480, Image::ChannelOrder::RGBA, Image::Datatype::UINT8 );

		delete [] initF1;
		delete [] initF2;
		delete [] initF3;
		delete [] initF4;
		delete [] initU1;
		delete [] initU2;
		delete [] initU3;
		delete [] initU4;

		BlockMetainfo gauss = info.exportBlock< OcvGaussianBlurBlock, WithoutContext >( "OcvGaussianBlurBlock" );
		gauss.setDescription( "applies gaussian blur to input image" );
		gauss.setCategory( "image filter" );
		gauss.addCustomTypeInlet( "in_image", "image", imgF3.toCustomType() );
		gauss.addParameter< unsigned char >( "param_kernel_x", 7 );
		gauss.addParameter< unsigned char >( "param_kernel_y", 7 );
		gauss.addParameter< double >( "param_sigma_x", 2.1 );
		gauss.addParameter< double >( "param_sigma_y", 2.1 );
		//gauss.addInlet< int >( "boder_interpolation", 0, borderOptions );
		gauss.addCustomTypeOutlet( "out_image", "image" );

		BlockMetainfo sobel = info.exportBlock< OcvSobelBlock, WithoutContext >( "OcvSobelBlock" );
		sobel.setDescription( "applies sobel to input image" );
		sobel.setCategory( "image filter" );
		sobel.addCustomTypeInlet( "in_image", "image", imgU3.toCustomType() );
		sobel.addParameter< unsigned char >( "param_order_x", 1 );
		sobel.addParameter< unsigned char >( "param_order_y", 1 );
		sobel.addParameter< unsigned char >( "param_aperture", 1 );
		//sobel.addInlet< int >( "boder_interpolation", 0, borderOptions );
		sobel.addCustomTypeOutlet( "out_image", "image" );

		BlockMetainfo equalize = info.exportBlock< OcvEqualizeHistogramBlock, WithoutContext >( "OcvEqualizeHistogramBlock" );
		equalize.setDescription( "applies histogram normalization to input image" );
		equalize.setCategory( "image filter" );
		equalize.addCustomTypeInlet( "in_image", "image", imgF1.toCustomType() );
		equalize.addCustomTypeOutlet( "out_image", "image" );
	}
	catch ( Exception &e )
	{
		cout << e.message() << endl;
		e.rethrow();
	}
}