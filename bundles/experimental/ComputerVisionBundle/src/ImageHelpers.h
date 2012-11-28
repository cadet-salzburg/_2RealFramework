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
TData * makeCirclePattern( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz, float *black, float *white )
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
						*p = static_cast< TData >( black[ k ] );
						++p;
					}
				}
				else
				{
					for ( unsigned int k=0; k<channels; ++k )
					{
						*p = static_cast< TData >( white[ k ] );
						++p;
					}
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = static_cast< TData >( white[ k ] );
					++p;
				}
			}
		}
	}
	return data;
}

template< typename TData >
TData * makeColorCheckerboard( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz, float *black, float *white )
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
					*p = static_cast< TData >( black[ k ] );
					++p;
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = static_cast< TData >( white[ k ] );
					++p;
				}
			}
		}
	}
	return data;
}

template< typename TData >
TData * makeCheckerboard( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz )
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
					*p = static_cast< TData >( 255 );
					++p;
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = static_cast< TData >( 0 );
					++p;
				}
			}
		}
	}
	return data;
}

// ok, clearly i'm too stupid or something
template< >
inline unsigned short * makeCheckerboard( const unsigned int width, const unsigned int height, const unsigned char channels, const unsigned char sz )
{
	unsigned short *data = new unsigned short[ width * height * channels ];
	unsigned short *p = data;
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
					*p = static_cast< unsigned short >( 0xFF00 );
					++p;
				}
			}
			else
			{
				for ( unsigned int k=0; k<channels; ++k )
				{
					*p = static_cast< unsigned short >( 0x0000 );
					++p;
				}
			}
		}
	}
	return data;
}

inline cv::Mat *const convertToCvMat( Image &src )
{
	unsigned int width = src.getWidth();
	unsigned int height = src.getHeight();
	unsigned char *data = src.getData();
	void *d = static_cast< void * >( data );
	int cvType;

	if ( width == 0 || height == 0 )
	{
	}

	ImageType type = src.getImageType();

	unsigned int numChannels = src.getChannelOrder().getNumberOfChannels();
	switch ( numChannels )
	{
	case 1:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC1;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC1;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC1;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC1;
			break;
		default:
			cvType = CV_8UC1;
			break;
		}
		break;
	case 2:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC2;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC2;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC2;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC2;
			break;
		default:
			cvType = CV_8UC2;
			break;
		}
		break;
	case 3:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC3;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC3;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC3;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC3;
			break;
		default:
			cvType = CV_8UC3;
			break;
		}
		break;
	case 4:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC4;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC4;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC4;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC4;
			break;
		default:
			cvType = CV_8UC4;
			break;
		}
		break;
	default:
		cvType = CV_8UC1;
		break;
	}

	cv::Mat *result = new cv::Mat( src.getHeight(), src.getWidth(), cvType, d );

	return result;
}

inline cv::Mat const* const convertToCvMat( Image const& src )
{
	unsigned int width = src.getWidth();
	unsigned int height = src.getHeight();
	unsigned char * data = const_cast< unsigned char * >( src.getData() );
	void *d = static_cast< void * >( data );
	int cvType;

	if ( width == 0 || height == 0 )
	{
	}

	ImageType type = src.getImageType();

	unsigned int numChannels = src.getChannelOrder().getNumberOfChannels();
	switch ( numChannels )
	{
	case 1:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC1;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC1;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC1;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC1;
			break;
		default:
			cvType = CV_8UC1;
			break;
		}
		break;
	case 2:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC2;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC2;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC2;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC2;
			break;
		default:
			cvType = CV_8UC2;
			break;
		}
		break;
	case 3:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC3;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC3;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC3;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC3;
			break;
		default:
			cvType = CV_8UC3;
			break;
		}
		break;
	case 4:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			cvType = CV_8UC4;
			break;
		case ImageType::UNSIGNED_SHORT:
			cvType = CV_16UC4;
			break;
		case ImageType::FLOAT:
			cvType = CV_32FC4;
			break;
		case ImageType::DOUBLE:
			cvType = CV_64FC4;
			break;
		default:
			cvType = CV_8UC4;
			break;
		}
		break;
	default:
		cvType = CV_8UC1;
		break;
	}

	cv::Mat *result = new cv::Mat( src.getHeight(), src.getWidth(), cvType, d );

	return result;
}