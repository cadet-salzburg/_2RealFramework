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
#include "OcvHistogramNormalizationBlock.h"
#include "ImageHelpers.h"
#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>

using namespace _2Real::bundle;
using namespace _2Real;
using namespace std;

OcvEqualizeHistogramBlock::OcvEqualizeHistogramBlock() : Block()
{
}

OcvEqualizeHistogramBlock::~OcvEqualizeHistogramBlock()
{
}

void OcvEqualizeHistogramBlock::setup( BlockHandle &block )
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

void OcvEqualizeHistogramBlock::update()
{
	try
	{
		InletHandle hInImage =			mBlock.getInletHandle( "in_image" );
		OutletHandle hOutImage =		mBlock.getOutletHandle( "out_image" );

		std::shared_ptr< const CustomType > inData = hInImage.getReadableRef();
		std::shared_ptr< const Image > inImage = Image::asImage( inData );
		Image const& in = *inImage.get();

		int format = getCvImageDepth( in );
		if ( format < 0 )
		{
			std::cout << "wrong input image format" << std::endl;
			hOutImage.discard();
			return;
		}

		cv::Mat const* matSrc = convertToCvMat( in );

		if ( 1 != in.getChannelOrder().getNumberOfChannels() )
		{
			if ( 3 == in.getChannelOrder().getNumberOfChannels() )
			{
				cv::Mat *matGrey = new cv::Mat;
				cv::cvtColor( *matSrc, *matGrey, CV_RGB2GRAY );
				delete matSrc;
				matSrc = matGrey;
			}
			else if ( 4 == in.getChannelOrder().getNumberOfChannels() )
			{
				cv::Mat *matGrey = new cv::Mat;
				cv::cvtColor( *matSrc, *matGrey, CV_RGBA2GRAY );
				delete matSrc;
				matSrc= matGrey;
			}
			else
			{
				std::cout << "wrong input image format" << std::endl;
				hOutImage.discard();
				delete matSrc;
				return;
			}
		}

		if ( format != CV_8U )
		{
			cv::Mat *matTmp = new cv::Mat;
			matSrc->convertTo( *matTmp, CV_8U );
			delete matSrc;
			matSrc = matTmp;
		}

		std::shared_ptr< CustomType > outData = hOutImage.getWriteableRef();
		std::shared_ptr< Image > outImage = Image::asImage( outData );
		Image &out = *outImage.get();
		out.createImagedata( in.getWidth(), in.getHeight(), Image::ChannelOrder::RGB, Image::Datatype::UINT8 );
		cv::Mat * matDst = convertToCvMat( out );

		cv::Mat *matEq = new cv::Mat;
		cv::equalizeHist( *matSrc, *matEq );
		cv::cvtColor( *matEq, *matDst, CV_GRAY2RGB );

		delete matEq;

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

void OcvEqualizeHistogramBlock::shutdown()
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