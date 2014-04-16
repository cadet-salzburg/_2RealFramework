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
#include "app/_2RealIoSlotMetainfo.h"

namespace _2Real
{
	class BlockMetainfoImpl;

	namespace app
	{

		class BlockMetainfo
		{

		public:

			explicit BlockMetainfo( std::shared_ptr< const BlockMetainfoImpl > );

			bool			isValid() const;

			bool			isSingleton() const;
			std::string		getName() const;
			std::string		getDescription() const;

			std::vector< std::string >			getDependenciesByName() const;

			std::vector< InletMetainfo >		getInlets() const;
			std::vector< OutletMetainfo >		getOutlets() const;
			std::vector< ParameterMetainfo >	getParameters() const;

			InletMetainfo		getInlet( std::string const& );
			OutletMetainfo		getOutlet( std::string const& );
			ParameterMetainfo	getParameter( std::string const& );
			
		private:

			std::weak_ptr< const BlockMetainfoImpl >		mImpl;

		};
	}
}