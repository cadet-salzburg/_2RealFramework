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
#include "OcvGaussianBlurBlock.h"
#include "ImageHelpers.h"
#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

OcvGaussianBlurBlock::OcvGaussianBlurBlock() :
	Block()
{
}

OcvGaussianBlurBlock::~OcvGaussianBlurBlock()
{
}

void OcvGaussianBlurBlock::setup( BlockHandle &block )
{
	try
	{
		mBlock = block;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
		Exception exc( e.what() );
		throw exc;
	}
}

void OcvGaussianBlurBlock::update()
{
	try
	{
		InletHandle hInImage = mBlock.getInletHandle( "in_image" );
		ParameterHandle hParamKx  = mBlock.getParameterHandle( "param_kernel_x" );
		ParameterHandle hParamKy  = mBlock.getParameterHandle( "param_kernel_y" );
		ParameterHandle hParamSx  = mBlock.getParameterHandle( "param_sigma_x" );
		ParameterHandle hParamSy  = mBlock.getParameterHandle( "param_sigma_y" );
		OutletHandle hOutImage = mBlock.getOutletHandle( "out_image" );

		std::shared_ptr< const CustomType > inData = hInImage.getReadableRef();
		std::shared_ptr< const Image > inImage = Image::asImage( inData );
		Image const& in = *inImage.get();

		//std::cout << in.getWidth() << " " << in.getHeight() << " " << ( std::string ) in.getDatatype() << " " << ( std::string ) in.getChannelOrder() << std::endl;

		std::shared_ptr< CustomType > outData = hOutImage.getWriteableRef();
		std::shared_ptr< Image > outImage = Image::asImage( outData );
		Image &out = *outImage.get();

		out.createImagedata( in.getWidth(), in.getHeight(), in.getChannelOrder(), in.getDatatype() );

		std::shared_ptr< const CustomType > kxData = hParamKx.getReadableRef();
		unsigned char kx = *( kxData->get< unsigned char >( "default" ).get() );

		std::shared_ptr< const CustomType > kyData = hParamKy.getReadableRef();
		unsigned char ky = *( kyData->get< unsigned char >( "default" ).get() );

		std::shared_ptr< const CustomType > sxData = hParamSx.getReadableRef();
		double sx = *( sxData->get< double >( "default" ).get() );

		std::shared_ptr< const CustomType > syData = hParamSy.getReadableRef();
		double sy = *( syData->get< double >( "default" ).get() );

		//std::cout << ( int ) kx << " " << ( int ) ky << " " << sx << " " << sy << std::endl;

		cv::Mat const* const matSrc = convertToCvMat( in );
		cv::Mat *const matDst = convertToCvMat( out );

		cv::GaussianBlur( *matSrc, *matDst, cv::Size( kx, ky ), sx, sy, 4 );

		delete matSrc;
		delete matDst;
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}

void OcvGaussianBlurBlock::shutdown()
{
	try
	{
	}
	catch( Exception & e )
	{
		cout << e.message() << " " << e.what() << endl;
		e.rethrow();
	}
	catch( std::exception & e )
	{
		cout << e.what() << endl;
	}
}