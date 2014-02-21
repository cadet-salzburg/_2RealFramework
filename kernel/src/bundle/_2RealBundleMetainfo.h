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

namespace _2Real
{
	class SharedLibraryMetainfo;

	namespace bundle
	{
		class TypeMetainfo;
		class FunctionBlockMetainfo;
		class ContextBlockMetainfo;

		class BundleMetainfo
		{

		public:

			explicit BundleMetainfo( std::shared_ptr< SharedLibraryMetainfo > );

			void setAuthor( std::string const& );
			void setDescription( std::string const& );
			void setContact( std::string const& );
			void setCategory( std::string const& );
			void setVersion( const unsigned int, const unsigned int, const unsigned int );

			TypeMetainfo createTypeMetainfo( std::string const& name );
			FunctionBlockMetainfo createFunctionBlockMetainfo( std::string const& name );
			ContextBlockMetainfo createContextBlockMetainfo( std::string const& name );

		private:

			std::weak_ptr< SharedLibraryMetainfo >		mImpl;

		};
	}
}

/*
*	TODO: name checking
*/