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
	Image::Image() : CustomType()
	{
		//initField( "data", new AnyHolder< std::vector< unsigned char > >() );
		//initField( "width", new AnyHolder< unsigned int >() );
		//initField( "height", new AnyHolder< unsigned int >() );
		//initField( "datatype", new AnyHolder< Image::Datatype >() );
		//initField( "format", new AnyHolder< Image::ChannelOrder >() );
		//initField( "datatype", new AnyHolder< Image::Datatype >() );
		//initField( "format", new AnyHolder< Image::ChannelOrder >() );
		std::shared_ptr< TypeMetadata > init( CustomDerivedType< Image >::getTypeMetadata() );
		initFrom( *( init.get() ) );
	}

	Image& Image::operator=( Image const& other )
	{
		if ( this == &other )
			return *this;

		iter( "data" )->second = other.constIter( "data" )->second->clone();
		iter( "width" )->second = other.constIter( "width" )->second->clone();
		iter( "height" )->second = other.constIter( "height" )->second->clone();
		iter( "datatype" )->second = other.constIter( "datatype" )->second->clone();
		iter( "format" )->second = other.constIter( "format" )->second->clone();

		//CustomType::operator=( other );

		return *this;
	}

	bool Image::operator==( Image const& other ) const
	{
		bool equal = true;
		equal &= constIter( "data" )->second->isEqualTo( *( other.constIter( "data" )->second ) );
		equal &= constIter( "width" )->second->isEqualTo( *( other.constIter( "width" )->second ) );
		equal &= constIter( "height" )->second->isEqualTo( *( other.constIter( "height" )->second ) );
		equal &= constIter( "datatype" )->second->isEqualTo( *( other.constIter( "datatype" )->second ) );
		equal &= constIter( "format" )->second->isEqualTo( *( other.constIter( "format" )->second ) );
		return equal;
	}

	unsigned int Image::getWidth() const
	{
		return get< unsigned int >( "width" );
	}

	unsigned int Image::getHeight() const
	{
		return get< unsigned int >( "height" );
	}

	unsigned char const* Image::getPixels() const
	{
		return &( get< std::vector< unsigned char > >( "data" ) )[ 0 ];
	}

	std::string const& Image::getChannelOrder() const
	{
		return get< std::string >( "format" );
	}

	std::string const& Image::getDatatype() const
	{
		return get< std::string >( "datatype" );
	}

	//void Image::setInternal( std::vector< unsigned char > const& data, const unsigned int w, const unsigned int h, const Image::Datatype type, const Image::ChannelOrder format )
	//{
	//	CustomType::set< std::vector< unsigned char > >( "data", data );
	//	CustomType::set< unsigned int >( "width", w );
	//	CustomType::set< unsigned int >( "height", h );
	//	CustomType::set< Image::Datatype >( "datatype", Image::Datatype( Image::Datatype::UINT8 ) );
	//}

	//Image::Datatype::Datatype( const Type t ) : Option( t ), mType( t )
	//{
	//}

	//Image::Datatype::operator Type() const
	//{
	//	return mType;
	//}

	//Image::Datatype::operator std::string() const
	//{
	//	return "THIS IS A TODO :)";
	//}

	//Image::Datatype& Image::Datatype::operator=( Image::Datatype const& other )
	//{
	//	if ( this == &other )
	//		return *this;

	//	Option::operator=( other );
	//	mType = other.mType;

	//	return *this;
	//}

	//bool Image::Datatype::operator==( Image::Datatype const& other ) const
	//{
	//	return mType == other.mType;
	//}

	//Image::ChannelOrder::ChannelOrder( const Type t )
	//{
	//}

	//Image::ChannelOrder::operator Type() const
	//{
	//	return UNDEFINED;
	//}

	//Image::ChannelOrder::operator std::string() const
	//{
	//	return "undefined";
	//}

	//Image::ChannelOrder& Image::ChannelOrder::operator=( Image::ChannelOrder const& other )
	//{
	//	return *this;
	//}

	//bool Image::ChannelOrder::operator==( Image::ChannelOrder const& other )
	//{
	//	return false;
	//}

}