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

#include "engine/_2RealHumanReadableNameVisitor.h"
#include "engine/_2RealId.h"

namespace _2Real
{
	std::string HumanReadableNameVisitor::operator()( const uint8_t val ) const
	{
		( void )( val );
		return "uchar";
	}

	std::string HumanReadableNameVisitor::operator()( const int8_t val ) const
	{
		( void )( val );
		return "char";
	}

	std::string HumanReadableNameVisitor::operator()( const uint32_t val ) const
	{
		( void )( val );
		return "uint";
	}

	std::string HumanReadableNameVisitor::operator()( const int32_t val ) const
	{
		( void )( val );
		return "int";
	}

	std::string HumanReadableNameVisitor::operator()( const uint64_t val ) const
	{
		( void )( val );
		return "ulong";
	}

	std::string HumanReadableNameVisitor::operator()( const int64_t val ) const
	{
		( void )( val );
		return "long";
	}

	std::string HumanReadableNameVisitor::operator()( const double val ) const
	{
		( void )( val );
		return "double";
	}

	std::string HumanReadableNameVisitor::operator()( const float val ) const
	{
		( void )( val );
		return "float";
	}

	std::string HumanReadableNameVisitor::operator()( std::string const& val ) const
	{
		( void )( val );
		return "string";
	}

	std::string HumanReadableNameVisitor::operator()( const bool val ) const
	{
		( void )( val );
		return "bool";
	}

	std::string HumanReadableNameVisitor::operator()( CustomDataItem const& val ) const
	{
		return val.mTypeMetainfo->getName();
	}
}