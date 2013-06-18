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
		initField( "data", new AnyHolder< std::vector< unsigned char > >() );
		initField( "width", new AnyHolder< unsigned int >() );
		initField( "height", new AnyHolder< unsigned int >() ); 
	}

	Image& Image::operator=( Image const& other )
	{
		if ( this == &other )
			return *this;

		iter( "data" )->second = other.constIter( "data" )->second->clone();
		iter( "width" )->second = other.constIter( "width" )->second->clone();
		iter( "height" )->second = other.constIter( "height" )->second->clone();

		return *this;
	}

	bool Image::operator==( Image const& other ) const
	{
		bool equal = true;
		equal &= constIter( "data" )->second->isEqualTo( *( other.constIter( "data" )->second ) );
		equal &= constIter( "width" )->second->isEqualTo( *( other.constIter( "width" )->second ) );
		equal &= constIter( "height" )->second->isEqualTo( *( other.constIter( "height" )->second ) );
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

	void Image::set( std::vector< unsigned char > const& data, const unsigned int w, const unsigned int h )
	{
		CustomType::set< std::vector< unsigned char > >( "data", data );
		CustomType::set< unsigned int >( "width", w );
		CustomType::set< unsigned int >( "height", h );
	}
}