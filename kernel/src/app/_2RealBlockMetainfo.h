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

			/*
			*	block metainfo is only valid as long as the dll stays loaded
			*	@return:	true if the handle is still valid			
			*/
			bool			isValid() const;

			/*
			*	@return:	the export name
			*/
			std::string		getName() const;

			/*
			*	@return:	the description
			*/
			std::string		getDescription() const;

			/*
			*	@return:	the names of all other blocks that this block is dependent on
			*				the user is responsible for creating those first
			*				( blocks can only depend on other blocks exported by the same dll )
			*/
			std::vector< std::string >			getDependenciesByName() const;

			/*
			*	@ return:	all io slots of a particular type
			*				in the same order they were declared @dll export
			*/
			std::vector< InletMetainfo >		getInlets() const;
			std::vector< OutletMetainfo >		getOutlets() const;
			std::vector< ParameterMetainfo >	getParameters() const;

			/*
			*	@ return:	metainfo of an io slot
			*/
			InletMetainfo		getInlet( std::string const& );
			OutletMetainfo		getOutlet( std::string const& );
			ParameterMetainfo	getParameter( std::string const& );
			
		private:

			std::weak_ptr< const BlockMetainfoImpl >		mImpl;

		};
	}
}