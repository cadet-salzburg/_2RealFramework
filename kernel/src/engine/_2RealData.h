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

#include "helpers/_2RealBoost.h"
#include "helpers/_2RealException.h"

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_LIST_SIZE 40

#include <boost/variant.hpp>

namespace _2Real
{
	class CustomDataItem;

	/*
		this is our datatype
	*/
	typedef boost::variant< uint8_t,// std::vector< uint8_t >,
							int8_t,// std::vector< int8_t >,
							uint32_t,// std::vector< uint32_t >,
							int32_t,// std::vector< int32_t >,
							uint64_t,// std::vector< uint64_t >,
							int64_t,// std::vector< int64_t >,
							double,// std::vector< double >,
							float,// std::vector< float >,
							std::string,// std::vector< std::string >,
							bool,// std::vector< bool >,
							boost::recursive_wrapper< CustomDataItem >//, std::vector< boost::recursive_wrapper< CustomType > > >
							> DataItem;


	/*
	*	custom data item consists of data fields
	*/
	class DataField
	{

	public:

		std::string		mFieldName;
		DataItem		mValue;

	};

	class CustomDataItem
	{

	public:

		typedef std::vector< DataField >		DataFields;
		DataFields								mDataFields;
		std::string								mTypename;

		CustomDataItem& operator=( CustomDataItem const& other )
		{
			return *this;
		}

		// everything else can be solved with the visitor pattern
		std::string const& getHumanReadableName() const
		{
			return  mTypename;
		}

		friend std::ostream& operator<<( std::ostream& out, CustomDataItem const& val )
		{
			for ( auto it : val.mDataFields )
				// TODO: visitor b/c of formatting
				out << it.mFieldName << " is " << std::boolalpha << it.mValue << std::endl;
			return out;
		}

		template< typename TData >
		TData const& getValue( std::string const& fieldName ) const
		{
			try
			{
				for ( auto it : mFields )
				{
					if ( it.mName == fieldName )
					{
						TData const& result = boost::get< TData >( it.mValue );
						return result;
					}
				}
			}
			catch ( boost::bad_get const& e )
			{
				throw e;
			}

			throw _2Real::NotFoundException( fieldName );
		}

		template< typename TData >
		TData & getValue( std::string const& fieldName )
		{
			try
			{
				for ( auto it : mFields )
				{
					if ( it.mName == fieldName )
					{
						TData & result = boost::get< TData >( it.mValue );
						return result;
					}
				}
			}
			catch ( boost::bad_get const& e )
			{
				throw e;
			}

			throw _2Real::NotFoundException( fieldName );
		}

	};
//}
//
//#include "engine/_2RealCustomData.h"
//
//namespace _2Real
//{

	class HumanReadableNameVisitor : public boost::static_visitor< std::string >
	{

	public:

		std::string operator()( const uint8_t val ) const
		{
			( void )( val );
			return "unsigned char";
		}

		std::string operator()( const int8_t val ) const
		{
			( void )( val );
			return "char";
		}

		std::string operator()( const uint32_t val ) const
		{
			( void )( val );
			return "unsigned int";
		}

		std::string operator()( const int32_t val ) const
		{
			( void )( val );
			return "int";
		}

		std::string operator()( const uint64_t val ) const
		{
			( void )( val );
			return "unsigned long";
		}

		std::string operator()( const int64_t val ) const
		{
			( void )( val );
			return "long";
		}

		std::string operator()( double const& val ) const
		{
			( void )( val );
			return "double";
		}

		std::string operator()( const float val ) const
		{
			( void )( val );
			return "float";
		}

		std::string operator()( std::string const& val ) const
		{
			( void )( val );
			return "string";
		}

		std::string operator()( const bool val ) const
		{
			( void )( val );
			return "bool";
		}

		std::string operator()( _2Real::CustomDataItem const& val ) const
		{
			return val.getHumanReadableName();
		}

	};

}