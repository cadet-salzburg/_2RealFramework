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

#include "common/_2RealRemoveWarnings.h"
#include "common/_2RealStdIncludes.h"
#include "common/_2RealData.h"

namespace _2Real
{

	class HumanReadableNameVisitor : public boost::static_visitor< std::string >
	{

	public:

		template< typename TData >
		std::string operator()( std::vector< TData > const& data ) const
		{
			( void )( data );
			std::string result = "vector of ";
			TData *dummyval = new TData;
			result.append( this->operator()( *dummyval ) );
			return result;
		}

		std::string operator()( const uint8_t val ) const;
		std::string operator()( const int8_t val ) const;
		std::string operator()( const uint32_t val ) const;
		std::string operator()( const int32_t val ) const;
		std::string operator()( const uint64_t val ) const;
		std::string operator()( const int64_t val ) const;
		std::string operator()( const double val ) const;
		std::string operator()( const float val ) const;
		std::string operator()( const bool val ) const;
		std::string operator()( std::string const& val ) const;
		std::string operator()( CustomDataItem const& val ) const;

	};

}