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

#include "common/_2RealStdIncludes.h"
#include "common/_2RealData.h"

namespace _2Real
{
	class TypeCollection;
	class TypeMetainfoImpl_I;

	class TypeMetainfoVisitor : public boost::static_visitor< std::shared_ptr< const TypeMetainfoImpl_I > >
	{

	public:

		explicit TypeMetainfoVisitor( std::shared_ptr< const TypeCollection > );

		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const uint8_t val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const int8_t val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const uint32_t val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const int32_t val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const uint64_t val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const int64_t val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const double val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const float val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( const bool val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( std::string const& val ) const;
		std::shared_ptr< const TypeMetainfoImpl_I > operator()( CustomDataItem const& val ) const;

	private:

		std::shared_ptr< const TypeCollection > mTypes;

	};

}