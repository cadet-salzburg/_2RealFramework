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

#include "datatypes/_2RealImage.h"

namespace _2Real
{
	const std::string Image::FIELD_WIDTH = "width";
	const std::string Image::FIELD_HEIGHT = "height";
	const std::string Image::FIELD_DATA = "data";
	const std::string Image::FIELD_CHANNELS = "channelorder";
	const std::string Image::FIELD_DATATYPE = "datatype";

	std::shared_ptr< Image > Image::asImage( std::shared_ptr< CustomType > data )
	{
		Image *img = new Image;
		img->mData = data;
		return std::shared_ptr< Image >( img );
	}

	std::shared_ptr< const Image > Image::asImage( std::shared_ptr< const CustomType > data )
	{
		Image *img = new Image;
		img->mData = std::const_pointer_cast< CustomType >( data );
		return std::shared_ptr< const Image >( img );
	}

	Image::Image() : mData( new CustomType )
	{
		std::shared_ptr< TypeMetadata > init( CustomDerivedType< Image >::getTypeMetadata() );
		mData->initFrom( *( init.get() ) );
	}

	Image::Image( Image const& other ) : mData()
	{
		if ( other.mData.get() )
		{
			mData.reset( new CustomType( *( other.mData.get() ) ) );
		}
	}

	void Image::createImagedata( const unsigned int w, const unsigned int h, const Image::ChannelOrder channels, const Image::Datatype type )
	{
		size_t bsz = w*h*channels.getNumberOfChannels()*type.getBytesPerPixel();
		std::vector< unsigned char > &v = mData->get< std::vector< unsigned char > >( FIELD_DATA );
		v.resize( bsz );
		memset( &v[ 0 ], 0, bsz );

		mData->set< unsigned int >( FIELD_WIDTH, w );
		mData->set< unsigned int >( FIELD_HEIGHT, h );
		mData->set< int >( FIELD_DATATYPE, type );
		mData->set< int >( FIELD_CHANNELS, channels );
	}

	std::shared_ptr< CustomType > Image::toCustomType()
	{
		return mData;
	}

	std::shared_ptr< const CustomType > Image::toCustomType() const
	{
		return mData;
	}

	Image::operator std::shared_ptr< CustomType > ()
	{
		return mData;
	}

	Image::operator std::shared_ptr< const CustomType > () const
	{
		return mData;
	}

	unsigned int Image::getWidth() const
	{
		return mData->get< unsigned int >( Image::FIELD_WIDTH );
	}

	unsigned int Image::getHeight() const
	{
		return mData->get< unsigned int >( Image::FIELD_HEIGHT );
	}

	unsigned char const* Image::getPixels() const
	{
		return &( mData->get< std::vector< unsigned char > >( Image::FIELD_DATA ) )[ 0 ];
	}

	unsigned char* Image::getPixels()
	{
		return &( mData->get< std::vector< unsigned char > >( Image::FIELD_DATA ) )[ 0 ];
	}

	Image::ChannelOrder Image::getChannelOrder() const
	{
		return Image::ChannelOrder( mData->get< int >( Image::FIELD_CHANNELS ) );
	}

	Image::Datatype Image::getDatatype() const
	{
		return Image::Datatype( mData->get< int >( Image::FIELD_DATATYPE ) );
	}

	Image::ChannelOrder::ChannelOrder( const Code c ) : mCode( c )
	{
	}

	Image::ChannelOrder::ChannelOrder( const int i )
	{
		switch( i )
		{
		case 0:
			mCode = RGB;
			break;
		case 1:
			mCode = BGR;
			break;
		case 2:
			mCode = RGBA;
			break;
		case 3:
			mCode = BGRA;
			break;
		default:
			mCode = UNDEFINED;
		}
	}

	Image::ChannelOrder::ChannelOrder( std::string const& s )
	{
		if ( s == "rgb" )
		{
			mCode = RGB;
		}
		else if ( s == "bgr" )
		{
			mCode = BGR;
		}
		else if ( s == "rgba" )
		{
			mCode = RGBA;
		}
		else if ( s == "bgra" )
		{
			mCode = BGRA;
		}
		else
		{
			mCode = UNDEFINED;
		}
	}

	Image::ChannelOrder::operator int() const
	{
		switch( mCode )
		{
		case RGB:
			return 0;
			break;
		case BGR:
			return 1;
			break;
		case RGBA:
			return 2;
			break;
		case BGRA:
			return 3;
			break;
		default:
			return -1;
		}
	}

	Image::ChannelOrder::operator std::string() const
	{
		switch( mCode )
		{
		case RGB:
			return "rgb";
			break;
		case BGR:
			return "bgr";
			break;
		case RGBA:
			return "rgba";
			break;
		case BGRA:
			return "bgra";
			break;
		default:
			return "undefined";
		}
	}

	bool Image::ChannelOrder::operator==( ChannelOrder const& other )
	{
		return mCode == other.mCode;
	}

	unsigned int Image::ChannelOrder::getNumberOfChannels() const
	{
		switch( mCode )
		{
		case RGB:
			return 3;
			break;
		case BGR:
			return 3;
			break;
		case RGBA:
			return 4;
			break;
		case BGRA:
			return 4;
			break;
		default:
			return 0;
		}
	}

	Image::Datatype::Datatype( const Code c ) : mCode( c )
	{
	}

	Image::Datatype::Datatype( const int i )
	{
		switch( i )
		{
		case 0:
			mCode = UINT8;
			break;
		case 1:
			mCode = UINT16;
			break;
		case 2:
			mCode = UINT32;
			break;
		case 3:
			mCode = FLOAT32;
			break;
		default:
			mCode = UNDEFINED;
		}
	}

	Image::Datatype::Datatype( std::string const& s )
	{
		if ( s == "uint8" )
		{
			mCode = UINT8;
		}
		else if ( s == "uint16" )
		{
			mCode = UINT16;
		}
		else if ( s == "uint32" )
		{
			mCode = UINT32;
		}
		else if ( s == "float32" )
		{
			mCode = FLOAT32;
		}
		else
		{
			mCode = UNDEFINED;
		}
	}

	Image::Datatype::operator int() const
	{
		switch( mCode )
		{
		case UINT8:
			return 0;
			break;
		case UINT16:
			return 1;
			break;
		case UINT32:
			return 2;
			break;
		case FLOAT32:
			return 3;
			break;
		default:
			return -1;
		}
	}

	Image::Datatype::operator std::string() const
	{
		switch( mCode )
		{
		case UINT8:
			return "uint8";
			break;
		case UINT16:
			return "uint16";
			break;
		case UINT32:
			return "uint32";
			break;
		case FLOAT32:
			return "float32";
			break;
		default:
			return "undefined";
		}
	}

	bool Image::Datatype::operator==( Datatype const& other )
	{
		return mCode == other.mCode;
	}

	size_t Image::Datatype::getBytesPerPixel() const
	{
		switch( mCode )
		{
		case UINT8:
			return 1;
			break;
		case UINT16:
			return 2;
			break;
		case UINT32:
			return 4;
			break;
		case FLOAT32:
			return 4;
			break;
		default:
			return 0;
		}
	}
}