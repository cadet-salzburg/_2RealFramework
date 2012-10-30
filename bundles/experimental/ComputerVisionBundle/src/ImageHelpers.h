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
			//std::cout << "8UC1" << std::endl;
			cvType = CV_8UC1;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "YYYY 16UC1" << std::endl;
			cvType = CV_16UC1;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC1" << std::endl;
			cvType = CV_32FC1;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC1" << std::endl;
			cvType = CV_64FC1;
			break;
		default:
			std::cout << "default, 8UC1" << std::endl;
			cvType = CV_8UC1;
			break;
		}
		break;
	case 2:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			std::cout << "8UC2" << std::endl;
			cvType = CV_8UC2;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "16UC2" << std::endl;
			cvType = CV_16UC2;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC2" << std::endl;
			cvType = CV_32FC2;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC2" << std::endl;
			cvType = CV_64FC2;
			break;
		default:
			std::cout << "default, 8UC2" << std::endl;
			cvType = CV_8UC2;
			break;
		}
		break;
	case 3:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			std::cout << "8UC3" << std::endl;
			cvType = CV_8UC3;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "16UC3" << std::endl;
			cvType = CV_16UC3;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC3" << std::endl;
			cvType = CV_32FC3;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC3" << std::endl;
			cvType = CV_64FC3;
			break;
		default:
			std::cout << "default, 8UC3" << std::endl;
			cvType = CV_8UC3;
			break;
		}
		break;
	case 4:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			std::cout << "8UC4" << std::endl;
			cvType = CV_8UC4;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "16UC4" << std::endl;
			cvType = CV_16UC4;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC4" << std::endl;
			cvType = CV_32FC4;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC4" << std::endl;
			cvType = CV_64FC4;
			break;
		default:
			std::cout << "default, 8UC4" << std::endl;
			cvType = CV_8UC4;
			break;
		}
		break;
	default:
		std::cout << "default default, 8UC1" << std::endl;
		cvType = CV_8UC1;
		break;
	}

	cv::Mat *result = new cv::Mat( src.getWidth(), src.getHeight(), cvType, d );
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
			//std::cout << "8UC1" << std::endl;
			cvType = CV_8UC1;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "XXXX 16UC1" << std::endl;
			cvType = CV_16UC1;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC1" << std::endl;
			cvType = CV_32FC1;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC1" << std::endl;
			cvType = CV_64FC1;
			break;
		default:
			std::cout << "default, 8UC1" << std::endl;
			cvType = CV_8UC1;
			break;
		}
		break;
	case 2:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			std::cout << "8UC2" << std::endl;
			cvType = CV_8UC2;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "16UC2" << std::endl;
			cvType = CV_16UC2;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC2" << std::endl;
			cvType = CV_32FC2;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC2" << std::endl;
			cvType = CV_64FC2;
			break;
		default:
			std::cout << "default, 8UC2" << std::endl;
			cvType = CV_8UC2;
			break;
		}
		break;
	case 3:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			std::cout << "8UC3" << std::endl;
			cvType = CV_8UC3;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "16UC3" << std::endl;
			cvType = CV_16UC3;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC3" << std::endl;
			cvType = CV_32FC3;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC3" << std::endl;
			cvType = CV_64FC3;
			break;
		default:
			std::cout << "default, 8UC3" << std::endl;
			cvType = CV_8UC3;
			break;
		}
		break;
	case 4:
		switch( type.getDatatype() )
		{
		case ImageType::UNSIGNED_BYTE:
			std::cout << "8UC4" << std::endl;
			cvType = CV_8UC4;
			break;
		case ImageType::UNSIGNED_SHORT:
			std::cout << "16UC4" << std::endl;
			cvType = CV_16UC4;
			break;
		case ImageType::FLOAT:
			std::cout << "32FC4" << std::endl;
			cvType = CV_32FC4;
			break;
		case ImageType::DOUBLE:
			std::cout << "64FC4" << std::endl;
			cvType = CV_64FC4;
			break;
		default:
			std::cout << "default, 8UC4" << std::endl;
			cvType = CV_8UC4;
			break;
		}
		break;
	default:
		std::cout << "default default, 8UC1" << std::endl;
		cvType = CV_8UC1;
		break;
	}

	cv::Mat *result = new cv::Mat( src.getWidth(), src.getHeight(), cvType, d );
	return result;
}