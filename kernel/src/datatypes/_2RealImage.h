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

#include "datatypes/_2RealCustomData.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class TypeMetadata;

	class Image
	{

	public:

		// empty
		Image();
		// deep copy
		Image( Image const& other );

		static std::shared_ptr< Image >			asImage( std::shared_ptr< CustomType > data );
		static std::shared_ptr< const Image >	asImage( std::shared_ptr< const CustomType > data );

		static const std::string TYPENAME;
		static const std::string FIELD_WIDTH;
		static const std::string FIELD_HEIGHT;
		static const std::string FIELD_DATA;
		static const std::string FIELD_CHANNELS;
		static const std::string FIELD_DATATYPE;

		static std::shared_ptr< const TypeMetadata > getTypeMetadata();

		class ChannelOrder
		{
		public:
			enum Code
			{
				RGB,
				BGR,
				RGBA,
				BGRA,
				ARGB,
				SINGLECHANNEL,
				DUALCHANNEL,
				UNDEFINED,
			};
			ChannelOrder( const Code );
			explicit ChannelOrder( const int );
			explicit ChannelOrder( std::string const& );
			operator int() const;
			operator std::string() const;
			bool operator==( ChannelOrder const& other );
			unsigned int getNumberOfChannels() const;
			Code getCode() const;
		private:
			Code	mCode;
		};

		class Datatype
		{
		public:
			enum Code
			{
				UINT8,
				UINT16,
				UINT32,
				INT8,
				INT16,
				INT32,
				FLOAT32,
				FLOAT64,
				UNDEFINED,
			};
			Datatype( const Code );
			explicit Datatype( const int );
			explicit Datatype( std::string const& );
			operator int() const;
			operator std::string() const;
			bool operator==( Datatype const& other );
			size_t getBytesPerPixel() const;
			Code getCode() const;
		private:
			Code	mCode;
		};

		unsigned int					getWidth() const;
		unsigned int					getHeight() const;
		unsigned char const*			getPixels() const;
		unsigned char*					getPixels();
		Image::ChannelOrder				getChannelOrder() const;
		Image::Datatype					getDatatype() const;

		void createImagedata( const unsigned int w, const unsigned int h, const Image::ChannelOrder channels, const Image::Datatype type );

		template< typename T >
		void setImagedata( T const* data, const unsigned int w, const unsigned int h, const Image::ChannelOrder channels, const Image::Datatype type )
		{
			// TODO: some checks, if the format matches etc
			unsigned int bsz = w*h*channels.getNumberOfChannels()*sizeof( T ) / sizeof( unsigned char );

			std::shared_ptr< std::vector< unsigned char > > v = mData->get< std::vector< unsigned char > >( FIELD_DATA );
			v->resize( bsz );
			memcpy( &( *v.get() )[ 0 ], data, bsz );

			mData->set< unsigned int >( FIELD_WIDTH, w );
			mData->set< unsigned int >( FIELD_HEIGHT, h );
			mData->set< int >( FIELD_DATATYPE, type );
			mData->set< int >( FIELD_CHANNELS, channels );
		}

		std::shared_ptr< CustomType > toCustomType();
		std::shared_ptr< const CustomType > toCustomType() const;

	private:

		Image( std::shared_ptr< CustomType > );
		Image& operator=( Image const& other );

		std::shared_ptr< CustomType >			mData;

	};
}