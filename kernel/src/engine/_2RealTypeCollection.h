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

#include "helpers/_2RealStdIncludes.h"
#include <unordered_map>

namespace _2Real
{
	class TMetainfo;
	class SharedTypeMetainfo;
	class MetainfoId;

	class TypeCollection : public std::enable_shared_from_this< TypeCollection >
	{

	public:

		TypeCollection() = default;
		~TypeCollection();

		TypeCollection( TypeCollection const& other ) = delete;
		TypeCollection& operator=( TypeCollection const& other ) = delete;

		void clear();

		void addType( std::shared_ptr< const TMetainfo > );
		std::shared_ptr< const TMetainfo > getMetainfo( std::shared_ptr< const MetainfoId > ) const;

		void typeRemoved( std::shared_ptr< const MetainfoId > info );

	private:

		struct Cmp
		{
			bool operator()( std::shared_ptr< const MetainfoId > one, std::shared_ptr< const MetainfoId > other ) const;
		};

		typedef std::map< std::shared_ptr< const MetainfoId >, std::shared_ptr< const TMetainfo >, Cmp > Types;

		Types								mTypes;

	};

}
