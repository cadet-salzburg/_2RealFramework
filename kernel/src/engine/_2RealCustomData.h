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

#include "engine/_2RealAbstractAnyHolder.h"
#include "engine/_2RealFrameworkCompatibleType.h"
#include "helpers/_2RealStdIncludes.h"

namespace _2Real
{
	class SharedTypeMetainfo;

	/*
	*	actualy data, initialized from SharedTypeMetainfo
	*/
	class CustomData
	{

		friend class SharedTypeMetainfo;

	public:

		CustomData();
		CustomData& operator=( CustomData const& other );
		CustomData( CustomData const& other );
		~CustomData();

		// field getters & setters
		/////////////////////////////////////////////////////////////////////////
		template< typename TType >
		void set( std::string const& fieldName, TType const& value )
		{
			setValueInternal( fieldName, new AnyHolder< TType >( value ) );
		}

		template< typename TType >
		std::shared_ptr< const TType > get( std::string const& fieldName ) const
		{
			AbstractAnyHolder const* value = getValueInternal( fieldName );
			std::shared_ptr< const TType > result = extract< TType >( *value );
			return result;
		}

		template< typename TType >
		std::shared_ptr< TType > get( std::string const& fieldName )
		{
			AbstractAnyHolder *value = getValueInternal( fieldName );
			std::shared_ptr< TType > result = extract< TType >( *value );
			return result;
		}
		/////////////////////////////////////////////////////////////////////////

		bool operator==( CustomData const& other );
		bool operator<( CustomData const& other );

	private:

		typedef std::map< std::string, std::shared_ptr< _2Real::AbstractAnyHolder > >	DataFields;

		DataFields										mDataFields;
		//std::weak_ptr< const SharedTypeMetainfo >		mMetainfo;

		void										setValueInternal( std::string const& fieldName, std::shared_ptr< AbstractAnyHolder > );
		std::shared_ptr< const AbstractAnyHolder >	getValueInternal( std::string const& fieldName ) const;
		std::shared_ptr< AbstractAnyHolder >		getValueInternal( std::string const& fieldName );

	};

	template< >
	struct FrameworkCompatibleType< CustomData >
	{
		static std::string humanReadableName()
		{
			return "custom";
		}

		static CustomData defaultValue()
		{
			return CustomData();
		}

		static void writeTo( std::ostream &out, CustomData const& data )
		{
			( void )( out );
			( void )( data );
		}

		static void readFrom( std::istream &in, CustomData &data )
		{
			( void )( in );
			( void )( data );
		}
	};
}
