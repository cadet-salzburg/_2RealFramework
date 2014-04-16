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

			std::string getName() const;

			template< typename TBlock >
			void setBlockClass()
			{
				std::shared_ptr< BlockFactory_I > f( new BlockFactory_T< TBlock > );
				setBlockClassInternal( f );
			}

			void setDescription( std::string const& );
			void setDependencies( std::vector< std::string > const& );

			OutletMetainfo			getOutletMetainfo( std::string const& name );
			InletMetainfo			getInletMetainfo( std::string const& name );
			ParameterMetainfo		getParameterMetainfo( std::string const& name );

			UpdatePolicyMetainfo	getUpdatePolicyMetainfo();

		private:

			void setBlockClassInternal( std::shared_ptr< BlockFactory_I > );

			std::shared_ptr< BlockMetainfoImpl >		mImpl;

		};
	}
}