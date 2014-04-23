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

/*
*	metainfo of an inlet, outlet or parameter of a block
*/

namespace _2Real
{
	class IoSlotMetainfoImpl;

	namespace app
	{
		class TypeMetainfo_I;

		class IoSlotMetainfo
		{

		public:

			/*
			*	@created by block metainfo
			*/
			explicit IoSlotMetainfo( std::shared_ptr< const IoSlotMetainfoImpl > );

			/*
			*	@return: true is underlying object is valid
			*/
			bool			isValid() const;

			/*
			*	@return: name
			*/
			std::string		getName() const;

			/*
			*	@return: description
			*/
			std::string		getDescription() const;

			/*
			*	@return: metainfo of datatype
			*/
			std::shared_ptr< TypeMetainfo_I >	getTypeMetainfo() const;

			/*
			*	@return: inital value, use boost::get to extract correct type or better, use boost::apply_visitor
			*/
			DataItem							getInitialValue() const;

			/*
			*	currently, only inlets may be multi inlets -> everything else will always return false
			*/
			bool			isMulti() const;
	
		private:

			std::weak_ptr< const IoSlotMetainfoImpl >		mImpl;

		};

		typedef IoSlotMetainfo InletMetainfo;
		typedef IoSlotMetainfo OutletMetainfo;
		typedef IoSlotMetainfo ParameterMetainfo;
	}
}