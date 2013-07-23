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

#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

using namespace _2Real;

template< typename TData >
TData * makeCirclePattern( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz, TData *black, TData *white )
{
	float radius = 0.5f*sz;

	TData *data = new TData[ width * height * channels ];
	TData *p = data;
	for ( unsigned int i=0; i<height; ++i )
	{
		unsigned int divI = i/sz;
		unsigned int modI = i%sz;
		for ( unsigned int j=0; j<width; ++j )
		{
			unsigned int divJ = j/sz;
			unsigned int modJ = j%sz;
			if ( ( divJ%2 == 0 ) ^ ( divI%2 == 0 ) )
			{
				float x = modI - radius;
				float y = modJ - radius;
				float dist = sqrt( x*x + y*y );

				if ( dist < radius )
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

inline int getCvImageDepth( Image const& src )
{
	const Image::Datatype type = src.getDatatype();
	switch( type.getCode() )
	{
	case Image::Datatype::UINT8:
		return CV_8U;
	case Image::Datatype::UINT16:
		return CV_16U;
	case Image::Datatype::UINT32:
		return -2;
	case Image::Datatype::INT8:
		return CV_8S;
	case Image::Datatype::INT16:
		return CV_16S;
	case Image::Datatype::INT32:
		return CV_32S;
	case Image::Datatype::FLOAT32:
		return CV_32F;
	case Image::Datatype::FLOAT64:
		return CV_64F;
	default:
		return -1;
	}
}

inline cv::Mat *const convertToCvMat( Image &src )
{
	const unsigned int w = src.getWidth();
	const unsigned int h = src.getHeight();
	unsigned char *const data = src.getPixels();
	const Image::Datatype type = src.getDatatype();
	unsigned int numChannels = src.getChannelOrder().getNumberOfChannels();
	void *const d = static_cast< void * >( data );

	int cvType;

	if ( w == 0 || h == 0 )
		return new cv::Mat;

	switch ( numChannels )
	{
	case 1:
		switch( type.getCode() )
		{
		case Image::Datatype::UINT8:
			cvType = CV_8UC1;
			break;
		case Image::Datatype::UINT16:
			cvType = CV_16UC1;
			break;
		case Image::Datatype::UINT32:
			throw std::exception( "unsupported image format - opencv does not support 32 bit unsigned integer images" );
		case Image::Datatype::INT8:
			cvType = CV_8SC1;
			break;
		case Image::Datatype::INT16:
			cvType = CV_16SC1;
			break;
		case Image::Datatype::INT32:
			cvType = CV_32SC1;
			break;
		case Image::Datatype::FLOAT32:
			cvType = CV_32FC1;
			break;
		case Image::Datatype::FLOAT64:
			cvType = CV_64FC1;
			break;
		default:
			throw std::exception( "unknown image format" );
		}
		break;
	case 2:
		switch( type.getCode() )
		{
		case Image::Datatype::UINT8:
			cvType = CV_8UC2;
			break;
		case Image::Datatype::UINT16:
			cvType = CV_16UC2;
			break;
		case Image::Datatype::UINT32:
			throw std::exception( "unsupported image format - opencv does not support 32 bit unsigned integer images" );
		case Image::Datatype::INT8:
			cvType = CV_8SC2;
			break;
		case Image::Datatype::INT16:
			cvType = CV_16SC2;
			break;
		case Image::Datatype::INT32:
			cvType = CV_32SC2;
			break;
		case Image::Datatype::FLOAT32:
			cvType = CV_32FC2;
			break;
		case Image::Datatype::FLOAT64:
			cvType = CV_64FC2;
			break;
		default:
			throw std::exception( "unknown image format" );
		}
		break;
	case 3:
		switch( type.getCode() )
		{
		case Image::Datatype::UINT8:
			cvType = CV_8UC3;
			break;
		case Image::Datatype::UINT16:
			cvType = CV_16UC3;
			break;
		case Image::Datatype::UINT32:
			throw std::exception( "unsupported image format - opencv does not support 32 bit unsigned integer images" );
		case Image::Datatype::INT8:
			cvType = CV_8SC3;
			break;
		case Image::Datatype::INT16:
			cvType = CV_16SC3;
			break;
		case Image::Datatype::INT32:
			cvType = CV_32SC3;
			break;
		case Image::Datatype::FLOAT32:
			cvType = CV_32FC3;
			break;
		case Image::Datatype::FLOAT64:
			cvType = CV_64FC3;
			break;
		default:
			throw std::exception( "unknown image format" );
		}
		break;
	case 4:
		switch( type.getCode() )
		{
		case Image::Datatype::UINT8:
			cvType = CV_8UC4;
			break;
		case Image::Datatype::UINT16:
			cvType = CV_16UC4;
			break;
		case Image::Datatype::UINT32:
			throw std::exception( "unsupported image format - opencv does not support 32 bit unsigned integer images" );
		case Image::Datatype::INT8:
			cvType = CV_8SC4;
			break;
		case Image::Datatype::INT16:
			cvType = CV_16SC4;
			break;
		case Image::Datatype::INT32:
			cvType = CV_32SC4;
			break;
		case Image::Datatype::FLOAT32:
			cvType = CV_32FC4;
			break;
		case Image::Datatype::FLOAT64:
			cvType = CV_64FC4;
			break;
		default:
			throw std::exception( "unknown image format" );
		}
		break;
	default:
		throw std::exception( "unknown image format" );
	}

	cv::Mat *result = new cv::Mat( w, h, cvType, d );
	return result;
}

inline cv::Mat const* const convertToCvMat( Image const& src )
{
	Image &tmp = const_cast< Image & >( src );
	return convertToCvMat( tmp );
}