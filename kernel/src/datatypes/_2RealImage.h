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

#include "datatypes/_2RealOption.h"
#include "datatypes/_2RealCustomData.h"
#include "datatypes/_2RealTypes.h"
#include "datatypes/_2RealBaseTypes.h"
#include "datatypes/_2RealCustomBase.h"

#include <vector>
#include <string>

namespace _2Real
{
	class Image : public CustomType
	{

	public:

		//class Datatype : public Option
		//{
		//public:

		//	//TODO
		//	enum Type { UNDEFINED = 0x0000, FLOAT32 = 0x0001, UINT8 = 0x0002 };
		//	Datatype( const Type t );
		//	operator Type() const;
		//	operator std::string() const;
		//	Datatype& operator=( Datatype const& other );
		//	bool operator==( Datatype const& other ) const;

		//private:

		//	Type		mType;

		//};

		//class ChannelOrder : public CustomType
		//{
		//public:

		//	//TODO
		//	enum Type { UNDEFINED, RGB, BGR };
		//	ChannelOrder( const Type t );
		//	operator Type() const;
		//	operator std::string() const;
		//	ChannelOrder& operator=( ChannelOrder const& other );
		//	bool operator==( ChannelOrder const& other );

		//private:

		//	Type		mValue;

		//};

		Image();
		Image& operator=( Image const& other );
		bool operator==( Image const& other ) const;

		unsigned int getWidth() const;
		unsigned int getHeight() const;
		unsigned char const* getPixels() const;
		//Image::Datatype getDatatype() const;
		//unsigned int getFormat() const;

		std::string const& getChannelOrder() const;
		std::string const& getDatatype() const;

		//template< typename T >
		//void set( std::vector< T > const& data, const unsigned int w, const unsigned int h );

		template< typename T >
		void setImagedata( T const* data, const unsigned int w, const unsigned int h, std::string const& type, std::string const& format/*const Image::Datatype type, const Image::ChannelOrder format*/ )
		{
			unsigned int sz = w*h;
			unsigned int bsz = w*h*sizeof( T ) / sizeof( unsigned char );

			std::vector< unsigned char > &v = CustomType::get< std::vector< unsigned char > >( "data" );
			v.resize( bsz );
			memcpy( &v[ 0 ], data, bsz );

			CustomType::set< unsigned int >( "width", w );
			CustomType::set< unsigned int >( "height", w );
			//CustomType::set< Image::Datatype >( "datatype", type );
			//CustomType::set< Image::ChannelOrder >( "format", format );
			CustomType::set< std::string >( "datatype", type );
			CustomType::set< std::string >( "format", format );
		}

		class Format
		{

		public:

			const static unsigned int UNDEFINED = 0;
			const static unsigned int FLOAT32 = 1;
			const static unsigned int FLOAT64 = 2;
			const static unsigned int UINT8 = 3;
			const static unsigned int UINT16 = 4;

			static std::string getFormatString( const unsigned int t )
			{
				return "TODO";
			}

		};

	private:

	};

	//template< >
	//struct Init< Image::ChannelOrder >
	//{
	//	static Image::ChannelOrder defaultValue()
	//	{
	//		return Image::ChannelOrder( Image::ChannelOrder::UNDEFINED );
	//	}
	//};

	//template< >
	//struct Name< Image::ChannelOrder >
	//{
	//	static std::string humanReadableName()
	//	{
	//		return "image channel order";
	//	}
	//};

	//template< >
	//struct Init< Image::Datatype >
	//{
	//	static Image::Datatype defaultValue()
	//	{
	//		return Image::Datatype( Image::Datatype::UNDEFINED );
	//	}
	//};

	//template< >
	//struct Name< Image::Datatype >
	//{
	//	static std::string humanReadableName()
	//	{
	//		return "image datatype";
	//	}
	//};

	template< >
	struct Init< Image >
	{
		static Image defaultValue()
		{
			return Image();
		}
	};

	template< >
	struct Name< Image >
	{
		static std::string humanReadableName()
		{
			return "image";
		}
	};

	template< >
	struct CustomDerivedType< Image >
	{
		static bool isCustomDerived()
		{
			return true;
		}

		static TypeMetadata *getTypeMetadata()
		{
			TypeMetadata *meta = new TypeMetadata( Name< Image >::humanReadableName() );
			meta->addField( "width",	new FieldDescriptor_t< unsigned int >( unsigned int( 0 ) ) );
			meta->addField( "height",	new FieldDescriptor_t< unsigned int >( unsigned int( 0 ) ) );
			meta->addField( "data",		new FieldDescriptor_t< std::vector< unsigned char > >( std::vector< unsigned char >() ) );
			//meta->addField( "format",		*( new FieldDescriptor_t< Image::ChannelOrder >( Init< Image::ChannelOrder >::defaultValue() ) ) );
			//meta->addField( "datatype",		*( new FieldDescriptor_t< Image::Datatype >( Init< Image::Datatype >::defaultValue() ) ) );
			meta->addField( "format",	new FieldDescriptor_t< std::string >( Init< std::string >::defaultValue() ) );
			meta->addField( "datatype",	new FieldDescriptor_t< std::string >( Init< std::string >::defaultValue() ) );
			return meta;
		}
	};

	//template< >
	//struct CustomDerivedType< Image::ChannelOrder >
	//{
	//	static bool isCustomDerived()
	//	{
	//		return true;
	//	}

	//	static TypeMetadata *getTypeMetadata()
	//	{
	//		TypeMetadata *meta = new TypeMetadata();
	//		meta->addField( "value",		*( new FieldDescriptor_t< unsigned int >( unsigned int( 0 ) ) ) );
	//		return meta;
	//	}
	//};

	//template< >
	//struct CustomDerivedType< Image::Datatype >
	//{
	//	static bool isCustomDerived()
	//	{
	//		return true;
	//	}

	//	static TypeMetadata *getTypeMetadata()
	//	{
	//		TypeMetadata *meta = new TypeMetadata();
	//		return meta;
	//	}
	//};
}