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

#include "helpers/_2RealVersion.h"
#include "engine/_2RealInletPolicy.h"
#include "engine/_2RealTypeMetadata.h"

#include <vector>
#include <string>

namespace _2Real
{
	namespace app
	{

		class TypeMetainfo
		{

		public:

			struct FieldMetainfo
			{
				std::string			name;
				std::string			type;
			};

			TypeMetainfo( _2Real::TypeMetadata const& impl ) : mImpl( &impl ) {}

			void getFieldInfo( Fields &f ) const
			{
				mImpl->getFields( f );
			}

			std::pair< std::string, std::string > getTypename() const
			{
				return mImpl->getTypeId();
			}

		private:

			friend class CustomType;
			_2Real::TypeMetadata					const* mImpl;

		};

		struct IOInfo
		{
			IOInfo() : name( "undefined" ), typeName( "undefined" ), defaultPolicy( Policy::INVALID ), isMulti( false ) {}

			std::string								name;
			std::string								typeName;
			Policy									defaultPolicy;
			bool									isMulti;
		};

		struct BlockInfo
		{
			typedef std::vector< IOInfo >						IOInfos;
			typedef std::vector< IOInfo >::iterator				IOInfoIterator;
			typedef std::vector< IOInfo >::const_iterator		IOInfoConstIterator;

			BlockInfo() : name( "undefined" ), description( "undefined" ), category( "undefined" ),
				inlets(), outlets(), parameters() {}

			~BlockInfo()
			{
				parameters.clear();
				outlets.clear();
				inlets.clear();
				category.clear();
				description.clear();
				name.clear();
			}

			std::string			name;
			std::string			description;
			std::string			category;
			IOInfos				inlets;
			IOInfos				outlets;
			IOInfos				parameters;
		};

		struct BundleInfo
		{
			typedef std::vector< BlockInfo >						BlockInfos;
			typedef std::vector< BlockInfo >::iterator				BlockInfoIterator;
			typedef std::vector< BlockInfo >::const_iterator		BlockInfoConstIterator;

			BundleInfo() : exportedBlocks(), name( "undefined" ), directory( "undefined" ), description( "undefined" ),
				author( "undefined" ), contact( "undefined" ), category( "undefined" ), version( 0, 0, 0 ) {}

			BlockInfos			exportedBlocks;
			std::string			name;
			std::string			directory;
			std::string			description;
			std::string			author;
			std::string			contact;
			std::string			category;
			Version				version;
		};
	}
}