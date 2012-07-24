#pragma once

#include "_2RealDatatypes.h"
#include "opencv2/imgproc/imgproc.hpp"

using namespace _2Real;

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

inline cv::Mat * convertToCvMat( ImageSource const& src )
{
	unsigned int width = src.getWidth();
	unsigned int height = src.getHeight();
	unsigned char *data = src.getData();
	void *d = static_cast< void * >( data );
	int cvType;

	if ( width == 0 || height == 0 )
	{
		// no image size was defined
	}

	ImageType type = src.getImageType();

	//switch( type.getDatatype() )
	//{
	//case ImageType::UNSIGNED_BYTE:
	//	cvType = CV_8UC1;
	//	break;
	//case ImageType::UNSIGNED_SHORT:
	//	cvType = CV_16UC1;
	//	break;
	//case ImageType::FLOAT:
	//	cvType = CV_32FC1;
	//	break;
	//case ImageType::DOUBLE:
	//	cvType = CV_64FC1;
	//	break;
	//default:
	//	cvType = CV_8UC1;
	//	break;
	//}

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

	cv::Mat *result = new cv::Mat( src.getWidth(), src.getHeight(), cvType, d );
	return result;
}