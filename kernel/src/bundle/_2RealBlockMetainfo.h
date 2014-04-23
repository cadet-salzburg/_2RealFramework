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
#include "bundle/_2RealIoSlotMetainfo.h"
#include "engine/_2RealBlockFactory_T.h"

namespace _2Real
{
	class BlockMetainfoImpl;

	namespace bundle
	{
		class UpdatePolicyMetainfo;

		class BlockMetainfo
		{

		public:

			explicit BlockMetainfo( std::shared_ptr< BlockMetainfoImpl > );

			/*
			*	@return: name
			*/
			std::string getName() const;

			/*
			*	sets the actual block object
			*/
			template< typename TBlock >
			void setBlockClass()
			{
				std::shared_ptr< BlockFactory_I > f( new BlockFactory_T< TBlock > );
				setBlockClassInternal( f );
			}

			/*
			*	sets the description
			*/
			void setDescription( std::string const& );

			/*
			*	set the dependencies: i.e., all other blocks that must exist before this one can be created
			*/
			void setDependencies( std::vector< std::string > const& );

			/*
			*	@return: outlet metainfo
			*/
			OutletMetainfo			getOutletMetainfo( std::string const& name );

			/*
			*	@return: inlet metainfo
			*/
			InletMetainfo			getInletMetainfo( std::string const& name );

			/*
			*	@return: parameter metainfo
			*/
			ParameterMetainfo		getParameterMetainfo( std::string const& name );

			/*
			*	@return: update policy metainfo
			*/
			UpdatePolicyMetainfo	getUpdatePolicyMetainfo();

		private:

			void setBlockClassInternal( std::shared_ptr< BlockFactory_I > );

			std::shared_ptr< BlockMetainfoImpl >		mImpl;

		};
	}
}