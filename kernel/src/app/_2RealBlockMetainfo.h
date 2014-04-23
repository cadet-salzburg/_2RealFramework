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
			*	@ return: specific block metainfo, by name
			*/
			bool			isValid() const;

			/*
			*	@ return: name
			*/
			std::string		getName() const;

			/*
			*	@ return: description
			*/
			std::string		getDescription() const;

			/*
			*	@return:the names of all other blocks that this block is dependent on
			*			the user is responsible for creating those first
			*			( blocks can only depend on other blocks exported by the same dll )
			*/
			std::vector< std::string >			getDependenciesByName() const;

			/*
			*	@ return:	all inlets
			*				in the same order they were declared @dll export
			*/
			std::vector< InletMetainfo >		getInlets() const;

			/*
			*	@ return:	all outlets
			*				in the same order they were declared @dll export
			*/
			std::vector< OutletMetainfo >		getOutlets() const;

			/*
			*	@ return:	all parmeters
			*				in the same order they were declared @dll export
			*/
			std::vector< ParameterMetainfo >	getParameters() const;

			/*
			*	@ return:	metainfo of an inlet
			*/
			InletMetainfo		getInlet( std::string const& );

			/*
			*	@ return:	metainfo of an outlet
			*/
			OutletMetainfo		getOutlet( std::string const& );

			/*
			*	@ return:	metainfo of a parameter
			*/
			ParameterMetainfo	getParameter( std::string const& );
			
		private:

			std::weak_ptr< const BlockMetainfoImpl >		mImpl;

		};
	}
}